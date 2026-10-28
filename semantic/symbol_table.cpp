#include "symbol_table.h"
#include "types.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Scoring model for identifier repair.
//
//   score = W_SIMILARITY * similarity          (how close the spelling is)
//         + W_TYPE       * type compatibility  (candidate can hold the value)
//         + W_CONTEXT    * context bonus       (type fits AND spelling is close)
//
// Weights sum to 1.0, so a score is directly usable as a confidence value.
// ---------------------------------------------------------------------------
static const double W_SIMILARITY = 0.50;
static const double W_TYPE       = 0.35;
static const double W_CONTEXT    = 0.15;

// The context bonus only applies when the names are at least this similar,
// so a pure type match can never promote an unrelated name.
static const double CONTEXT_MIN_SIMILARITY = 0.50;

// When the usage gives no type information (e.g. the identifier sits inside
// an expression), the type and context weights cannot apply. Scoring on the
// weighted formula alone would cap every candidate at 0.50, so a one-letter
// typo could never be repaired. Instead score on spelling alone, capped so a
// match without type evidence never reaches "High" confidence.
static const double NO_TYPE_EVIDENCE_CAP = 0.90;

static std::string lower(const std::string& s)
{
    std::string r = s;
    for (char& c : r)
        c = (char)std::tolower((unsigned char)c);
    return r;
}

// Optimal-string-alignment distance: Levenshtein plus adjacent transposition
// (so "cuont" -> "count" costs 1, not 2).
static int editDistance(const std::string& a, const std::string& b)
{
    const size_t n = a.size();
    const size_t m = b.size();

    std::vector<std::vector<int>> d(n + 1, std::vector<int>(m + 1, 0));

    for (size_t i = 0; i <= n; i++) d[i][0] = (int)i;
    for (size_t j = 0; j <= m; j++) d[0][j] = (int)j;

    for (size_t i = 1; i <= n; i++)
    {
        for (size_t j = 1; j <= m; j++)
        {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;

            d[i][j] = std::min({
                d[i - 1][j] + 1,
                d[i][j - 1] + 1,
                d[i - 1][j - 1] + cost
            });

            if (i > 1 && j > 1 &&
                a[i - 1] == b[j - 2] &&
                a[i - 2] == b[j - 1])
            {
                d[i][j] = std::min(d[i][j], d[i - 2][j - 2] + 1);
            }
        }
    }

    return d[n][m];
}

// 1.0 = identical, 0.0 = nothing in common. Case-only differences are
// treated as a near-identical spelling.
static double similarity(const std::string& a, const std::string& b)
{
    if (a == b)
        return 1.0;

    if (lower(a) == lower(b))
        return 0.95;

    size_t longest = std::max(a.size(), b.size());

    if (longest == 0)
        return 1.0;

    return 1.0 - (double)editDistance(lower(a), lower(b)) / (double)longest;
}

bool SymbolTable::addSymbol(const std::string& name, const std::string& type)
{
    if (table.find(name) != table.end())
        return false;

    table[name] = {name, type, false};
    return true;
}

bool SymbolTable::contains(const std::string& name)
{
    return table.find(name) != table.end();
}

std::string SymbolTable::getType(const std::string& name)
{
    auto it = table.find(name);
    return it != table.end() ? it->second.type : "";
}

std::string SymbolTable::getClosestSymbol(const std::string& name)
{
    std::string closest;
    double best = -1.0;

    for (const auto& entry : table)
    {
        double s = similarity(name, entry.first);

        // Tie-break on name so the result does not depend on hash order.
        if (s > best || (s == best && entry.first < closest))
        {
            best = s;
            closest = entry.first;
        }
    }

    return closest;
}

std::vector<std::pair<std::string, double>>
SymbolTable::getRepairCandidates(const std::string& name, int expressionType)
{
    std::vector<std::pair<std::string, double>> candidates;

    for (const auto& entry : table)
    {
        const std::string& candidate = entry.first;

        double sim = similarity(name, candidate);

        if (expressionType == TYPE_UNKNOWN)
        {
            candidates.push_back({candidate, NO_TYPE_EVIDENCE_CAP * sim});
            continue;
        }

        double typeScore = 0.0;

        if (isAssignable(typeCode(entry.second.type), expressionType))
        {
            typeScore = W_TYPE;
        }

        double contextScore = 0.0;

        if (typeScore > 0.0 && sim >= CONTEXT_MIN_SIMILARITY)
            contextScore = W_CONTEXT;

        candidates.push_back({candidate,
                              W_SIMILARITY * sim + typeScore + contextScore});
    }

    // Highest score first; ties broken alphabetically for determinism.
    std::sort(candidates.begin(), candidates.end(),
              [](const auto& a, const auto& b)
              {
                  if (a.second != b.second)
                      return a.second > b.second;
                  return a.first < b.first;
              });

    return candidates;
}

void SymbolTable::setInitialized(const std::string& name, bool status)
{
    auto it = table.find(name);

    if (it != table.end())
        it->second.initialized = status;
}

bool SymbolTable::isInitialized(const std::string& name)
{
    auto it = table.find(name);
    return it != table.end() && it->second.initialized;
}

std::vector<Symbol> SymbolTable::getSymbols() const
{
    std::vector<Symbol> symbols;

    for (const auto& entry : table)
        symbols.push_back(entry.second);

    std::sort(symbols.begin(), symbols.end(),
              [](const Symbol& a, const Symbol& b) { return a.name < b.name; });

    return symbols;
}
