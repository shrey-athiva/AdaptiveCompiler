#include "recovery.h"
#include <cstdio>
#include <vector>
#include <utility>

static DiagnosticCounters counters = {0, 0, 0};

DiagnosticCounters getDiagnosticCounters()
{
    return counters;
}

const char* confidenceLabel(double score)
{
    if (score >= 0.85) return "High";
    if (score >= 0.65) return "Medium";
    return "Low";
}

void printConfidence(double score)
{
    printf("Confidence: %s (%.0f%%)\n", confidenceLabel(score), score * 100.0);
}

void markDiag(const char* severity, const char* code, int line, double confidence)
{
    std::string s = severity;

    if (s == "error")         counters.errors++;
    else if (s == "warning")  counters.warnings++;
    else if (s == "recovery") counters.recoveries++;

    printf("DIAG:%s:%d:%s:%.2f\n", severity, line, code, confidence);
}

void reportError(const char* label, const char* code, int line,
                 const std::string& message, double confidence)
{
    printf("%s at line %d: %s\n", label, line, message.c_str());
    markDiag("error", code, line, confidence);
}

void reportWarning(int line, const std::string& message,
                   const std::string& suggestion)
{
    printf("Semantic Warning at line %d: %s\n", line, message.c_str());
    printf("Suggestion: %s\n", suggestion.c_str());
    markDiag("warning", "MAYBE_UNINITIALIZED", line, CONF_NONE);
    printf("\n");
}

// ---------------------------------------------------------------------------
// Undeclared identifier: rank similar names, otherwise propose a declaration.
// usageType is the type of the value the identifier is used with, if known.
// ---------------------------------------------------------------------------
void reportUndeclared(SymbolTable& table, const std::string& name,
                      int usageType, int line)
{
    printf("Semantic Error at line %d: Variable '%s' is not declared.\n",
           line, name.c_str());

    auto candidates = table.getRepairCandidates(name, usageType);
    double confidence;

    if (!candidates.empty() && candidates[0].second >= MIN_REPAIR_SCORE)
    {
        printf("\nCandidate Repairs:\n");

        size_t limit = candidates.size() < 3 ? candidates.size() : 3;

        for (size_t i = 0; i < limit; i++)
        {
            printf("%zu. Replace '%s' with '%s' | Score: %.0f%%\n",
                   i + 1, name.c_str(), candidates[i].first.c_str(),
                   candidates[i].second * 100.0);
        }

        const std::string& best = candidates[0].first;
        confidence = candidates[0].second;

        printf("\nSuggested Repair: Replace '%s' with '%s'\n",
               name.c_str(), best.c_str());
        printf("REPAIR:REPLACE_IDENTIFIER:%s:%s\n", name.c_str(), best.c_str());
        printConfidence(confidence);
    }
    else
    {
        // Infer the declaration type from usage; default to int.
        const char* inferred =
            (usageType == TYPE_FLOAT || usageType == TYPE_CHAR ||
             usageType == TYPE_BOOL) ? typeName(usageType) : "int";

        confidence = CONF_DECLARE_VARIABLE;

        printf("Suggested Repair: Declare '%s' before using it.\n", name.c_str());
        printf("Suggested Declaration: %s %s;\n", inferred, name.c_str());
        printf("REPAIR:DECLARE_VARIABLE:%s:%s:%d\n", name.c_str(), inferred, line);
        printConfidence(confidence);
    }

    printf("CONF:%.2f\n", confidence);
    markDiag("error", "UNDECLARED", line, confidence);
    printf("\n");
}

// ---------------------------------------------------------------------------
// Type mismatch in a declaration initializer or an assignment.
// ---------------------------------------------------------------------------
void reportTypeMismatch(const char* context, int targetType,
                        const std::string& name, int valueType, int line)
{
    if (std::string(context) == "initializer")
        printf("Type Error at line %d: Incompatible initializer for %s variable '%s'.\n",
               line, typeName(targetType), name.c_str());
    else
        printf("Type Error at line %d: Incompatible expression assigned to %s variable '%s'.\n",
               line, typeName(targetType), name.c_str());

    double confidence = CONF_TYPE_GUESS;

    if (targetType == TYPE_INT && valueType == TYPE_FLOAT)
    {
        printf("Suggested Repair: Declare '%s' as float, or use an integer expression.\n",
               name.c_str());
        confidence = CONF_WIDEN_TYPE;
    }
    else if (isNumeric(targetType))
    {
        printf("Suggested Repair: Use a numeric expression for '%s'.\n", name.c_str());
    }
    else if (targetType == TYPE_CHAR)
    {
        printf("Suggested Repair: Use a character literal such as 'a'.\n");
    }
    else if (targetType == TYPE_BOOL)
    {
        printf("Suggested Repair: Use true or false.\n");
    }
    else
    {
        printf("Suggested Repair: Use an expression compatible with the declared type.\n");
    }

    printConfidence(confidence);
    markDiag("error", "TYPE_MISMATCH", line, confidence);
    printf("\n");
}

void reportMissingSemicolon(const char* statementKind, int line)
{
    printf("Error at line %d: Missing ';' after %s\n", line, statementKind);
    printf("Suggested Repair: Add ';'\n");
    printConfidence(CONF_ADD_SEMICOLON);
    printf("REPAIR:ADD_SEMICOLON:%d\n", line);
    printf("CONF:%.2f\n", CONF_ADD_SEMICOLON);
    markDiag("recovery", "ADD_SEMICOLON", line, CONF_ADD_SEMICOLON);
    printf("\n");
}

void reportMissingRParen(int line)
{
    printf("Syntax Error at line %d: Missing ')' in IF condition.\n", line);
    printf("Suggested Repair: Add ')' before '{'.\n");
    printConfidence(CONF_ADD_RPAREN);
    printf("REPAIR:ADD_RPAREN:%d\n", line);      // this marker was missing before
    printf("CONF:%.2f\n", CONF_ADD_RPAREN);
    markDiag("recovery", "ADD_RPAREN", line, CONF_ADD_RPAREN);
    printf("\n");
}
