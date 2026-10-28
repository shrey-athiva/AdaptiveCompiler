#ifndef TYPES_H
#define TYPES_H

#include <string>

// Type codes carried by ExprValue in the parser.
// TYPE_UNKNOWN means "an error was already reported for this expression";
// it is propagated silently so one mistake yields one diagnostic.
enum TypeCode
{
    TYPE_UNKNOWN = 0,
    TYPE_INT     = 1,
    TYPE_FLOAT   = 2,
    TYPE_CHAR    = 3,
    TYPE_BOOL    = 4
};

inline const char* typeName(int code)
{
    switch (code)
    {
        case TYPE_INT:   return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_CHAR:  return "char";
        case TYPE_BOOL:  return "bool";
        default:         return "unknown";
    }
}

inline int typeCode(const std::string& name)
{
    if (name == "int")   return TYPE_INT;
    if (name == "float") return TYPE_FLOAT;
    if (name == "char")  return TYPE_CHAR;
    if (name == "bool")  return TYPE_BOOL;
    return TYPE_UNKNOWN;
}

inline bool isNumeric(int t)
{
    return t == TYPE_INT || t == TYPE_FLOAT;
}

// Can a value of type `valueType` be stored in a variable of type `target`?
// Single source of truth for declarations, assignments and repair scoring.
inline bool isAssignable(int target, int valueType)
{
    switch (target)
    {
        case TYPE_INT:   return valueType == TYPE_INT;
        case TYPE_FLOAT: return valueType == TYPE_INT || valueType == TYPE_FLOAT;
        case TYPE_CHAR:  return valueType == TYPE_CHAR;
        case TYPE_BOOL:  return valueType == TYPE_BOOL;
        default:         return false;
    }
}

#endif
