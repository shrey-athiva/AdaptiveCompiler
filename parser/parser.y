%code requires
{
    #include <string>

    struct ExprValue
    {
        int type;            // TypeCode (see semantic/types.h)
        std::string place;   // variable, literal or temporary holding the value
    };
}

%code
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <utility>
#include "../semantic/symbol_table.h"
#include "../semantic/types.h"
#include "../ir/ir_generator.h"
#include "../recovery/recovery.h"

int yylex(void);
void yyerror(const char *s);

extern int yylineno;
extern int lastTokenLine;   // line of the last non-newline token (lexer.l)

int if_line = 0;            // set by the lexer when it sees 'if'

bool syntaxRecoveryOccurred = false;   // a repair was applied (e.g. missing ';')
bool syntaxErrorOccurred    = false;   // a statement had to be skipped
bool semanticErrorOccurred  = false;
bool lexicalErrorOccurred   = false;

SymbolTable symbolTable;
IRGenerator irGenerator;

std::vector<std::pair<std::string, std::string>> ifLabelStack;     // {false, end}
std::vector<std::pair<std::string, std::string>> whileLabelStack;  // {start, end}
std::vector<IRGenerator::Checkpoint> whileIRCheckpoints;
std::vector<bool> whileValidityStack;
std::vector<IRGenerator::Checkpoint> ifIRCheckpoints;
std::vector<bool> ifValidityStack;

// ---------------------------------------------------------------------------
// Statement boundary: the IR and bookkeeping-stack state at the last point
// where a new statement could start. When a statement is skipped by error
// recovery we return to this state, so half-built IR and stale stack entries
// from the discarded statement do not leak into the output.
// ---------------------------------------------------------------------------
struct StatementBoundary
{
    IRGenerator::Checkpoint ir;
    size_t ifLabels, whileLabels, ifIR, ifValidity, whileIR, whileValidity;
};

StatementBoundary boundary = {{0, 0, 0}, 0, 0, 0, 0, 0, 0};

template <typename T>
static void truncateTo(std::vector<T>& v, size_t n)
{
    if (v.size() > n)
        v.erase(v.begin() + n, v.end());
}

static void markStatementBoundary()
{
    boundary.ir            = irGenerator.createCheckpoint();
    boundary.ifLabels      = ifLabelStack.size();
    boundary.whileLabels   = whileLabelStack.size();
    boundary.ifIR          = ifIRCheckpoints.size();
    boundary.ifValidity    = ifValidityStack.size();
    boundary.whileIR       = whileIRCheckpoints.size();
    boundary.whileValidity = whileValidityStack.size();
}

static void restoreStatementBoundary()
{
    irGenerator.rollback(boundary.ir);
    truncateTo(ifLabelStack,      boundary.ifLabels);
    truncateTo(whileLabelStack,   boundary.whileLabels);
    truncateTo(ifIRCheckpoints,   boundary.ifIR);
    truncateTo(ifValidityStack,   boundary.ifValidity);
    truncateTo(whileIRCheckpoints, boundary.whileIR);
    truncateTo(whileValidityStack, boundary.whileValidity);
}

static bool isValidComparison(int leftType, int rightType, const std::string& op)
{
    if (isNumeric(leftType) && isNumeric(rightType))
        return true;

    // char/char and bool/bool: equality only
    if ((leftType == TYPE_CHAR && rightType == TYPE_CHAR) ||
        (leftType == TYPE_BOOL && rightType == TYPE_BOOL))
        return op == "==" || op == "!=";

    return false;
}

// Validate a condition. Operands with an already-reported error make the
// condition invalid without adding a second diagnostic.
static bool checkComparison(int l, int r, const std::string& op,
                            int line, const char* what)
{
    if (l == TYPE_UNKNOWN || r == TYPE_UNKNOWN)
    {
        semanticErrorOccurred = true;
        return false;
    }

    if (isValidComparison(l, r, op))
        return true;

    semanticErrorOccurred = true;
    reportError("Semantic Error", "BAD_COMPARISON", line,
                std::string("Invalid comparison between incompatible types. ")
                + what + " statement IR was skipped.");
    return false;
}

// Shared by the normal and the missing-')' IF header.
static void beginIf(ExprValue* left, const char* op, ExprValue* right)
{
    std::string trueLabel  = irGenerator.newLabel();
    std::string falseLabel = irGenerator.newLabel();
    std::string endLabel   = irGenerator.newLabel();

    bool valid = checkComparison(left->type, right->type, op, if_line, "IF");
    ifValidityStack.push_back(valid);

    if (valid)
    {
        irGenerator.emit("IF " + left->place + " " + op + " " +
                         right->place + " GOTO " + trueLabel);
        irGenerator.emit("GOTO " + falseLabel);
    }

    irGenerator.emit(trueLabel + ":");
    ifLabelStack.push_back({falseLabel, endLabel});

    delete left;
    delete right;

    markStatementBoundary();
}

// Finalize one IF. An invalid IF is rolled back in full, including the IR of
// the statements in its body.
static void finishIfIR()
{
    if (ifIRCheckpoints.empty() || ifValidityStack.empty())
        return;

    if (!ifValidityStack.back())
        irGenerator.rollback(ifIRCheckpoints.back());

    ifValidityStack.pop_back();
    ifIRCheckpoints.pop_back();
}

// Arithmetic. `verb` is only used in the diagnostic text.
static ExprValue* makeBinary(const char* irOp, const char* verb,
                             ExprValue* l, ExprValue* r)
{
    ExprValue* result = new ExprValue;
    result->type  = TYPE_UNKNOWN;
    result->place = "0";

    if (l->type == TYPE_UNKNOWN || r->type == TYPE_UNKNOWN)
    {
        // An operand already reported its own error; stay silent.
        semanticErrorOccurred = true;
    }
    else if (!isNumeric(l->type) || !isNumeric(r->type))
    {
        semanticErrorOccurred = true;
        reportError("Type Error", "NON_NUMERIC_OPERAND", lastTokenLine,
                    std::string(verb) + " requires numeric operands.");
    }
    else
    {
        result->type = (l->type == TYPE_FLOAT || r->type == TYPE_FLOAT)
                           ? TYPE_FLOAT : TYPE_INT;

        std::string temp = irGenerator.newTemp();
        irGenerator.emit(std::string(irOp) + " " + l->place + " " +
                         r->place + " -> " + temp);
        result->place = temp;
    }

    delete l;
    delete r;
    return result;
}

}

%define parse.error verbose

%union
{
    char *str;
    char *lexeme;
    const char *op;
    int type;
    ExprValue *expr;
}

%destructor { free($$); }   <str> <lexeme>
%destructor { delete $$; }  <expr>

%token INT FLOAT CHAR BOOL IF ELSE WHILE
%token <lexeme> CHARACTER BOOLEAN

%token <str> IDENTIFIER
%token <lexeme> NUMBER

%token EQ NE GE LE GT LT
%type <op> comparison_operator
%token PLUS MINUS MULT DIV ASSIGN

%token SEMICOLON LPAREN RPAREN LBRACE RBRACE
%token NEWLINE

%type <type> type
%type <expr> expression
%type <expr> term
%type <expr> factor

%%

program:
    optional_newlines function_definition optional_newlines
    ;

optional_newlines:
    %empty
    | optional_newlines NEWLINE
    ;

function_definition:
    type IDENTIFIER LPAREN RPAREN LBRACE
    {
        markStatementBoundary();
    }
    statements RBRACE
    {
        free($2);
    }
    ;

statements:
    %empty
    | statements statement
    {
        markStatementBoundary();
    }
    ;

statement:
    declaration
    | assignment
    | if_statement
    | while_statement
    | NEWLINE
    | error SEMICOLON
    {
        syntaxErrorOccurred = true;

        // Discard everything the skipped statement had emitted.
        restoreStatementBoundary();

        printf("Note: skipped the invalid statement ending at line %d "
               "and continued parsing.\n\n", lastTokenLine);

        yyerrok;
    }
    ;

type:
    INT       { $$ = TYPE_INT; }
    | FLOAT   { $$ = TYPE_FLOAT; }
    | CHAR    { $$ = TYPE_CHAR; }
    | BOOL    { $$ = TYPE_BOOL; }
    ;

declaration:
    type IDENTIFIER ASSIGN expression declaration_end
    {
        std::string variableType = typeName($1);

        if (!symbolTable.addSymbol($2, variableType))
        {
            semanticErrorOccurred = true;
            reportError("Semantic Error", "DUPLICATE_DECLARATION", lastTokenLine,
                        "Duplicate declaration of '" + std::string($2) + "'");
        }
        else if ($4->type == TYPE_UNKNOWN)
        {
            // The initializer already reported its error.
            semanticErrorOccurred = true;
        }
        else if (!isAssignable($1, $4->type))
        {
            semanticErrorOccurred = true;
            reportTypeMismatch("initializer", $1, $2, $4->type, lastTokenLine);
        }
        else
        {
            irGenerator.emit("DECLARE " + variableType + " " + std::string($2));
            irGenerator.emit("ASSIGN " + std::string($2) + " " + $4->place);
            symbolTable.setInitialized($2, true);
        }

        delete $4;
        free($2);
    }

    | type IDENTIFIER declaration_end
    {
        std::string variableType = typeName($1);

        if (!symbolTable.addSymbol($2, variableType))
        {
            semanticErrorOccurred = true;
            reportError("Semantic Error", "DUPLICATE_DECLARATION", lastTokenLine,
                        "Duplicate declaration of '" + std::string($2) + "'");
        }
        else
        {
            irGenerator.emit("DECLARE " + variableType + " " + std::string($2));
        }

        free($2);
    }
    ;

declaration_end:
    SEMICOLON
    | NEWLINE
    {
        syntaxRecoveryOccurred = true;
        reportMissingSemicolon("declaration", lastTokenLine);
    }
    ;

assignment_end:
    SEMICOLON
    | NEWLINE
    {
        syntaxRecoveryOccurred = true;
        reportMissingSemicolon("assignment", lastTokenLine);
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression assignment_end
    {
        if (!symbolTable.contains($1))
        {
            semanticErrorOccurred = true;
            reportUndeclared(symbolTable, $1, $3->type, lastTokenLine);
        }
        else
        {
            int target = typeCode(symbolTable.getType($1));

            if ($3->type == TYPE_UNKNOWN)
            {
                // The right-hand side already reported its error.
                semanticErrorOccurred = true;
            }
            else if (!isAssignable(target, $3->type))
            {
                semanticErrorOccurred = true;
                reportTypeMismatch("assignment", target, $1, $3->type,
                                   lastTokenLine);
            }
            else
            {
                irGenerator.emit("ASSIGN " + std::string($1) + " " + $3->place);
                symbolTable.setInitialized($1, true);
            }
        }

        delete $3;
        free($1);
    }
    ;

if_statement:
    if_header LBRACE statements RBRACE ELSE
    {
        irGenerator.emit("GOTO " + ifLabelStack.back().second);
        irGenerator.emit(ifLabelStack.back().first + ":");
        markStatementBoundary();
    }
    LBRACE statements RBRACE
    {
        irGenerator.emit(ifLabelStack.back().second + ":");
        ifLabelStack.pop_back();
        finishIfIR();
    }

    | if_header LBRACE statements RBRACE
    {
        irGenerator.emit("GOTO " + ifLabelStack.back().second);
        irGenerator.emit(ifLabelStack.back().first + ":");
        irGenerator.emit(ifLabelStack.back().second + ":");

        ifLabelStack.pop_back();
        finishIfIR();
    }
    ;

if_start:
    IF
    {
        ifIRCheckpoints.push_back(irGenerator.createCheckpoint());
    }
    ;

if_header:
    if_start LPAREN expression comparison_operator expression RPAREN
    {
        beginIf($3, $4, $5);
    }

    // Recovery rule: the closing ')' is missing before '{'.
    | if_start LPAREN expression comparison_operator expression
    {
        syntaxRecoveryOccurred = true;
        reportMissingRParen(if_line);
        beginIf($3, $4, $5);
    }
    ;

while_statement:
    WHILE
    {
        whileIRCheckpoints.push_back(irGenerator.createCheckpoint());

        std::string startLabel = irGenerator.newLabel();
        irGenerator.emit(startLabel + ":");

        whileLabelStack.push_back({startLabel, ""});
        whileValidityStack.push_back(false);
    }

    LPAREN expression comparison_operator expression RPAREN
    {
        bool valid = checkComparison($4->type, $6->type, $5,
                                     lastTokenLine, "WHILE");

        whileValidityStack.back() = valid;

        if (valid)
        {
            std::string bodyLabel = irGenerator.newLabel();
            std::string endLabel  = irGenerator.newLabel();

            irGenerator.emit("IF " + $4->place + " " + $5 + " " +
                             $6->place + " GOTO " + bodyLabel);
            irGenerator.emit("GOTO " + endLabel);
            irGenerator.emit(bodyLabel + ":");

            whileLabelStack.back().second = endLabel;
        }

        delete $4;
        delete $6;

        markStatementBoundary();
    }

    LBRACE statements RBRACE
    {
        if (whileValidityStack.back())
        {
            irGenerator.emit("GOTO " + whileLabelStack.back().first);
            irGenerator.emit(whileLabelStack.back().second + ":");
        }
        else
        {
            // Remove the entire invalid WHILE (condition and body).
            irGenerator.rollback(whileIRCheckpoints.back());
        }

        whileLabelStack.pop_back();
        whileValidityStack.pop_back();
        whileIRCheckpoints.pop_back();
    }
    ;

comparison_operator:
    LT      { $$ = "<"; }
    | GT    { $$ = ">"; }
    | LE    { $$ = "<="; }
    | GE    { $$ = ">="; }
    | EQ    { $$ = "=="; }
    | NE    { $$ = "!="; }
    ;

expression:
    expression PLUS term
    {
        $$ = makeBinary("ADD", "Addition", $1, $3);
    }
    | expression MINUS term
    {
        $$ = makeBinary("SUB", "Subtraction", $1, $3);
    }
    | term
    {
        $$ = $1;
    }
    ;

term:
    term MULT factor
    {
        $$ = makeBinary("MUL", "Multiplication", $1, $3);
    }
    | term DIV factor
    {
        $$ = makeBinary("DIV", "Division", $1, $3);
    }
    | factor
    {
        $$ = $1;
    }
    ;

factor:
    NUMBER
    {
        ExprValue *result = new ExprValue;
        result->type  = (strchr($1, '.') != NULL) ? TYPE_FLOAT : TYPE_INT;
        result->place = $1;
        $$ = result;
        free($1);
    }
    | CHARACTER
    {
        ExprValue *result = new ExprValue;
        result->type  = TYPE_CHAR;
        result->place = $1;
        $$ = result;
        free($1);
    }
    | BOOLEAN
    {
        ExprValue *result = new ExprValue;
        result->type  = TYPE_BOOL;
        result->place = $1;
        $$ = result;
        free($1);
    }
    | IDENTIFIER
    {
        ExprValue *result = new ExprValue;
        result->place = $1;

        if (symbolTable.contains($1))
        {
            result->type = typeCode(symbolTable.getType($1));

            if (!symbolTable.isInitialized($1))
            {
                reportWarning(lastTokenLine,
                    "Variable '" + std::string($1) +
                        "' may be used before initialization.",
                    "Assign a value to '" + std::string($1) +
                        "' before using it.");
            }
        }
        else
        {
            semanticErrorOccurred = true;
            reportUndeclared(symbolTable, $1, TYPE_UNKNOWN, lastTokenLine);
            result->type = TYPE_UNKNOWN;
        }

        $$ = result;
        free($1);
    }
    | LPAREN expression RPAREN
    {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s)
{
    syntaxErrorOccurred = true;
    reportError("Syntax Error", "SYNTAX", lastTokenLine, s);
}

int main()
{
    printf("Enter source code:\n");

    int parseResult = yyparse();

    const char *result;
    const char *message;
    int exitCode;

    DiagnosticCounters counters = getDiagnosticCounters();

    if (parseResult != 0)
    {
        result = "syntax_error";
        message = "Compilation failed: unrecovered syntax error.";
        exitCode = 1;
    }
    else if (lexicalErrorOccurred)
    {
        result = "lexical_error";
        message = "Compilation failed: lexical errors.";
        exitCode = 1;
    }
    else if (semanticErrorOccurred)
    {
        result = "semantic_error";
        message = "Compilation completed with semantic errors.";
        exitCode = 1;
    }
    else if (syntaxErrorOccurred)
    {
        // A statement was discarded, so the IR is incomplete: not a success.
        result = "syntax_error";
        message = "Compilation completed with syntax errors; "
                  "skipped statements were not compiled.";
        exitCode = 1;
    }
    else if (syntaxRecoveryOccurred)
    {
        result = "recovered";
        message = "Compilation successful with syntax recovery.";
        exitCode = 0;
    }
    else if (counters.warnings > 0)
    {
        result = "warnings";
        message = "Compilation successful with warnings.";
        exitCode = 0;
    }
    else
    {
        result = "clean";
        message = "Syntax analysis successful.";
        exitCode = 0;
    }

    printf("\n%s\n", message);

    // Machine-readable summary (consumed by the web UI and llm_repair/).
    printf("RESULT:%s\n", result);
    printf("SUMMARY:errors=%d warnings=%d recoveries=%d\n",
           counters.errors, counters.warnings, counters.recoveries);

    printf("SYMBOLS:");
    bool first = true;
    for (const Symbol& s : symbolTable.getSymbols())
    {
        printf("%s%s:%s:%d", first ? "" : ",", s.name.c_str(),
               s.type.c_str(), s.initialized ? 1 : 0);
        first = false;
    }
    printf("\n");

    irGenerator.print();
    return exitCode;
}
