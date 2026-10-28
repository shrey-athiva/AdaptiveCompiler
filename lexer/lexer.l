
%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.tab.h"
#include "../recovery/recovery.h"

extern int if_line;
extern bool lexicalErrorOccurred;

int skip_next_newline = 0;

// Line of the most recent non-newline lexeme. Diagnostics use this instead of
// yylineno, which has already advanced when a NEWLINE token is returned.
int lastTokenLine = 1;

#define YY_USER_ACTION if (yytext[0] != '\n') lastTokenLine = yylineno;
%}

%%

"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"bool"      { return BOOL; }

"true"      { yylval.lexeme = strdup(yytext); return BOOLEAN; }
"false"     { yylval.lexeme = strdup(yytext); return BOOLEAN; }

"if"        { if_line = yylineno; return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }

"=="        { return EQ; }
"!="        { return NE; }
">="        { return GE; }
"<="        { return LE; }
">"         { return GT; }
"<"         { return LT; }
"="         { return ASSIGN; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }

";"         { return SEMICOLON; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         {
                skip_next_newline = 1;
                return RBRACE;
            }

\'([^\\'\n]|\\.)\' {
    yylval.lexeme = strdup(yytext);
    return CHARACTER;
}

[0-9]+(\.[0-9]+)? {
    yylval.lexeme = strdup(yytext);
    return NUMBER;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r]+     ;

\n {
    if (skip_next_newline)
    {
        skip_next_newline = 0;
    }
    else
    {
        return NEWLINE;
    }
}

. {
    // Report, skip the character and keep scanning so later errors are
    // still found. The compile result is marked as failed in main().
    lexicalErrorOccurred = true;
    reportError("Lexical Error", "LEXICAL", yylineno,
                std::string("Unknown character '") + yytext + "'");
}

%%
