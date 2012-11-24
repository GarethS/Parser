/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 14 "valve.y"

/* #define YYDEBUG 1 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
//#include <assert.h>
#include "compiler.h"

// Variable, constant and function name symbol table
symbolNode symbolTable[VAR_ITEMS];
// When the function name gets parsed (the last thing to be done after parsing
//  the statements), this is the index used to hold the function name. When
//  this happens, a space will be reserved at the current 'symbolTableFreeIndex'
//  to hold the next function name. 
//  In addition, all the variables in the scope of the current function will be
//  inspected, and if they are the same name as a function parameter, they will
//  be marked as stack based variables. Their entry in the symbol table will,
//  likewise, be noted.
unsigned int symbolTableLastFunctionIndex = 0;
unsigned int symbolTableFreeIndex = 1;
unsigned int functionParameterIndex = 0; // Index of argument on execution stack so interpreter knows how to access this variable. 

// Abstract syntax tree table
astNode syntaxTable[SYNTAX_ITEMS];
unsigned int syntaxTableFreeIndex = 0;

astNode statementTable[STATEMENT_ITEMS];
unsigned int statementTableFreeIndex = 0;



/* Line 189 of yacc.c  */
#line 106 "valve.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INPUTS = 258,
     OUTPUTS = 259,
     COMMA = 260,
     BANG = 261,
     EQUAL = 262,
     LBRACE = 263,
     RBRACE = 264,
     ARRAYDEFINE = 265,
     IF = 266,
     ELSE = 267,
     WHILE = 268,
     VAR = 269,
     VAR_METHOD = 270,
     CONST = 271,
     CONST_FLOAT = 272,
     MAIN = 273,
     BITWISEOR = 274,
     BITWISEAND = 275,
     OR = 276,
     AND = 277,
     LSS = 278,
     GTR = 279,
     NEQ = 280,
     LEQ = 281,
     GEQ = 282,
     TEST_FOR_EQUAL = 283,
     LPAREN = 284,
     MINUS = 285,
     PLUS = 286,
     XOR = 287,
     DIV = 288,
     MULT = 289,
     LBRACKET = 290,
     RBRACKET = 291,
     RPAREN = 292,
     SEMI = 293
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 48 "valve.y"

	int integer;
    float floatingPoint;
	char* string;
	astNode* pSyntaxNode;



/* Line 214 of yacc.c  */
#line 189 "valve.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 201 "valve.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   244

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  49
/* YYNRULES -- Number of states.  */
#define YYNSTATES  117

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     7,    10,    18,    26,    27,    30,
      32,    34,    36,    42,    44,    52,    60,    72,    77,    85,
      89,    93,    97,   101,   105,   109,   113,   117,   121,   125,
     129,   133,   137,   141,   145,   149,   151,   153,   158,   163,
     164,   167,   168,   172,   173,   176,   180,   181,   185,   190
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    42,    41,    -1,    -1,    41,    43,    -1,
      18,    29,    52,    37,     8,    44,     9,    -1,    14,    29,
      52,    37,     8,    44,     9,    -1,    -1,    44,    45,    -1,
      48,    -1,    47,    -1,    46,    -1,    14,    29,    50,    37,
      38,    -1,    54,    -1,    13,    29,    49,    37,     8,    44,
       9,    -1,    11,    29,    49,    37,     8,    44,     9,    -1,
      11,    29,    49,    37,     8,    44,     9,    12,     8,    44,
       9,    -1,    14,     7,    49,    38,    -1,    14,    35,    49,
      36,     7,    49,    38,    -1,    29,    49,    37,    -1,    49,
      31,    49,    -1,    49,    30,    49,    -1,    49,    34,    49,
      -1,    49,    33,    49,    -1,    49,    32,    49,    -1,    49,
      22,    49,    -1,    49,    21,    49,    -1,    49,    20,    49,
      -1,    49,    19,    49,    -1,    49,    28,    49,    -1,    49,
      25,    49,    -1,    49,    27,    49,    -1,    49,    26,    49,
      -1,    49,    24,    49,    -1,    49,    23,    49,    -1,    14,
      -1,    16,    -1,    14,    29,    50,    37,    -1,    14,    35,
      49,    36,    -1,    -1,    49,    51,    -1,    -1,    51,     5,
      49,    -1,    -1,    14,    53,    -1,    20,    14,    53,    -1,
      -1,    53,     5,    14,    -1,    53,     5,    20,    14,    -1,
      10,    14,    35,    16,    36,    38,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    84,    84,    86,    87,    89,    91,    93,    94,    96,
      97,    98,    99,   100,   102,   104,   108,   110,   112,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   135,
     136,   138,   139,   141,   142,   143,   145,   146,   147,   149
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INPUTS", "OUTPUTS", "COMMA", "BANG",
  "EQUAL", "LBRACE", "RBRACE", "ARRAYDEFINE", "IF", "ELSE", "WHILE", "VAR",
  "VAR_METHOD", "CONST", "CONST_FLOAT", "MAIN", "BITWISEOR", "BITWISEAND",
  "OR", "AND", "LSS", "GTR", "NEQ", "LEQ", "GEQ", "TEST_FOR_EQUAL",
  "LPAREN", "MINUS", "PLUS", "XOR", "DIV", "MULT", "LBRACKET", "RBRACKET",
  "RPAREN", "SEMI", "$accept", "program", "functionDefnList",
  "functionDefnMain", "functionDefn", "statementList", "statement",
  "statementWhile", "statementIf", "statementAssign", "expr", "argList",
  "commaArgList", "defnArgList", "defnCommaArgList", "arrayDefine", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    43,    44,    44,    45,
      45,    45,    45,    45,    46,    47,    47,    48,    48,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      50,    51,    51,    52,    52,    52,    53,    53,    53,    54
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     2,     7,     7,     0,     2,     1,
       1,     1,     5,     1,     7,     7,    11,     4,     7,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     4,     4,     0,
       2,     0,     3,     0,     2,     3,     0,     3,     4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     3,    43,     1,     2,    46,     0,     0,
       0,     4,    44,    46,     0,    43,     0,    45,     7,     0,
      47,     0,     0,     0,    48,     5,     0,     0,     0,     0,
       8,    11,    10,     9,    13,     7,     0,     0,     0,     0,
      39,     0,     0,     0,    35,    36,     0,     0,     0,     0,
      41,     0,     0,     6,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,    40,     0,     0,     0,
       0,     0,    19,    28,    27,    26,    25,    34,    33,    30,
      32,    31,    29,    21,    20,    24,    23,    22,     7,     7,
       0,    12,     0,    49,    37,    38,     0,     0,    42,     0,
      15,    14,    18,     0,     7,     0,    16
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     6,     3,    11,    22,    30,    31,    32,    33,
      50,    51,    76,     9,    12,    34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -36
static const yytype_int16 yypact[] =
{
      -7,   -15,    18,   -36,    -5,   -36,     6,   -36,     9,     7,
      14,   -36,    51,   -36,    54,    -5,    -4,    51,   -36,    30,
     -36,    71,   201,    78,   -36,   -36,    73,    76,    77,    10,
     -36,   -36,   -36,   -36,   -36,   -36,    64,    -8,    -8,    -8,
      -8,    -8,   207,   102,   -16,   -36,    -8,    70,    89,    27,
     163,    87,   127,   -36,   101,    -8,    -8,   108,    -8,    -8,
      -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,
      -8,    -8,    -8,   117,   135,   -36,   139,   118,   155,   136,
     143,   145,   -36,   175,   175,   175,   175,   210,   210,   210,
     210,   210,   210,     8,     8,   -32,   -36,   -36,   -36,   -36,
      -8,   -36,    -8,   -36,   -36,   -36,   213,   219,   163,    50,
     180,   -36,   -36,   196,   -36,   225,   -36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -36,   -36,   -36,   -36,   -36,   -35,   -36,   -36,   -36,   -36,
     -34,   158,   -36,   204,   212,   -36
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      42,    71,    72,    47,    48,    49,    44,    52,    45,     7,
      20,     1,    57,    55,     4,     8,    21,    39,     5,    56,
      10,    46,    81,    13,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    40,
      70,    71,    72,    15,    14,    41,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    16,    68,    69,    70,
      71,    72,    18,   106,   107,    75,   108,    23,   109,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,   115,
      68,    69,    70,    71,    72,    24,    35,    36,   112,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    43,
      68,    69,    70,    71,    72,    37,    38,    73,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    54,    68,
      69,    70,    71,    72,    77,    98,    74,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    79,    68,    69,
      70,    71,    72,    99,   100,    82,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,   101,    68,    69,    70,
      71,    72,   102,    78,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,   103,    68,    69,    70,    71,    72,
     104,   105,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,   113,    68,    69,    70,    71,    72,    62,    63,
      64,    65,    66,    67,   114,    68,    69,    70,    71,    72,
      25,    26,    27,    80,    28,    29,    53,    26,    27,    19,
      28,    29,   110,    26,    27,    17,    28,    29,   111,    26,
      27,     0,    28,    29,   116,    26,    27,     0,    28,    29,
      68,    69,    70,    71,    72
};

static const yytype_int8 yycheck[] =
{
      35,    33,    34,    37,    38,    39,    14,    41,    16,    14,
      14,    18,    46,    29,    29,    20,    20,     7,     0,    35,
      14,    29,    56,    14,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    29,
      32,    33,    34,    29,    37,    35,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     5,    30,    31,    32,
      33,    34,     8,    98,    99,    38,   100,    37,   102,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,   114,
      30,    31,    32,    33,    34,    14,     8,    14,    38,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    35,
      30,    31,    32,    33,    34,    29,    29,    37,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    16,    30,
      31,    32,    33,    34,    37,     8,    37,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    36,    30,    31,
      32,    33,    34,     8,     5,    37,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    38,    30,    31,    32,
      33,    34,     7,    36,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    38,    30,    31,    32,    33,    34,
      37,    36,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    12,    30,    31,    32,    33,    34,    23,    24,
      25,    26,    27,    28,     8,    30,    31,    32,    33,    34,
       9,    10,    11,    55,    13,    14,     9,    10,    11,    15,
      13,    14,     9,    10,    11,    13,    13,    14,     9,    10,
      11,    -1,    13,    14,     9,    10,    11,    -1,    13,    14,
      30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    18,    40,    42,    29,     0,    41,    14,    20,    52,
      14,    43,    53,    14,    37,    29,     5,    53,     8,    52,
      14,    20,    44,    37,    14,     9,    10,    11,    13,    14,
      45,    46,    47,    48,    54,     8,    14,    29,    29,     7,
      29,    35,    44,    35,    14,    16,    29,    49,    49,    49,
      49,    50,    49,     9,    16,    29,    35,    49,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    30,    31,
      32,    33,    34,    37,    37,    38,    51,    37,    36,    36,
      50,    49,    37,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,     8,     8,
       5,    38,     7,    38,    37,    36,    44,    44,    49,    49,
       9,     9,    38,    12,     8,    44,     9
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 84 "valve.y"
    {dumpSymbolTable("symbolTable.txt");;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 86 "valve.y"
    {;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 87 "valve.y"
    {;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 89 "valve.y"
    {addFunction("main", (yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 91 "valve.y"
    {addFunction((yyvsp[(1) - (7)].string), (yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 93 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 94 "valve.y"
    {/*printf("\nstatementList:%d, statement:%d", (int)$1, (int)$2);*/ (yyval.pSyntaxNode) = addStatement((yyvsp[(1) - (2)].pSyntaxNode), (yyvsp[(2) - (2)].pSyntaxNode)); /*printf(" newStatementList:%d", (int)$$);*/;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 96 "valve.y"
    {/*printf("\nstatementAssign:%d", (int)$1); walkSyntaxTree($1, "start", 0);*/ (yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 97 "valve.y"
    {/*printf("\nstatementIf:%d", (int)$1);*/ (yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 98 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 99 "valve.y"
    {(yyval.pSyntaxNode) = addNodeFunctionCall((yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].pSyntaxNode));;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 100 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 102 "valve.y"
    {(yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode), NULL, nodeWhile);;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 104 "valve.y"
    {/*printf("statementIf:statementList:%d", (int)$6);*/ (yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode), NULL, nodeIf);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 108 "valve.y"
    {/*printf("statementIf:statementList:%d", (int)$6);*/ (yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (11)].pSyntaxNode), (yyvsp[(6) - (11)].pSyntaxNode), (yyvsp[(10) - (11)].pSyntaxNode), nodeIf);;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 110 "valve.y"
    {(yyval.pSyntaxNode) = addNodeVariableOperator(EQUAL, addVarToSymbolTable((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 112 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(EQUAL, addNodeArray((yyvsp[(1) - (7)].string), (yyvsp[(3) - (7)].pSyntaxNode)), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 114 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(2) - (3)].pSyntaxNode);;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 115 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(PLUS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 116 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(MINUS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 117 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(MULT, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 118 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(DIV, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 119 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(XOR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 120 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(AND, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 121 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(OR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 122 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(BITWISEAND, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 123 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(BITWISEOR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 124 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(TEST_FOR_EQUAL, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 125 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(NEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 126 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(GEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 127 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(LEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 128 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(GTR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 129 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(LSS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 130 "valve.y"
    {(yyval.pSyntaxNode) = addNodeSymbolIndex(addVarToSymbolTable((yyvsp[(1) - (1)].string)));;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 131 "valve.y"
    {(yyval.pSyntaxNode) = addNodeSymbolIndex(addVarToSymbolTable((yyvsp[(1) - (1)].string)));;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 132 "valve.y"
    {(yyval.pSyntaxNode) = addNodeFunctionCall((yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 133 "valve.y"
    {(yyval.pSyntaxNode) = addNodeArray((yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 135 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 136 "valve.y"
    {(yyval.pSyntaxNode) = addFcnCallArgument((yyvsp[(2) - (2)].pSyntaxNode), (yyvsp[(1) - (2)].pSyntaxNode));;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 138 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 139 "valve.y"
    {(yyval.pSyntaxNode) = addFcnCallArgument((yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 141 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 142 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(2) - (2)].pSyntaxNode), (yyvsp[(1) - (2)].string), VAR_PASS_BY_VALUE);;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 143 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(3) - (3)].pSyntaxNode), (yyvsp[(2) - (3)].string), VAR_PASS_BY_REFERENCE);;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 145 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 146 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].string), VAR_PASS_BY_VALUE);;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 147 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(1) - (4)].pSyntaxNode), (yyvsp[(4) - (4)].string), VAR_PASS_BY_REFERENCE);;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 149 "valve.y"
    {(yyval.integer) = addArrayToSymbolTable((yyvsp[(2) - (6)].string), atoi((yyvsp[(4) - (6)].string)));;}
    break;



/* Line 1464 of yacc.c  */
#line 1855 "valve.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 151 "valve.y"
 /* Additional C code */


#include "lex.yy.c"

void yyerror (char* s)
{
	printf ("Line: %d: %s before '%s'\n", --yylineno, s, yytext);
}

int main ()
{
#if 0
	/* Initialize IO boilerplate code. */
	FILE* stream = fopen("\\dev\\parser\\port1\\boilerplate.c", "r" );
	if (stream == NULL) {
		printf("Error opening boilerplate code.\n");
	} else {
		#define BUF_LEN (256)
		char buffer[BUF_LEN];
		printf("/* This code automatically generated by ZPORT 1.0. */\n", buffer);
		while (fgets(buffer, BUF_LEN, stream) != NULL) {
			printf("%s", buffer);
		}
		fclose(stream);
	}
#endif

	initVarTable();
	// To turn on debugging, make sure the next line is uncommented and
	//  turn on the -t (also use -v -l) options in bison.exe.
	yydebug = 1; 
    yyin = fopen("valve4.def", "r" );
	yyparse ();
    return 0;
}

void initVarTable(void) {
	int len;
	for (len = VAR_ITEMS - 1; len >= 0; len--) {
		symbolTable[len].name[0] = EOS;
		symbolTable[len].val = DEFAULT_VAR_VALUE;	// Initialize variable to 0
	}
}

// Return index of variable/constant in symbol table
int insertVariable(symbolNode* pVar) {
	if (symbolTableFreeIndex < VAR_ITEMS) {
		symbolTable[symbolTableFreeIndex] = *pVar;
		return symbolTableFreeIndex++;
	}
	return VAR_TABLE_LIMIT;
}

// A little danger here. Forces symbol to be placed at a specific index.
// Used to insert function name at start of symbol list for that function.
void insertVariableAtIndex(symbolNode* pVar, const unsigned int index) {
    symbolTable[index] = *pVar;
}

int insertVariableNew(astNode* pSymbolTable, symbolNode* pSymbol) {
    if (pSymbolTable == NULL) {
        debugAssert(ERR:insertVariableNew():pSymbolTable == NULL);
        return VAR_TABLE_INVALID;
    }
    if (pSymbolTable->pLeft == NULL) {
        debugAssert(ERR:insertVariableNew():pSymbolTable->pLeft == NULL);
        return VAR_TABLE_INVALID;
    }
	if (symbolTableFreeIndex < VAR_ITEMS) {
		symbolTable[symbolTableFreeIndex] = *pSymbol;
		//return symbolTableFreeIndex++;
	} else {
        debugAssert(ERR:FATAL:insertVariableNew():symbolTableFreeIndex >= VAR_ITEMS);
        return VAR_TABLE_LIMIT;
    }
    astNode* p              = getNextASTNode();
    p->pSymbolNode          = symbolTable + symbolTableFreeIndex++;  // Just the pointer into the variable table
    p->pNext                = pSymbolTable->pLeft;  // Insert at top of list
    pSymbolTable->pLeft     = p;
	return symbolTableFreeIndex;
}

// Return index of variable (includes function names), or VAR_NOT_FOUND if not found.
int findVariableByName(const char* pVarName) {
    symbolNode thisNode;
    buildVariable(pVarName, 0, &thisNode);
    return findVariable(&thisNode);
}

int findVariableByNameNew(astNode* pSymbolTable, const char* pVarName) {
    symbolNode thisNode;
    buildVariable(pVarName, 0, &thisNode);
    return findVariableNew(pSymbolTable, &thisNode);
}

// Return index where variable is located in symbolTable, or VAR_NOT_FOUND if not found.
// TODO: nodeConst can be anywhere in symbol table since its value never varies.
int findVariable(symbolNode* pVar) {
	int i;
    // Only search within the scope of the function currently being parsed.
	for (i = symbolTableLastFunctionIndex; i < symbolTableFreeIndex; ++i) {
		if (strncmp(symbolTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
			return i;
		}
	}
	return VAR_NOT_FOUND;
}

int findVariableNew(astNode* pVarTable, symbolNode* pVar) {
    if (pVarTable == NULL) {
        debugAssert(ERR:findVariableNew():pVarTable == NULL);
        return VAR_NOT_FOUND;
    }
    astNode* pSymbol = pVarTable->pLeft;
	for (; pSymbol != NULL; pSymbol = pSymbol->pNext) {
        if (pSymbol->pSymbolNode == NULL) {
            debugAssert(ERR:findVariableNew():pSymbol->pSymbolNode == NULL);
            return VAR_NOT_FOUND;
        } else {
            //symbolNode* pVarNode = pSymbol->pSymbolNode;
            //symbolNode* symbolTable = pSymbol->pSymbolNode;
            if (strncmp(pSymbol->pSymbolNode->name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
                return getSymbolTableIndex(pSymbol->pSymbolNode);
                //return pSymbol;
            }
        }
	}
	return VAR_NOT_FOUND;
}

// e.g. if (x==2) {x = 1;} else {x = 4;}
// Note that both pIfStatementList and pElseStatementList can both be NULL. This can happen if they have empty statement lists.
astNode* addNodeIfOrWhile(astNode* pExpr, astNode* pIfOrWhileStatementList, astNode* pElseStatementList, nodeType type) {
    static int uniqueID = 0;

	astNode* p = getNextASTNode();
    if (pExpr == NULL) {
        debugAssert(ERR:addNodeIfOrWhile():pExpr == NULL);
		return NULL;
    }
    p->type     = type;
    p->value    = uniqueID++;
    p->pLeft    = pExpr;
    p->pCentre  = pIfOrWhileStatementList;
    p->pRight   = pElseStatementList;
    return p;
}

// e.g. '4 * c1'
astNode* addNodeBinaryOperator(int operator, astNode* pLeft, astNode* pRight) {
	astNode* p  = getNextASTNode();
	p->type     = nodeOperator;
	p->value    = operator;
	p->pLeft    = pLeft;
	p->pRight   = pRight;
	return p;	
}

// e.g. 'c3 = 4 * c1;' where 'operator' is '='
astNode* addNodeVariableOperator(int operator, int varIndex, astNode* pRight) {
	astNode* pLeft  = getNextASTNode();
	pLeft->type     = nodeVariable;
	pLeft->value    = varIndex;
    return addNodeBinaryOperator(operator, pLeft, pRight);
}

// Add either a function call (pStatmentList = NULL) or a function definition (pStatmentList != NULL)
astNode* addFunction(const char* pFuncName, astNode* pArgList, astNode* pStatementList) {
    // Essentially the same as addNodeFunctionCall() except this is a definition, not a call.
    // 0. Count number of arguments and assign that value to its syntax node
    // int argCount = countArguments(pArgList);
    // 1. See if pFuncName is already in the symbol list. It could be there from
    //     another function call or the function definition.
    // Make symbol table entry and add to symbol table
#if 0    
    int symbolIndex = findVariableByName(pFuncName);
    if (symbolIndex == VAR_NOT_FOUND) {
        symbolNode funcNode;
        buildVariable(pFuncName, argCount, &funcNode);
        funcNode.type = nodeFunctionCall;
        int symbolIndex = insertVariable(&funcNode);
        if (symbolIndex == VAR_TABLE_LIMIT) {
            return NULL;
        }
    } else {
        // Make sure value = argCount and type = nodeFunctionCall
        if (symbolTable[symbolIndex].val != argCount) {
            debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].val != argCount);
            return NULL;
        }
        if (symbolTable[symbolIndex].type != nodeFunctionCall) {
            debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].type != nodeFunctionCall);
            return NULL;
        }
    }
#endif    
    
    // 3. Walk statement list to see if there's already a call to this function.
    //     If so, check that the argument count matches
    // No need to do this. We checked the symbol table already and every function call or definition
    //  will do the same.
    // walkList(statementTable);   // Still needs modified to search for functions by this name
    
    if (pStatementList == NULL) {
        // This is a function call
        astNode* p = getNextASTNode();
        p->type = nodeFunctionCall;
        //p->value = symbolIndex;
        p->pNext = pArgList;
        return p;
    } else {
        // This is a function definition not a function call
        if (pFuncName == NULL) {
            debugAssert(ERR:addNodeFunction():pFuncName == NULL);
        } else {
            printf("\nFunction: %s", pFuncName);
        }
        // TODO: Go through pStatementList, fixing up references to arguments so they get
        //  picked off the evaluation stack, rather than the symbol table.
        // Add function name to reserved space in symbol table.
        functionParameterIndex = 0; // Reset for every new function definition
        symbolNode functionNode;
        buildVariable(pFuncName, 0, &functionNode);
        functionNode.type = nodeFunctionDefinition;
        insertVariableAtIndex(&functionNode, symbolTableLastFunctionIndex);
        symbolTableLastFunctionIndex = symbolTableFreeIndex++;
        
        FILE* fp = NULL;
        if (strcmp(pFuncName, "main") == 0) {
            // Clears the file so it doesn't keep getting bigger each time this program is run.
            fp = fopen("tree.txt", "wb");
        } else {
            fp = fopen("tree.txt", "ab");
        }
        fwrite("0 0 Start 0\n", 1, 12, fp);
        /*printf("\nstatementList=%d", (int)$1);*/
        walkList(pArgList, fp);
        walkList(pStatementList, fp);
        fclose(fp);
    }
    return NULL;
}

astNode* addNodeFunctionCall(char* pFuncName, astNode* pArgList) {
    return addFunction(pFuncName, pArgList, NULL);
}

astNode* addNodeArray(char* pVarName, astNode* pASTNode) {
    // 1. Make new astNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pASTNode->value, pASTNode->pLeft->value, pASTNode->pRight->value);
    //printf("xxx root=%d\n", pASTNode->value);
	astNode* pArrayVar = getNextASTNode();
	pArrayVar->type = nodeArray;
    symbolNode pArrayNode;
    buildVariable(pVarName, 0, &pArrayNode);
	pArrayVar->value = findVariable(&pArrayNode);
    if (pArrayVar->value == VAR_NOT_FOUND) {
        debugAssert(ERR: addNodeArray():VAR_NOT_FOUND);
    }
#if 1
    // This fails
    return addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
#else    
    // This passes, but looks like it's using the wrong index in the array
    //return addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
    astNode* pan = addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
    //printf("yyyroot=%d, left=%d, right=%d", pan->value, pan->pLeft->value, pan->pRight->value);
    return pan;
#endif    
}

astNode* addNodeArrayNew(astNode* pSymbolTable, char* pVarName, astNode* pASTNode) {
    // 1. Make new astNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pASTNode->value, pASTNode->pLeft->value, pASTNode->pRight->value);
    //printf("xxx root=%d\n", pASTNode->value);
	astNode* pArrayVar = getNextASTNode();
	pArrayVar->type = nodeArray;
    symbolNode pArrayNode;
    buildVariable(pVarName, 0, &pArrayNode);
    pArrayVar->value = findVariableNew(pSymbolTable, &pArrayNode);
    if (pArrayVar->value == VAR_NOT_FOUND) {
        debugAssert(ERR: addNodeArrayNew():VAR_NOT_FOUND);
        //return NULL;
    }
    return addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
}

astNode* addNodeSymbolIndex(int varIndex) {
	astNode* p  = getNextASTNode();
	p->type     = nodeVariable;
	p->value    = varIndex;
	return p;	
}

astNode* addNodeSymbolIndexNew(astNode* pVar) {
	astNode* p  = getNextASTNode();
	p->type     = nodeVariable;
	p->value    = getSymbolTableIndex(pVar->pSymbolNode);
	return p;	
}

void initNode(astNode* pSyntaxNode) {
    if (pSyntaxNode == NULL) {
        debugAssert(ERR:initNode():pSyntaxNode == NULL);
        return;
    }
    pSyntaxNode->type           = nodeInvalid;
    pSyntaxNode->value          = -1;
    pSyntaxNode->pLeft          = NULL;
    pSyntaxNode->pRight         = NULL;
    pSyntaxNode->pCentre        = NULL;
    pSyntaxNode->pNext          = NULL;
    pSyntaxNode->pSymbolNode    = NULL;
}

astNode* getNextASTNode(void) {
	if (syntaxTableFreeIndex < SYNTAX_ITEMS) {
        // Initialize all members before making this node available
        initNode(syntaxTable + syntaxTableFreeIndex);
		return syntaxTable + syntaxTableFreeIndex++;
	}
    // Cannot survive this condition.
    debugAssert(FATAL:ERR:getNextASTNode():syntaxTableFreeIndex >= SYNTAX_ITEMS);
	return NULL;
}

astNode* getNextStatementNode(void) {
	if (statementTableFreeIndex < STATEMENT_ITEMS) {
#if REGRESS_1    
        printf("getNextStatementNode() %d\n", statementTableFreeIndex);
        fflush(stdout);
#endif /* REGRESS_1 */
        initNode(statementTable + statementTableFreeIndex);
		return statementTable + statementTableFreeIndex++;
	}
    debugAssert(ERR:FATAL:getNextStatementNode():);
	return NULL;
}

// A variable may not start with a number. One that does is consider a constant.
int isConstant(symbolNode* pVar) {
	if (isdigit((int)pVar->name[0])) {
		return TRUE;
	}
	return FALSE;
}

void buildVariable(const char* name, int value, symbolNode* varNode) {
    if (name == NULL || varNode == NULL) {
        debugAssert(ERR:buildVariable());
        return;
    }
	strncpy(varNode->name, name, VAR_NAME_LENGTH-1);
	varNode->name[VAR_NAME_LENGTH-1] = EOS;
	varNode->val = value;
    varNode->type = nodeVariable;
	if (isConstant(varNode)) {
		// Assume it's a constant, but can just treat it like a variable, making sure that
		//  any variable that starts with a number (i.e. a constant) is never altered.
		varNode->val = atoi(name);
        varNode->type = nodeConst;
	}
}

// The first entry in the array definition contains the max range. The next entry
//  is the contents of a[0], then a[1] ...
// N.B. If maxRange is 2, then maxIndex is 1.
int addArrayToSymbolTable(char* var, const unsigned int maxRange) {
    //printf("addArrayToSymbolTable: %s, %d", var, maxRange);
    symbolNode tmp;
    buildVariable(var, maxRange, &tmp);
    int found = findVariable(&tmp);
	if (found == VAR_NOT_FOUND) {
        if (insertVariable(&tmp) == VAR_TABLE_LIMIT) {
            return VAR_TABLE_LIMIT;
        }
        if (symbolTableFreeIndex + maxRange >= VAR_ITEMS) {
            symbolTableFreeIndex--;    // Remove variable just inserted with insertVariable().
            return VAR_TABLE_LIMIT;
        }
        symbolTableFreeIndex += maxRange;
    }
    return found;
}

int addArrayToSymbolTableNew(astNode* pVarTable, char* var, const unsigned int maxRange) {
    //printf("addArrayToSymbolTable: %s, %d", var, maxRange);
    symbolNode tmp;
    buildVariable(var, maxRange, &tmp);
    int found = findVariableNew(pVarTable, &tmp);
	if (found == VAR_NOT_FOUND) {
        if (insertVariable(&tmp) == VAR_TABLE_LIMIT) {
            return VAR_TABLE_LIMIT;
        }
        if (symbolTableFreeIndex + maxRange >= VAR_ITEMS) {
            symbolTableFreeIndex--;    // Remove variable just inserted with insertVariable().
            return VAR_TABLE_LIMIT;
        }
        symbolTableFreeIndex += maxRange;
    }
    return found;
}

// Return index in symbol table
int addVarToSymbolTable(char* var) {
	symbolNode tmp;
    buildVariable(var, DEFAULT_VAR_VALUE, &tmp);
	int found = findVariable(&tmp);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(&tmp);
	}
	return found;
}

int addVarToSymbolTableNew(astNode* pVarTable, char* var) {
	symbolNode tmpSymbolNode;
    buildVariable(var, DEFAULT_VAR_VALUE, &tmpSymbolNode);
	int found = findVariableNew(pVarTable, &tmpSymbolNode);
	if (found == VAR_NOT_FOUND) {
		return insertVariableNew(pVarTable, &tmpSymbolNode);
	}
	return found;
}

astNode* addFcnDefnArgument(astNode* pArgumentListNode, const char* pArgumentName, const int passByValueOrReference) {
    if (pArgumentName == NULL) {
        debugAssert(ERR:addFcnDefnArgument():pArgumentName == NULL);
        return NULL;
    }
    //printf("\naddFcnDefnArgument():pArgumentName=%s", pArgumentName);
    // Add symbols to symbol table, marking them as function parameters.
    // Note that the variable value refers to its index on the execution stack.
    symbolNode argumentNode;
    buildVariable(pArgumentName, functionParameterIndex++, &argumentNode);
    argumentNode.type = nodeArgumentValue;
    if (passByValueOrReference == VAR_PASS_BY_REFERENCE) {
        argumentNode.type = nodeArgumentReference;
    }
    insertVariable(&argumentNode);
    return NULL;
    
	astNode* p = getNextASTNode();
    p->type = passByValueOrReference;
    // Create variable in symbol table
    symbolNode tmp;
    buildVariable(pArgumentName, DEFAULT_VAR_VALUE, &tmp);
    tmp.type = passByValueOrReference;
    //p->Left = 
    p->pNext = pArgumentListNode;
    return NULL;
}

astNode* addFcnCallArgument(astNode* pArgumentListNode, astNode* pArgumentNode) {
    if (pArgumentNode == NULL) {
        // There are no arguments! Just return current argument list.
        return pArgumentListNode;
    }
	astNode* pNewArgumentNode = getNextASTNode();
    if (pArgumentNode->type == nodeVariable) {
        // Duplicate names should be handled already. No need to do anything here I think.
    }
    pNewArgumentNode->type = nodeArgumentCall;
	pNewArgumentNode->pLeft = pArgumentNode;
    pNewArgumentNode->pNext = pArgumentListNode;
	return pNewArgumentNode;
}

astNode* addStatement(astNode* pStatementListNode, astNode* pStatementNode) {
#if REGRESS_1    
    printf("!!addStatement(astNode* pStatementListNode=%d, astNode* pStatementNode=%d)\n", pStatementListNode, pStatementNode);
    printf("pStatementListNode->Next=%d\n", pStatementListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    if (pStatementNode == NULL) {
        // Special case for array definition
        return pStatementListNode;
    }
	astNode* p          = getNextStatementNode();
    p->type             = nodeStatement;
	p->pLeft            = pStatementNode;
    p->pNext            = pStatementListNode;
	return p;
}

// Walk tree in postfix order; left, right, root.
void walkSyntaxTree(astNode* pSyntaxNode, char* position, int indent, FILE* fp) {
	if (pSyntaxNode == NULL) {
		return;
	}
    if (indent == 0) {
        printf("\nStatement:");
    }
	//printf("Start pattern walk");
    if (pSyntaxNode->type != nodeIf && pSyntaxNode->type != nodeWhile) {
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        walkSyntaxTree(pSyntaxNode->pCentre, "CENTRE", indent + 1, fp);  // TODO: Only used by if-then-else and while. Not necessary
        walkSyntaxTree(pSyntaxNode->pRight, "RIGHT", indent + 1, fp);
    }
    printIndent(indent);
    printf("%s", position);
    char tmp[64];
	switch (pSyntaxNode->type) {
	case (nodeOperator):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Operator %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Operator: ");
        printOperator(pSyntaxNode->value);
		break;
	case (nodeVariable):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Var: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
		break;
    case (nodeArray):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Array: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        break;
    case (nodeIf):
        if (fp != NULL) {
            sprintf(tmp, "%d %s If %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printIndent(indent);
		printf("If %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EVAL0 %d\n", indent, position, pSyntaxNode->value);
            //sprintf(tmp, "%d %s Then %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printIndent(indent);
		printf("If EVAL == 0 JMP Else %d", pSyntaxNode->value);
		//printf("THEN %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s JmpEndIf %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
            sprintf(tmp, "%d %s Else %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printIndent(indent);
		printf("JMP EndIf %d", pSyntaxNode->value);
		printf("\nElse %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pRight, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndIf %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printf("\nEndIf %d", pSyntaxNode->value);
        
        break;
    case (nodeWhile):
        if (fp != NULL) {
            sprintf(tmp, "%d %s While %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printIndent(indent);
		printf("While %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s Do %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printIndent(indent);
		printf("If EVAL == 0 JMP EndWhile %d", pSyntaxNode->value);
		//printf("Do %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndWhile %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        printf("\nEndWhile %d", pSyntaxNode->value);
        
        break;
    case (nodeStatement):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Statement %d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		//printf(" Statement: index,%d", pSyntaxNode->value);
        break;
    case (nodeArgumentCall):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Argument call%d\n", indent, position, pSyntaxNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }
        break;
	default:
		printf(" walkSyntaxTree: Unknown type:%d", pSyntaxNode->type);
		break;
	}
	//walkSyntaxTree(pSyntaxNode->pRight, "RIGHT", indent + 1);
	//printf("End pattern walk");
}

void walkList(astNode* pListNode, FILE* fp) {
#if REGRESS_1
    printf("\nwalkList(astNode* pListNode=%d)\n", (int)pListNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pListNode == NULL) {
		return;
	}
#if REGRESS_1    
    printf("\nwalkList(pListNode->pNext)=%d\n", (int)pListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkList(pListNode->pNext, fp);
    
#if REGRESS_1    
    printf("walkSyntaxTree(pListNode->pLeft=%d, ROOT, 0)\n", (int)pListNode->pLeft);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkSyntaxTree(pListNode->pLeft, "ROOT", 0, fp);
}

unsigned int countArguments(astNode* pArgNode) {
    unsigned int argCount = 0;
    while (pArgNode != NULL) {
        // This node should either be for a function call or function definition
        if (pArgNode->type != nodeArgumentCall && pArgNode->type != nodeArgumentValue && pArgNode->type != nodeArgumentReference) {
            debugAssert(ERR:countArguments());
            // Shouldn't happen
            return 0;
        }
        ++argCount;
        pArgNode = pArgNode->pNext;
    }
    return argCount;
}

void printIndent(const unsigned int indent) {
    int i;
    printf("\n");
    for (i = indent; i > 0; --i) {
        printf("*");
    }
}

void dumpSymbolTable(const char* fileName) {
	int i;
    FILE* fpSymbol = NULL;
    if (fileName != NULL) {
        fpSymbol = fopen(fileName, "wb");
        if (fpSymbol == NULL) {
            debugAssert(ERR:dumpSymbolTable() fpSymbol == NULL);
        }
    }
    printf("\n\nSymbol table start:");
    // Subtract 1 from symbolTableFreeIndex because the last index is a place
    //  holder for the function name.
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
		dumpSymbol(i, fpSymbol);
	}
    printf("\nSymbol table end:\n");
    if (fpSymbol != NULL) {
        fclose(fpSymbol);
    }
}

void dumpSymbol(int i, FILE* fpSymbol) {
    //nodeType symbolType = isConstant(&symbolTable[i]) ? nodeConst : nodeVariable;
    nodeType symbolType = symbolTable[i].type;
	printf("\nindex:%d, name:%s, type:%d, val:%d", i, symbolTable[i].name, symbolType, symbolTable[i].val);
    char tmp[64];
    sprintf(tmp, "%d %d\n", /*symbolTable[i].name,*/ symbolType, symbolTable[i].val);
    if (fpSymbol != NULL) {
        fwrite(tmp, 1, strlen(tmp), fpSymbol);
    }
}

// Calculate the symbol table index.
int getSymbolTableIndex(symbolNode* pSymbol) {
    if (pSymbol == NULL) {
        debugAssert(ERR:getSymbolTableIndex():pSymbol == NULL);
    }
    return (pSymbol - symbolTable) / sizeof(symbolTable[0]);
}

void printOperator(const int value) {
    switch (value) {
    case BANG:
        printf("!");
        break;
    case EQUAL:
        printf("=");
        break;
    case PLUS:
        printf("+");
        break;
    case MINUS:
        printf("-");
        break;
    case MULT:
        printf("*");
        break;
    case DIV:
        printf("/");
        break;
    case XOR:
        printf("^");
        break;
    case GEQ:
        printf(">=");
        break;
    case LEQ:
        printf("<=");
        break;
    case NEQ:
        printf("!=");
        break;
    case GTR:
        printf(">");
        break;
    case LSS:
        printf("<");
        break;
    case AND:
        printf("&&");
        break;
    case OR:
        printf("||");
        break;
    case BITWISEAND:
        printf("&");
        break;
    case BITWISEOR:
        printf("|");
        break;
    case TEST_FOR_EQUAL:
        printf("==");
        break;
    case LBRACKET:
        printf("array");
        break;
    default:
        printf("Unknown operator");
        break;
    }
    printf(" %d", value);
}

