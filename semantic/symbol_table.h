#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

struct Symbol
{
    std::string name;
    std::string type;
    bool initialized;
};

class SymbolTable
{
private:
    std::unordered_map<std::string, Symbol> table;

public:
    bool addSymbol(const std::string& name, const std::string& type);
    bool contains(const std::string& name);
    std::string getType(const std::string& name);

    // Most lexically similar declared name ("" if the table is empty).
    std::string getClosestSymbol(const std::string& name);

    // Ranked replacement candidates for an undeclared identifier.
    // expressionType is a TypeCode describing what the identifier is used
    // with (TYPE_UNKNOWN when there is no usable context).
    std::vector<std::pair<std::string, double>> getRepairCandidates(
        const std::string& name,
        int expressionType
    );

    void setInitialized(const std::string& name, bool status);
    bool isInitialized(const std::string& name);

    // All symbols sorted by name (used for the SYMBOLS marker / LLM context).
    std::vector<Symbol> getSymbols() const;
};

#endif
