/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_CHAR = 5,                       /* CHAR  */
  YYSYMBOL_BOOL = 6,                       /* BOOL  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_CHARACTER = 10,                 /* CHARACTER  */
  YYSYMBOL_BOOLEAN = 11,                   /* BOOLEAN  */
  YYSYMBOL_IDENTIFIER = 12,                /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 13,                    /* NUMBER  */
  YYSYMBOL_EQ = 14,                        /* EQ  */
  YYSYMBOL_NE = 15,                        /* NE  */
  YYSYMBOL_GE = 16,                        /* GE  */
  YYSYMBOL_LE = 17,                        /* LE  */
  YYSYMBOL_GT = 18,                        /* GT  */
  YYSYMBOL_LT = 19,                        /* LT  */
  YYSYMBOL_PLUS = 20,                      /* PLUS  */
  YYSYMBOL_MINUS = 21,                     /* MINUS  */
  YYSYMBOL_MULT = 22,                      /* MULT  */
  YYSYMBOL_DIV = 23,                       /* DIV  */
  YYSYMBOL_ASSIGN = 24,                    /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 25,                 /* SEMICOLON  */
  YYSYMBOL_LPAREN = 26,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 27,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 28,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 29,                    /* RBRACE  */
  YYSYMBOL_NEWLINE = 30,                   /* NEWLINE  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_program = 32,                   /* program  */
  YYSYMBOL_optional_newlines = 33,         /* optional_newlines  */
  YYSYMBOL_function_definition = 34,       /* function_definition  */
  YYSYMBOL_35_1 = 35,                      /* $@1  */
  YYSYMBOL_statements = 36,                /* statements  */
  YYSYMBOL_statement = 37,                 /* statement  */
  YYSYMBOL_type = 38,                      /* type  */
  YYSYMBOL_declaration = 39,               /* declaration  */
  YYSYMBOL_declaration_end = 40,           /* declaration_end  */
  YYSYMBOL_assignment_end = 41,            /* assignment_end  */
  YYSYMBOL_assignment = 42,                /* assignment  */
  YYSYMBOL_if_statement = 43,              /* if_statement  */
  YYSYMBOL_44_2 = 44,                      /* $@2  */
  YYSYMBOL_if_start = 45,                  /* if_start  */
  YYSYMBOL_if_header = 46,                 /* if_header  */
  YYSYMBOL_while_statement = 47,           /* while_statement  */
  YYSYMBOL_48_3 = 48,                      /* $@3  */
  YYSYMBOL_49_4 = 49,                      /* $@4  */
  YYSYMBOL_comparison_operator = 50,       /* comparison_operator  */
  YYSYMBOL_expression = 51,                /* expression  */
  YYSYMBOL_term = 52,                      /* term  */
  YYSYMBOL_factor = 53                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 13 "parser/parser.y"

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


#line 346 "parser/parser.tab.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   113

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  91

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   237,   237,   241,   242,   247,   246,   257,   258,   265,
     266,   267,   268,   269,   270,   285,   286,   287,   288,   292,
     323,   343,   344,   352,   353,   361,   397,   396,   409,   421,
     428,   434,   444,   455,   443,   500,   501,   502,   503,   504,
     505,   509,   513,   517,   524,   528,   532,   539,   547,   555,
     563,   591
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT", "CHAR",
  "BOOL", "IF", "ELSE", "WHILE", "CHARACTER", "BOOLEAN", "IDENTIFIER",
  "NUMBER", "EQ", "NE", "GE", "LE", "GT", "LT", "PLUS", "MINUS", "MULT",
  "DIV", "ASSIGN", "SEMICOLON", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
  "NEWLINE", "$accept", "program", "optional_newlines",
  "function_definition", "$@1", "statements", "statement", "type",
  "declaration", "declaration_end", "assignment_end", "assignment",
  "if_statement", "$@2", "if_start", "if_header", "while_statement", "$@3",
  "$@4", "comparison_operator", "expression", "term", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-41)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -41,     2,     1,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
       6,    -7,    -1,    16,     5,   -41,   -41,    10,    20,   -41,
     -41,    37,   -41,   -41,   -41,    51,   -41,   -41,   -41,    45,
      48,   -41,   -41,    58,    68,    83,    68,   -41,    68,   -41,
     -41,   -41,   -41,    68,    21,   -13,   -41,    68,   -41,   -41,
     -41,    82,    23,    82,    17,    68,    68,   -41,   -41,   -41,
      68,    68,    65,   -41,   -41,   -41,   -41,   -41,   -41,    68,
      69,    68,   -41,   -13,   -13,   -41,   -41,   -41,    28,   -41,
      84,   -41,    59,   -41,   -41,    60,    53,   -41,   -41,    63,
     -41
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,    15,    16,    17,    18,     4,     3,
       0,     2,     0,     0,     0,     5,     7,     0,     0,    29,
      32,     0,     6,    13,     8,     0,     9,    10,    11,     0,
       0,    12,    14,     0,     0,     0,     0,     7,     0,    48,
      49,    50,    47,     0,     0,    43,    46,     0,    21,    22,
      20,     0,     0,     0,     0,     0,     0,    23,    24,    25,
       0,     0,     0,    39,    40,    38,    37,    36,    35,     0,
      28,     0,    51,    41,    42,    44,    45,    19,    31,    26,
       0,    30,     0,    33,     7,     0,     0,     7,    27,     0,
      34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,    80,   -41,   -41,   -37,   -41,    89,   -41,    44,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,    56,
     -35,    18,   -40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     9,    16,    17,    24,    25,    26,    50,
      59,    27,    28,    82,    29,    30,    31,    33,    85,    69,
      44,    45,    46
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      52,    51,     3,    53,     4,     5,     6,     7,    54,    60,
      61,    18,    62,     4,     5,     6,     7,    19,    12,    20,
      75,    76,    21,     8,    18,    13,     4,     5,     6,     7,
      19,     8,    20,    15,    78,    21,    80,    55,    56,    22,
      23,    55,    56,    14,    72,    32,    57,    86,    55,    56,
      89,    58,    70,    23,    18,    81,     4,     5,     6,     7,
      19,    34,    20,    35,    18,    21,     4,     5,     6,     7,
      19,    36,    20,    73,    74,    21,    37,    79,    39,    40,
      41,    42,    88,    23,    38,    55,    56,    84,    87,    11,
      48,    10,    90,    23,    43,    49,    63,    64,    65,    66,
      67,    68,    55,    56,    55,    56,    77,    47,    48,    71,
       0,    83,     0,    49
};

static const yytype_int8 yycheck[] =
{
      37,    36,     0,    38,     3,     4,     5,     6,    43,    22,
      23,     1,    47,     3,     4,     5,     6,     7,    12,     9,
      60,    61,    12,    30,     1,    26,     3,     4,     5,     6,
       7,    30,     9,    28,    69,    12,    71,    20,    21,    29,
      30,    20,    21,    27,    27,    25,    25,    84,    20,    21,
      87,    30,    29,    30,     1,    27,     3,     4,     5,     6,
       7,    24,     9,    12,     1,    12,     3,     4,     5,     6,
       7,    26,     9,    55,    56,    12,    28,     8,    10,    11,
      12,    13,    29,    30,    26,    20,    21,    28,    28,     9,
      25,     2,    29,    30,    26,    30,    14,    15,    16,    17,
      18,    19,    20,    21,    20,    21,    62,    24,    25,    53,
      -1,    27,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    32,    33,     0,     3,     4,     5,     6,    30,    34,
      38,    33,    12,    26,    27,    28,    35,    36,     1,     7,
       9,    12,    29,    30,    37,    38,    39,    42,    43,    45,
      46,    47,    25,    48,    24,    12,    26,    28,    26,    10,
      11,    12,    13,    26,    51,    52,    53,    24,    25,    30,
      40,    51,    36,    51,    51,    20,    21,    25,    30,    41,
      22,    23,    51,    14,    15,    16,    17,    18,    19,    50,
      29,    50,    27,    52,    52,    53,    53,    40,    51,     8,
      51,    27,    44,    27,    28,    49,    36,    28,    29,    36,
      29
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    35,    34,    36,    36,    37,
      37,    37,    37,    37,    37,    38,    38,    38,    38,    39,
      39,    40,    40,    41,    41,    42,    44,    43,    43,    45,
      46,    46,    48,    49,    47,    50,    50,    50,    50,    50,
      50,    51,    51,    51,    52,    52,    52,    53,    53,    53,
      53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     0,     2,     0,     8,     0,     2,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     5,
       3,     1,     1,     1,     1,     4,     0,     9,     4,     1,
       6,     5,     0,     0,    11,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     1,     1,     1,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_CHARACTER: /* CHARACTER  */
#line 213 "parser/parser.y"
            { free(((*yyvaluep).lexeme)); }
#line 1371 "parser/parser.tab.c"
        break;

    case YYSYMBOL_BOOLEAN: /* BOOLEAN  */
#line 213 "parser/parser.y"
            { free(((*yyvaluep).lexeme)); }
#line 1377 "parser/parser.tab.c"
        break;

    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 213 "parser/parser.y"
            { free(((*yyvaluep).str)); }
#line 1383 "parser/parser.tab.c"
        break;

    case YYSYMBOL_NUMBER: /* NUMBER  */
#line 213 "parser/parser.y"
            { free(((*yyvaluep).lexeme)); }
#line 1389 "parser/parser.tab.c"
        break;

    case YYSYMBOL_expression: /* expression  */
#line 214 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1395 "parser/parser.tab.c"
        break;

    case YYSYMBOL_term: /* term  */
#line 214 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1401 "parser/parser.tab.c"
        break;

    case YYSYMBOL_factor: /* factor  */
#line 214 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1407 "parser/parser.tab.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5: /* $@1: %empty  */
#line 247 "parser/parser.y"
    {
        markStatementBoundary();
    }
#line 1682 "parser/parser.tab.c"
    break;

  case 6: /* function_definition: type IDENTIFIER LPAREN RPAREN LBRACE $@1 statements RBRACE  */
#line 251 "parser/parser.y"
    {
        free((yyvsp[-6].str));
    }
#line 1690 "parser/parser.tab.c"
    break;

  case 8: /* statements: statements statement  */
#line 259 "parser/parser.y"
    {
        markStatementBoundary();
    }
#line 1698 "parser/parser.tab.c"
    break;

  case 14: /* statement: error SEMICOLON  */
#line 271 "parser/parser.y"
    {
        syntaxErrorOccurred = true;

        // Discard everything the skipped statement had emitted.
        restoreStatementBoundary();

        printf("Note: skipped the invalid statement ending at line %d "
               "and continued parsing.\n\n", lastTokenLine);

        yyerrok;
    }
#line 1714 "parser/parser.tab.c"
    break;

  case 15: /* type: INT  */
#line 285 "parser/parser.y"
              { (yyval.type) = TYPE_INT; }
#line 1720 "parser/parser.tab.c"
    break;

  case 16: /* type: FLOAT  */
#line 286 "parser/parser.y"
              { (yyval.type) = TYPE_FLOAT; }
#line 1726 "parser/parser.tab.c"
    break;

  case 17: /* type: CHAR  */
#line 287 "parser/parser.y"
              { (yyval.type) = TYPE_CHAR; }
#line 1732 "parser/parser.tab.c"
    break;

  case 18: /* type: BOOL  */
#line 288 "parser/parser.y"
              { (yyval.type) = TYPE_BOOL; }
#line 1738 "parser/parser.tab.c"
    break;

  case 19: /* declaration: type IDENTIFIER ASSIGN expression declaration_end  */
#line 293 "parser/parser.y"
    {
        std::string variableType = typeName((yyvsp[-4].type));

        if (!symbolTable.addSymbol((yyvsp[-3].str), variableType))
        {
            semanticErrorOccurred = true;
            reportError("Semantic Error", "DUPLICATE_DECLARATION", lastTokenLine,
                        "Duplicate declaration of '" + std::string((yyvsp[-3].str)) + "'");
        }
        else if ((yyvsp[-1].expr)->type == TYPE_UNKNOWN)
        {
            // The initializer already reported its error.
            semanticErrorOccurred = true;
        }
        else if (!isAssignable((yyvsp[-4].type), (yyvsp[-1].expr)->type))
        {
            semanticErrorOccurred = true;
            reportTypeMismatch("initializer", (yyvsp[-4].type), (yyvsp[-3].str), (yyvsp[-1].expr)->type, lastTokenLine);
        }
        else
        {
            irGenerator.emit("DECLARE " + variableType + " " + std::string((yyvsp[-3].str)));
            irGenerator.emit("ASSIGN " + std::string((yyvsp[-3].str)) + " " + (yyvsp[-1].expr)->place);
            symbolTable.setInitialized((yyvsp[-3].str), true);
        }

        delete (yyvsp[-1].expr);
        free((yyvsp[-3].str));
    }
#line 1772 "parser/parser.tab.c"
    break;

  case 20: /* declaration: type IDENTIFIER declaration_end  */
#line 324 "parser/parser.y"
    {
        std::string variableType = typeName((yyvsp[-2].type));

        if (!symbolTable.addSymbol((yyvsp[-1].str), variableType))
        {
            semanticErrorOccurred = true;
            reportError("Semantic Error", "DUPLICATE_DECLARATION", lastTokenLine,
                        "Duplicate declaration of '" + std::string((yyvsp[-1].str)) + "'");
        }
        else
        {
            irGenerator.emit("DECLARE " + variableType + " " + std::string((yyvsp[-1].str)));
        }

        free((yyvsp[-1].str));
    }
#line 1793 "parser/parser.tab.c"
    break;

  case 22: /* declaration_end: NEWLINE  */
#line 345 "parser/parser.y"
    {
        syntaxRecoveryOccurred = true;
        reportMissingSemicolon("declaration", lastTokenLine);
    }
#line 1802 "parser/parser.tab.c"
    break;

  case 24: /* assignment_end: NEWLINE  */
#line 354 "parser/parser.y"
    {
        syntaxRecoveryOccurred = true;
        reportMissingSemicolon("assignment", lastTokenLine);
    }
#line 1811 "parser/parser.tab.c"
    break;

  case 25: /* assignment: IDENTIFIER ASSIGN expression assignment_end  */
#line 362 "parser/parser.y"
    {
        if (!symbolTable.contains((yyvsp[-3].str)))
        {
            semanticErrorOccurred = true;
            reportUndeclared(symbolTable, (yyvsp[-3].str), (yyvsp[-1].expr)->type, lastTokenLine);
        }
        else
        {
            int target = typeCode(symbolTable.getType((yyvsp[-3].str)));

            if ((yyvsp[-1].expr)->type == TYPE_UNKNOWN)
            {
                // The right-hand side already reported its error.
                semanticErrorOccurred = true;
            }
            else if (!isAssignable(target, (yyvsp[-1].expr)->type))
            {
                semanticErrorOccurred = true;
                reportTypeMismatch("assignment", target, (yyvsp[-3].str), (yyvsp[-1].expr)->type,
                                   lastTokenLine);
            }
            else
            {
                irGenerator.emit("ASSIGN " + std::string((yyvsp[-3].str)) + " " + (yyvsp[-1].expr)->place);
                symbolTable.setInitialized((yyvsp[-3].str), true);
            }
        }

        delete (yyvsp[-1].expr);
        free((yyvsp[-3].str));
    }
#line 1847 "parser/parser.tab.c"
    break;

  case 26: /* $@2: %empty  */
#line 397 "parser/parser.y"
    {
        irGenerator.emit("GOTO " + ifLabelStack.back().second);
        irGenerator.emit(ifLabelStack.back().first + ":");
        markStatementBoundary();
    }
#line 1857 "parser/parser.tab.c"
    break;

  case 27: /* if_statement: if_header LBRACE statements RBRACE ELSE $@2 LBRACE statements RBRACE  */
#line 403 "parser/parser.y"
    {
        irGenerator.emit(ifLabelStack.back().second + ":");
        ifLabelStack.pop_back();
        finishIfIR();
    }
#line 1867 "parser/parser.tab.c"
    break;

  case 28: /* if_statement: if_header LBRACE statements RBRACE  */
#line 410 "parser/parser.y"
    {
        irGenerator.emit("GOTO " + ifLabelStack.back().second);
        irGenerator.emit(ifLabelStack.back().first + ":");
        irGenerator.emit(ifLabelStack.back().second + ":");

        ifLabelStack.pop_back();
        finishIfIR();
    }
#line 1880 "parser/parser.tab.c"
    break;

  case 29: /* if_start: IF  */
#line 422 "parser/parser.y"
    {
        ifIRCheckpoints.push_back(irGenerator.createCheckpoint());
    }
#line 1888 "parser/parser.tab.c"
    break;

  case 30: /* if_header: if_start LPAREN expression comparison_operator expression RPAREN  */
#line 429 "parser/parser.y"
    {
        beginIf((yyvsp[-3].expr), (yyvsp[-2].op), (yyvsp[-1].expr));
    }
#line 1896 "parser/parser.tab.c"
    break;

  case 31: /* if_header: if_start LPAREN expression comparison_operator expression  */
#line 435 "parser/parser.y"
    {
        syntaxRecoveryOccurred = true;
        reportMissingRParen(if_line);
        beginIf((yyvsp[-2].expr), (yyvsp[-1].op), (yyvsp[0].expr));
    }
#line 1906 "parser/parser.tab.c"
    break;

  case 32: /* $@3: %empty  */
#line 444 "parser/parser.y"
    {
        whileIRCheckpoints.push_back(irGenerator.createCheckpoint());

        std::string startLabel = irGenerator.newLabel();
        irGenerator.emit(startLabel + ":");

        whileLabelStack.push_back({startLabel, ""});
        whileValidityStack.push_back(false);
    }
#line 1920 "parser/parser.tab.c"
    break;

  case 33: /* $@4: %empty  */
#line 455 "parser/parser.y"
    {
        bool valid = checkComparison((yyvsp[-3].expr)->type, (yyvsp[-1].expr)->type, (yyvsp[-2].op),
                                     lastTokenLine, "WHILE");

        whileValidityStack.back() = valid;

        if (valid)
        {
            std::string bodyLabel = irGenerator.newLabel();
            std::string endLabel  = irGenerator.newLabel();

            irGenerator.emit("IF " + (yyvsp[-3].expr)->place + " " + (yyvsp[-2].op) + " " +
                             (yyvsp[-1].expr)->place + " GOTO " + bodyLabel);
            irGenerator.emit("GOTO " + endLabel);
            irGenerator.emit(bodyLabel + ":");

            whileLabelStack.back().second = endLabel;
        }

        delete (yyvsp[-3].expr);
        delete (yyvsp[-1].expr);

        markStatementBoundary();
    }
#line 1949 "parser/parser.tab.c"
    break;

  case 34: /* while_statement: WHILE $@3 LPAREN expression comparison_operator expression RPAREN $@4 LBRACE statements RBRACE  */
#line 481 "parser/parser.y"
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
#line 1970 "parser/parser.tab.c"
    break;

  case 35: /* comparison_operator: LT  */
#line 500 "parser/parser.y"
            { (yyval.op) = "<"; }
#line 1976 "parser/parser.tab.c"
    break;

  case 36: /* comparison_operator: GT  */
#line 501 "parser/parser.y"
            { (yyval.op) = ">"; }
#line 1982 "parser/parser.tab.c"
    break;

  case 37: /* comparison_operator: LE  */
#line 502 "parser/parser.y"
            { (yyval.op) = "<="; }
#line 1988 "parser/parser.tab.c"
    break;

  case 38: /* comparison_operator: GE  */
#line 503 "parser/parser.y"
            { (yyval.op) = ">="; }
#line 1994 "parser/parser.tab.c"
    break;

  case 39: /* comparison_operator: EQ  */
#line 504 "parser/parser.y"
            { (yyval.op) = "=="; }
#line 2000 "parser/parser.tab.c"
    break;

  case 40: /* comparison_operator: NE  */
#line 505 "parser/parser.y"
            { (yyval.op) = "!="; }
#line 2006 "parser/parser.tab.c"
    break;

  case 41: /* expression: expression PLUS term  */
#line 510 "parser/parser.y"
    {
        (yyval.expr) = makeBinary("ADD", "Addition", (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 2014 "parser/parser.tab.c"
    break;

  case 42: /* expression: expression MINUS term  */
#line 514 "parser/parser.y"
    {
        (yyval.expr) = makeBinary("SUB", "Subtraction", (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 2022 "parser/parser.tab.c"
    break;

  case 43: /* expression: term  */
#line 518 "parser/parser.y"
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2030 "parser/parser.tab.c"
    break;

  case 44: /* term: term MULT factor  */
#line 525 "parser/parser.y"
    {
        (yyval.expr) = makeBinary("MUL", "Multiplication", (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 2038 "parser/parser.tab.c"
    break;

  case 45: /* term: term DIV factor  */
#line 529 "parser/parser.y"
    {
        (yyval.expr) = makeBinary("DIV", "Division", (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 2046 "parser/parser.tab.c"
    break;

  case 46: /* term: factor  */
#line 533 "parser/parser.y"
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 2054 "parser/parser.tab.c"
    break;

  case 47: /* factor: NUMBER  */
#line 540 "parser/parser.y"
    {
        ExprValue *result = new ExprValue;
        result->type  = (strchr((yyvsp[0].lexeme), '.') != NULL) ? TYPE_FLOAT : TYPE_INT;
        result->place = (yyvsp[0].lexeme);
        (yyval.expr) = result;
        free((yyvsp[0].lexeme));
    }
#line 2066 "parser/parser.tab.c"
    break;

  case 48: /* factor: CHARACTER  */
#line 548 "parser/parser.y"
    {
        ExprValue *result = new ExprValue;
        result->type  = TYPE_CHAR;
        result->place = (yyvsp[0].lexeme);
        (yyval.expr) = result;
        free((yyvsp[0].lexeme));
    }
#line 2078 "parser/parser.tab.c"
    break;

  case 49: /* factor: BOOLEAN  */
#line 556 "parser/parser.y"
    {
        ExprValue *result = new ExprValue;
        result->type  = TYPE_BOOL;
        result->place = (yyvsp[0].lexeme);
        (yyval.expr) = result;
        free((yyvsp[0].lexeme));
    }
#line 2090 "parser/parser.tab.c"
    break;

  case 50: /* factor: IDENTIFIER  */
#line 564 "parser/parser.y"
    {
        ExprValue *result = new ExprValue;
        result->place = (yyvsp[0].str);

        if (symbolTable.contains((yyvsp[0].str)))
        {
            result->type = typeCode(symbolTable.getType((yyvsp[0].str)));

            if (!symbolTable.isInitialized((yyvsp[0].str)))
            {
                reportWarning(lastTokenLine,
                    "Variable '" + std::string((yyvsp[0].str)) +
                        "' may be used before initialization.",
                    "Assign a value to '" + std::string((yyvsp[0].str)) +
                        "' before using it.");
            }
        }
        else
        {
            semanticErrorOccurred = true;
            reportUndeclared(symbolTable, (yyvsp[0].str), TYPE_UNKNOWN, lastTokenLine);
            result->type = TYPE_UNKNOWN;
        }

        (yyval.expr) = result;
        free((yyvsp[0].str));
    }
#line 2122 "parser/parser.tab.c"
    break;

  case 51: /* factor: LPAREN expression RPAREN  */
#line 592 "parser/parser.y"
    {
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 2130 "parser/parser.tab.c"
    break;


#line 2134 "parser/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 597 "parser/parser.y"


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
