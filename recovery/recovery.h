#ifndef RECOVERY_H
#define RECOVERY_H

#include <string>
#include "../semantic/symbol_table.h"
#include "../semantic/types.h"

// ---------------------------------------------------------------------------
// Confidence values.
//
// Identifier repairs use the computed similarity/type score from the symbol
// table. Syntax repairs are deterministic grammar rules, so they use fixed
// priors (documented here, in one place, instead of scattered string literals).
// ---------------------------------------------------------------------------
const double CONF_ADD_SEMICOLON    = 0.95;  // statement complete up to the newline
const double CONF_ADD_RPAREN       = 0.95;  // condition complete, '{' follows
const double CONF_DECLARE_VARIABLE = 0.70;  // type inferred from usage only
const double CONF_WIDEN_TYPE       = 0.70;  // e.g. int x = <float>: clear fix, changes the declaration
const double CONF_TYPE_GUESS       = 0.40;  // several plausible fixes; needs developer intent
const double CONF_NONE             = 0.0;   // no deterministic repair available
const double MIN_REPAIR_SCORE      = 0.70;  // below this, don't suggest an identifier replacement

const char* confidenceLabel(double score);          // "High" | "Medium" | "Low"
void printConfidence(double score);                 // prints "Confidence: High (95%)"

// Machine-readable diagnostic marker + counters.
//   DIAG:<severity>:<line>:<CODE>:<confidence>
// severity: error | warning | recovery
void markDiag(const char* severity, const char* code, int line, double confidence);

// Human-readable error plus its DIAG marker.
void reportError(const char* label, const char* code, int line,
                 const std::string& message, double confidence = CONF_NONE);

void reportWarning(int line, const std::string& message,
                   const std::string& suggestion);

// Specific, repair-bearing diagnostics.
void reportUndeclared(SymbolTable& table, const std::string& name,
                      int usageType, int line);

void reportTypeMismatch(const char* context, int targetType,
                        const std::string& name, int valueType, int line);

void reportMissingSemicolon(const char* statementKind, int line);
void reportMissingRParen(int line);

struct DiagnosticCounters
{
    int errors;
    int warnings;
    int recoveries;
};

DiagnosticCounters getDiagnosticCounters();

#endif
