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

unsigned int statementOutputIndex = 0;

#define QUOTES_MAIN                     "main"
#define QUOTES_MOVEABSOLUTE             "moveAbsolute"
#define QUOTES_MOVERELATIVE             "moveRelative"
#define QUOTES_SLEEP                    "sleep"
#define QUOTES_SLEEP_UNTIL              "sleepUntil"
#define QUOTES_LED                      "LED"
#define QUOTES_RPM                      "RPM"
#define QUOTES_RPM_x10k                 "RPMx10k"
#define QUOTES_ACCEL_MICROSEC           "accelMicroSec"
#define QUOTES_DEGREE_x10k_ABSOLUTE     "degreex10kAbsolute"
#define QUOTES_DEGREE_x10k_RELATIVE     "degreex10kRelative"




/* Line 189 of yacc.c  */
#line 121 "valve.tab.c"

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
     EQUAL = 261,
     LBRACE = 262,
     RBRACE = 263,
     ARRAYDEFINE = 264,
     IF = 265,
     ELSE = 266,
     WHILE = 267,
     VAR = 268,
     VAR_METHOD = 269,
     CONST = 270,
     CONST_FLOAT = 271,
     MAIN = 272,
     MOVEABSOLUTE = 273,
     MOVERELATIVE = 274,
     SLEEP = 275,
     SLEEPUNTIL = 276,
     LED = 277,
     RPM = 278,
     RPMx10k = 279,
     ACCELMICROSEC = 280,
     DEGREEx10kABSOLUTE = 281,
     DEGREEx10kRELATIVE = 282,
     BITWISEOR = 283,
     BITWISEAND = 284,
     OR = 285,
     AND = 286,
     SHR = 287,
     SHL = 288,
     LSS = 289,
     GTR = 290,
     NEQ = 291,
     LEQ = 292,
     GEQ = 293,
     TEST_FOR_EQUAL = 294,
     LPAREN = 295,
     MINUS = 296,
     PLUS = 297,
     XOR = 298,
     DIV = 299,
     MULT = 300,
     LBRACKET = 301,
     RBRACKET = 302,
     RPAREN = 303,
     SEMI = 304,
     BANG = 305,
     TILDE = 306
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 63 "valve.y"

	int integer;
    float floatingPoint;
	char* string;
	astNode* pSyntaxNode;



/* Line 214 of yacc.c  */
#line 217 "valve.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 229 "valve.tab.c"

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
#define YYLAST   771

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNRULES -- Number of states.  */
#define YYNSTATES  243

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,    10,    18,    26,    34,    35,
      38,    40,    42,    44,    50,    59,    65,    74,    80,    89,
      95,   104,   110,   119,   125,   136,   142,   153,   159,   168,
     174,   183,   189,   198,   204,   206,   214,   222,   234,   239,
     247,   250,   253,   256,   260,   264,   268,   272,   276,   280,
     284,   288,   292,   296,   300,   304,   308,   312,   316,   320,
     322,   324,   329,   334,   335,   337,   339,   343,   344,   346,
     348,   351,   355,   360
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    55,    54,    -1,    -1,    54,    56,    -1,
      17,    40,    65,    48,     7,    57,     8,    -1,    13,    40,
      65,    48,     7,    57,     8,    -1,    18,    40,    65,    48,
       7,    57,     8,    -1,    -1,    57,    58,    -1,    61,    -1,
      60,    -1,    59,    -1,    13,    40,    63,    48,    49,    -1,
      18,    40,    29,    13,     5,    62,    48,    49,    -1,    18,
      40,    63,    48,    49,    -1,    19,    40,    29,    13,     5,
      62,    48,    49,    -1,    19,    40,    63,    48,    49,    -1,
      20,    40,    29,    13,     5,    62,    48,    49,    -1,    20,
      40,    63,    48,    49,    -1,    21,    40,    29,    13,     5,
      62,    48,    49,    -1,    21,    40,    63,    48,    49,    -1,
      22,    40,    29,    13,     5,    62,    48,    49,    -1,    22,
      40,    63,    48,    49,    -1,    23,    40,    29,    13,     5,
      62,     5,    62,    48,    49,    -1,    23,    40,    63,    48,
      49,    -1,    24,    40,    29,    13,     5,    62,     5,    62,
      48,    49,    -1,    24,    40,    63,    48,    49,    -1,    25,
      40,    29,    13,     5,    62,    48,    49,    -1,    25,    40,
      63,    48,    49,    -1,    26,    40,    29,    13,     5,    62,
      48,    49,    -1,    26,    40,    63,    48,    49,    -1,    27,
      40,    29,    13,     5,    62,    48,    49,    -1,    27,    40,
      63,    48,    49,    -1,    67,    -1,    12,    40,    62,    48,
       7,    57,     8,    -1,    10,    40,    62,    48,     7,    57,
       8,    -1,    10,    40,    62,    48,     7,    57,     8,    11,
       7,    57,     8,    -1,    13,     6,    62,    49,    -1,    13,
      46,    62,    47,     6,    62,    49,    -1,    41,    62,    -1,
      50,    62,    -1,    51,    62,    -1,    40,    62,    48,    -1,
      62,    42,    62,    -1,    62,    41,    62,    -1,    62,    45,
      62,    -1,    62,    44,    62,    -1,    62,    43,    62,    -1,
      62,    31,    62,    -1,    62,    30,    62,    -1,    62,    29,
      62,    -1,    62,    28,    62,    -1,    62,    39,    62,    -1,
      62,    36,    62,    -1,    62,    38,    62,    -1,    62,    37,
      62,    -1,    62,    35,    62,    -1,    62,    34,    62,    -1,
      13,    -1,    15,    -1,    13,    40,    63,    48,    -1,    13,
      46,    62,    47,    -1,    -1,    64,    -1,    62,    -1,    62,
       5,    64,    -1,    -1,    66,    -1,    13,    -1,    29,    13,
      -1,    66,     5,    13,    -1,    66,     5,    29,    13,    -1,
       9,    13,    46,    15,    47,    49,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   103,   103,   105,   106,   108,   110,   112,   114,   115,
     117,   118,   119,   120,   124,   126,   128,   129,   131,   132,
     134,   135,   137,   138,   140,   141,   143,   144,   146,   147,
     149,   150,   152,   153,   155,   157,   159,   163,   165,   167,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   194,   195,   197,   198,   200,   201,   203,
     204,   205,   206,   208
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INPUTS", "OUTPUTS", "COMMA", "EQUAL",
  "LBRACE", "RBRACE", "ARRAYDEFINE", "IF", "ELSE", "WHILE", "VAR",
  "VAR_METHOD", "CONST", "CONST_FLOAT", "MAIN", "MOVEABSOLUTE",
  "MOVERELATIVE", "SLEEP", "SLEEPUNTIL", "LED", "RPM", "RPMx10k",
  "ACCELMICROSEC", "DEGREEx10kABSOLUTE", "DEGREEx10kRELATIVE", "BITWISEOR",
  "BITWISEAND", "OR", "AND", "SHR", "SHL", "LSS", "GTR", "NEQ", "LEQ",
  "GEQ", "TEST_FOR_EQUAL", "LPAREN", "MINUS", "PLUS", "XOR", "DIV", "MULT",
  "LBRACKET", "RBRACKET", "RPAREN", "SEMI", "BANG", "TILDE", "$accept",
  "program", "functionDefnList", "functionDefnMain", "functionDefn",
  "statementList", "statement", "statementWhile", "statementIf",
  "statementAssign", "expr", "argList", "commaArgList", "defnArgList",
  "defnCommaArgList", "arrayDefine", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    56,    56,    57,    57,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    60,    60,    61,    61,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    63,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    67
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     2,     7,     7,     7,     0,     2,
       1,     1,     1,     5,     8,     5,     8,     5,     8,     5,
       8,     5,     8,     5,    10,     5,    10,     5,     8,     5,
       8,     5,     8,     5,     1,     7,     7,    11,     4,     7,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     4,     4,     0,     1,     1,     3,     0,     1,     1,
       2,     3,     4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     3,    67,     1,     2,    69,     0,     0,
      68,     0,     0,     4,    70,     0,     0,    67,    67,     8,
      71,     0,     0,     0,     0,    72,     0,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,    12,    11,    10,    34,     8,     8,
       0,     0,     0,     0,    63,     0,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,     0,     0,     0,    59,
      60,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     6,     7,     0,    63,     0,     0,    40,    41,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    52,    51,    50,    49,
      58,    57,    54,    56,    55,    53,    45,    44,    48,    47,
      46,     8,     8,    66,    13,     0,     0,    15,     0,    17,
       0,    19,     0,    21,     0,    23,     0,    25,     0,    27,
       0,    29,     0,    31,     0,    33,    73,    61,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    36,    35,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    16,    18,    20,
      22,     0,     0,    28,    30,    32,     8,     0,     0,     0,
      24,    26,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     6,     3,    13,    24,    43,    44,    45,    46,
      78,    79,    80,     9,    10,    47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -50
static const yytype_int16 yypact[] =
{
       4,   -13,    40,   -50,    19,   -50,    33,   -50,    34,     2,
      47,    21,    42,   -50,   -50,    53,    20,    19,    19,   -50,
     -50,    90,    41,    60,   632,   -50,   102,   109,   -50,   112,
      82,   100,    13,   106,   107,   110,   111,   113,   114,   117,
     118,   121,   122,   -50,   -50,   -50,   -50,   -50,   -50,   -50,
      96,   231,   231,   231,   231,   231,   115,   119,   123,   162,
     165,   168,   185,   208,   214,   227,   652,   672,   134,   -20,
     -50,   231,   231,   231,   231,   299,   320,   255,     0,   120,
     -50,   572,   154,   128,   166,   136,   169,   137,   173,   139,
     176,   142,   179,   145,   182,   148,   186,   153,   191,   159,
     197,   163,   -50,   -50,   170,   231,   231,   341,    57,   -50,
     -50,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   213,   215,   -50,   231,
     175,   222,   225,   183,   226,   184,   229,   189,   234,   192,
     240,   198,   245,   202,   247,   204,   252,   211,   256,   217,
     257,   220,   221,   228,   592,   -50,   726,   726,   726,   726,
      54,    54,    54,    54,    54,    54,    57,    57,   -37,   -50,
     -50,   -50,   -50,   -50,   -50,   231,   231,   -50,   231,   -50,
     231,   -50,   231,   -50,   231,   -50,   231,   -50,   231,   -50,
     231,   -50,   231,   -50,   231,   -50,   -50,   -50,   -50,   692,
     712,   277,   362,   383,   404,   425,   446,    49,    76,   467,
     488,   509,   262,   -50,   -50,   230,   238,   239,   246,   253,
     231,   231,   254,   260,   261,   267,   -50,   -50,   -50,   -50,
     -50,   530,   551,   -50,   -50,   -50,   -50,   268,   274,   732,
     -50,   -50,   -50
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -50,   -50,   -50,   -50,   -50,   -48,   -50,   -50,   -50,   -50,
     -49,   -47,   146,    38,   -50,   -50
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      66,    67,    75,    76,    77,   129,    81,   124,   125,    83,
      85,    87,    89,    91,    93,    95,    97,    99,   101,    53,
     105,     1,   107,   108,   109,   110,   106,     4,   111,   112,
     113,   114,     7,    20,   115,   116,   117,   118,   119,   120,
       5,   121,   122,   123,   124,   125,    11,    14,     8,    21,
      15,    12,    16,    54,   220,    22,    23,   154,   153,    55,
      19,    17,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   111,   112,   113,
     114,   221,    18,   115,   116,   117,   118,   119,   120,    26,
     121,   122,   123,   124,   125,   121,   122,   123,   124,   125,
     123,   124,   125,    25,   111,   112,   113,   114,    27,    48,
     115,   116,   117,   118,   119,   120,    49,   121,   122,   123,
     124,   125,    51,   199,   200,    50,   201,   202,    69,   203,
      70,   204,    69,   205,    70,   206,    69,   207,    70,   208,
      52,   209,    68,   210,    82,   211,    56,    57,    84,   104,
      58,    59,    86,    60,    61,    71,    72,    62,    63,    71,
      72,    64,    65,    71,    72,    73,    74,   132,   130,    73,
      74,   231,   232,    73,    74,    69,   133,    70,    69,   134,
      70,    69,   136,    70,   135,   137,   138,   139,   239,   140,
     141,    88,   142,   143,    90,   144,   145,    92,    69,   146,
      70,   147,    71,    72,   148,    71,    72,   149,    71,    72,
     150,   151,    73,    74,    94,    73,    74,   152,    73,    74,
     171,    69,   172,    70,   174,    71,    72,    69,   175,    70,
     176,   178,   177,   179,   180,    73,    74,    96,   181,   182,
      69,   183,    70,    98,    69,   184,    70,   185,    71,    72,
     186,   187,   188,   189,    71,    72,   100,   190,    73,    74,
     191,   192,   194,     0,    73,    74,   193,    71,    72,   195,
     196,    71,    72,   225,   236,   173,   197,    73,    74,   226,
       0,    73,    74,   111,   112,   113,   114,   227,   228,   115,
     116,   117,   118,   119,   120,   229,   121,   122,   123,   124,
     125,     0,   230,   233,   128,   111,   112,   113,   114,   234,
     235,   115,   116,   117,   118,   119,   120,   240,   121,   122,
     123,   124,   125,   241,     0,     0,   214,   111,   112,   113,
     114,     0,     0,   115,   116,   117,   118,   119,   120,     0,
     121,   122,   123,   124,   125,     0,     0,   126,   111,   112,
     113,   114,     0,     0,   115,   116,   117,   118,   119,   120,
       0,   121,   122,   123,   124,   125,     0,     0,   127,   111,
     112,   113,   114,     0,     0,   115,   116,   117,   118,   119,
     120,     0,   121,   122,   123,   124,   125,     0,     0,   155,
     111,   112,   113,   114,     0,     0,   115,   116,   117,   118,
     119,   120,     0,   121,   122,   123,   124,   125,     0,     0,
     215,   111,   112,   113,   114,     0,     0,   115,   116,   117,
     118,   119,   120,     0,   121,   122,   123,   124,   125,     0,
       0,   216,   111,   112,   113,   114,     0,     0,   115,   116,
     117,   118,   119,   120,     0,   121,   122,   123,   124,   125,
       0,     0,   217,   111,   112,   113,   114,     0,     0,   115,
     116,   117,   118,   119,   120,     0,   121,   122,   123,   124,
     125,     0,     0,   218,   111,   112,   113,   114,     0,     0,
     115,   116,   117,   118,   119,   120,     0,   121,   122,   123,
     124,   125,     0,     0,   219,   111,   112,   113,   114,     0,
       0,   115,   116,   117,   118,   119,   120,     0,   121,   122,
     123,   124,   125,     0,     0,   222,   111,   112,   113,   114,
       0,     0,   115,   116,   117,   118,   119,   120,     0,   121,
     122,   123,   124,   125,     0,     0,   223,   111,   112,   113,
     114,     0,     0,   115,   116,   117,   118,   119,   120,     0,
     121,   122,   123,   124,   125,     0,     0,   224,   111,   112,
     113,   114,     0,     0,   115,   116,   117,   118,   119,   120,
       0,   121,   122,   123,   124,   125,     0,     0,   237,   111,
     112,   113,   114,     0,     0,   115,   116,   117,   118,   119,
     120,     0,   121,   122,   123,   124,   125,     0,     0,   238,
     111,   112,   113,   114,     0,     0,   115,   116,   117,   118,
     119,   120,     0,   121,   122,   123,   124,   125,     0,   131,
     111,   112,   113,   114,     0,     0,   115,   116,   117,   118,
     119,   120,     0,   121,   122,   123,   124,   125,     0,   198,
      28,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     102,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     103,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     212,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     213,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     242,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     115,   116,   117,   118,   119,   120,     0,   121,   122,   123,
     124,   125
};

static const yytype_int16 yycheck[] =
{
      48,    49,    51,    52,    53,     5,    55,    44,    45,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,     6,
      40,    17,    71,    72,    73,    74,    46,    40,    28,    29,
      30,    31,    13,    13,    34,    35,    36,    37,    38,    39,
       0,    41,    42,    43,    44,    45,    13,    13,    29,    29,
      48,    18,     5,    40,     5,    17,    18,   106,   105,    46,
       7,    40,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    28,    29,    30,
      31,     5,    40,    34,    35,    36,    37,    38,    39,    48,
      41,    42,    43,    44,    45,    41,    42,    43,    44,    45,
      43,    44,    45,    13,    28,    29,    30,    31,    48,     7,
      34,    35,    36,    37,    38,    39,     7,    41,    42,    43,
      44,    45,    40,   171,   172,    13,   175,   176,    13,   178,
      15,   180,    13,   182,    15,   184,    13,   186,    15,   188,
      40,   190,    46,   192,    29,   194,    40,    40,    29,    15,
      40,    40,    29,    40,    40,    40,    41,    40,    40,    40,
      41,    40,    40,    40,    41,    50,    51,    13,    48,    50,
      51,   220,   221,    50,    51,    13,    48,    15,    13,    13,
      15,    13,    13,    15,    48,    48,    13,    48,   236,    13,
      48,    29,    13,    48,    29,    13,    48,    29,    13,    13,
      15,    48,    40,    41,    13,    40,    41,    48,    40,    41,
      13,    48,    50,    51,    29,    50,    51,    47,    50,    51,
       7,    13,     7,    15,    49,    40,    41,    13,     6,    15,
       5,     5,    49,    49,     5,    50,    51,    29,    49,     5,
      13,    49,    15,    29,    13,     5,    15,    49,    40,    41,
       5,    49,     5,    49,    40,    41,    29,     5,    50,    51,
      49,     5,     5,    -1,    50,    51,    49,    40,    41,    49,
      49,    40,    41,    11,     7,   129,    48,    50,    51,    49,
      -1,    50,    51,    28,    29,    30,    31,    49,    49,    34,
      35,    36,    37,    38,    39,    49,    41,    42,    43,    44,
      45,    -1,    49,    49,    49,    28,    29,    30,    31,    49,
      49,    34,    35,    36,    37,    38,    39,    49,    41,    42,
      43,    44,    45,    49,    -1,    -1,    49,    28,    29,    30,
      31,    -1,    -1,    34,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    28,    29,
      30,    31,    -1,    -1,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    43,    44,    45,    -1,    -1,    48,    28,
      29,    30,    31,    -1,    -1,    34,    35,    36,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    -1,    -1,    48,
      28,    29,    30,    31,    -1,    -1,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    28,    29,    30,    31,    -1,    -1,    34,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    28,    29,    30,    31,    -1,    -1,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    48,    28,    29,    30,    31,    -1,    -1,    34,
      35,    36,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    -1,    -1,    48,    28,    29,    30,    31,    -1,    -1,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    48,    28,    29,    30,    31,    -1,
      -1,    34,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    28,    29,    30,    31,
      -1,    -1,    34,    35,    36,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    -1,    -1,    48,    28,    29,    30,
      31,    -1,    -1,    34,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    28,    29,
      30,    31,    -1,    -1,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    43,    44,    45,    -1,    -1,    48,    28,
      29,    30,    31,    -1,    -1,    34,    35,    36,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    -1,    -1,    48,
      28,    29,    30,    31,    -1,    -1,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    -1,    47,
      28,    29,    30,    31,    -1,    -1,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    -1,    47,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       8,     9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    17,    53,    55,    40,     0,    54,    13,    29,    65,
      66,    13,    18,    56,    13,    48,     5,    40,    40,     7,
      13,    29,    65,    65,    57,    13,    48,    48,     8,     9,
      10,    12,    13,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    58,    59,    60,    61,    67,     7,     7,
      13,    40,    40,     6,    40,    46,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    57,    57,    46,    13,
      15,    40,    41,    50,    51,    62,    62,    62,    62,    63,
      64,    62,    29,    63,    29,    63,    29,    63,    29,    63,
      29,    63,    29,    63,    29,    63,    29,    63,    29,    63,
      29,    63,     8,     8,    15,    40,    46,    62,    62,    62,
      62,    28,    29,    30,    31,    34,    35,    36,    37,    38,
      39,    41,    42,    43,    44,    45,    48,    48,    49,     5,
      48,    47,    13,    48,    13,    48,    13,    48,    13,    48,
      13,    48,    13,    48,    13,    48,    13,    48,    13,    48,
      13,    48,    47,    63,    62,    48,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,     7,     7,    64,    49,     6,     5,    49,     5,    49,
       5,    49,     5,    49,     5,    49,     5,    49,     5,    49,
       5,    49,     5,    49,     5,    49,    49,    48,    47,    57,
      57,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,     8,     8,    49,    48,    48,    48,    48,    48,
       5,     5,    48,    48,    48,    11,    49,    49,    49,    49,
      49,    62,    62,    49,    49,    49,     7,    48,    48,    57,
      49,    49,     8
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
#line 103 "valve.y"
    {postProcess(); dumpSymbolTable("symbolTable.txt");;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 105 "valve.y"
    {;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 106 "valve.y"
    {;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 108 "valve.y"
    {addFunction(QUOTES_MAIN, (yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 110 "valve.y"
    {addFunction((yyvsp[(1) - (7)].string), (yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 112 "valve.y"
    {yyerror(QUOTES_MOVEABSOLUTE);;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 114 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 115 "valve.y"
    {/*printf("\nstatementList:%d, statement:%d", (int)$1, (int)$2);*/ (yyval.pSyntaxNode) = addStatement((yyvsp[(1) - (2)].pSyntaxNode), (yyvsp[(2) - (2)].pSyntaxNode)); /*printf(" newStatementList:%d", (int)$$);*/;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 117 "valve.y"
    {/*printf("\nstatementAssign:%d", (int)$1); walkSyntaxTree($1, "start", 0);*/ (yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 118 "valve.y"
    {/*printf("\nstatementIf:%d", (int)$1);*/ (yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 119 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 120 "valve.y"
    {(yyval.pSyntaxNode) = addNodeFunctionCall((yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].pSyntaxNode));;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 124 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_MOVEABSOLUTE, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 126 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_MOVEABSOLUTE);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 128 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_MOVERELATIVE, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 129 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_MOVERELATIVE);;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 131 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_SLEEP, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 132 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_SLEEP);;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 134 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_SLEEP_UNTIL, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 135 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_SLEEP_UNTIL);;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 137 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_LED, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 138 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_LED);;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 140 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction2(QUOTES_RPM, (yyvsp[(4) - (10)].string), (yyvsp[(6) - (10)].pSyntaxNode), (yyvsp[(8) - (10)].pSyntaxNode));;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 141 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_RPM);;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 143 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction2(QUOTES_RPM_x10k, (yyvsp[(4) - (10)].string), (yyvsp[(6) - (10)].pSyntaxNode), (yyvsp[(8) - (10)].pSyntaxNode));;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 144 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_RPM_x10k);;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 146 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_ACCEL_MICROSEC, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 147 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_ACCEL_MICROSEC);;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 149 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_DEGREE_x10k_ABSOLUTE, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 150 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_DEGREE_x10k_ABSOLUTE);;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 152 "valve.y"
    {(yyval.pSyntaxNode) = addNodeInstrinsicFunction1(QUOTES_DEGREE_x10k_RELATIVE, (yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].pSyntaxNode));;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 153 "valve.y"
    {(yyval.pSyntaxNode) = NULL; yyerror(QUOTES_DEGREE_x10k_RELATIVE);;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 155 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 157 "valve.y"
    {(yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode), NULL, nodeWhile);;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 159 "valve.y"
    {/*printf("statementIf:statementList:%d", (int)$6);*/ (yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (7)].pSyntaxNode), (yyvsp[(6) - (7)].pSyntaxNode), NULL, nodeIf);;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 163 "valve.y"
    {/*printf("statementIf:statementList:%d", (int)$6);*/ (yyval.pSyntaxNode) = addNodeIfOrWhile((yyvsp[(3) - (11)].pSyntaxNode), (yyvsp[(6) - (11)].pSyntaxNode), (yyvsp[(10) - (11)].pSyntaxNode), nodeIf);;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 165 "valve.y"
    {/*printf("\nstatementAssign");*/ (yyval.pSyntaxNode) = addNodeVariableOperator(EQUAL, addVarToSymbolTable((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 167 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(EQUAL, addNodeArray((yyvsp[(1) - (7)].string), (yyvsp[(3) - (7)].pSyntaxNode)), (yyvsp[(6) - (7)].pSyntaxNode));;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 170 "valve.y"
    {astNode* pMinusOneNode = addNodeSymbolIndex(addVarToSymbolTable("-1")); (yyval.pSyntaxNode) = addNodeBinaryOperator(MULT, (yyvsp[(2) - (2)].pSyntaxNode), pMinusOneNode);;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 171 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(BANG, (yyvsp[(2) - (2)].pSyntaxNode), NULL);;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 172 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(TILDE, (yyvsp[(2) - (2)].pSyntaxNode), NULL);;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 173 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(2) - (3)].pSyntaxNode);;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 174 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(PLUS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 175 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(MINUS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 176 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(MULT, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 177 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(DIV, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 178 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(XOR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 179 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(AND, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 180 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(OR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 181 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(BITWISEAND, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 182 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(BITWISEOR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 183 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(TEST_FOR_EQUAL, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 184 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(NEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 185 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(GEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 186 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(LEQ, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 187 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(GTR, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 188 "valve.y"
    {(yyval.pSyntaxNode) = addNodeBinaryOperator(LSS, (yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].pSyntaxNode));;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 189 "valve.y"
    {(yyval.pSyntaxNode) = addNodeSymbolIndex(addVarToSymbolTable((yyvsp[(1) - (1)].string)));;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 190 "valve.y"
    {(yyval.pSyntaxNode) = addNodeSymbolIndex(addVarToSymbolTable((yyvsp[(1) - (1)].string)));;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 191 "valve.y"
    {(yyval.pSyntaxNode) = addNodeFunctionCall((yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 192 "valve.y"
    {(yyval.pSyntaxNode) = addNodeArray((yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].pSyntaxNode));;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 194 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 195 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 197 "valve.y"
    {(yyval.pSyntaxNode) = addFcnCallArgument(NULL, (yyvsp[(1) - (1)].pSyntaxNode));;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 198 "valve.y"
    {(yyval.pSyntaxNode) = addFcnCallArgument((yyvsp[(3) - (3)].pSyntaxNode), (yyvsp[(1) - (3)].pSyntaxNode));;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 200 "valve.y"
    {(yyval.pSyntaxNode) = NULL;;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 201 "valve.y"
    {(yyval.pSyntaxNode) = (yyvsp[(1) - (1)].pSyntaxNode);;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 203 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument(NULL, (yyvsp[(1) - (1)].string), VAR_PASS_BY_VALUE, VAR_FIRST_PARAMETER);;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 204 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument(NULL, (yyvsp[(2) - (2)].string), VAR_PASS_BY_REFERENCE, VAR_FIRST_PARAMETER);;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 205 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(1) - (3)].pSyntaxNode), (yyvsp[(3) - (3)].string), VAR_PASS_BY_VALUE, VAR_SUBSEQUENT_PARAMETER);;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 206 "valve.y"
    {(yyval.pSyntaxNode) = addFcnDefnArgument((yyvsp[(1) - (4)].pSyntaxNode), (yyvsp[(4) - (4)].string), VAR_PASS_BY_REFERENCE, VAR_SUBSEQUENT_PARAMETER);;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 208 "valve.y"
    {(yyval.integer) = addArrayToSymbolTable((yyvsp[(2) - (6)].string), atoi((yyvsp[(4) - (6)].string)));;}
    break;



/* Line 1464 of yacc.c  */
#line 2231 "valve.tab.c"
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
#line 210 "valve.y"
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

	//initVarTable(); // Doesn't seem necessary since we use buildSymbol()
	// To turn on debugging, make sure the next line is uncommented and
	//  turn on the -t (also use -v -l) options in bison.exe.
	yydebug = 1; 
    yyin = fopen("valve5.def", "r" );
	yyparse ();
    return 0;
}

void postProcess(void) {
    checkFcnSanity();
    fixupFcnCalls();
}

// Replace function call fcnDefnLink with the index of the function definition
void fixupFcnCalls(void) {
    int i;
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        //printf("\nindex:%d, type:%d", i, symbolTable[i].type);
        if (symbolTable[i].type == nodeFunctionCall) {
            // Find the definition for this call and grab its statement link index.
            int fcnDefnIndex;
            int count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count != 1) {
                // We should have only one definition and this should have been detected in checkFcnSanity()
                debugAssert(ERR:fixupFcnCalls():incorrect number of function definitions);
                return;
            }
            //printf("\nfixupFcnCalls():%d, %d", i, symbolTableFreeIndex);
            symbolTable[i].fcnDefnLink = fcnDefnIndex;
            //break;
        }
    }
}

void checkFcnSanity(void) {
    //printf("checkFcnSanity");
    // 0. Check for a function called main()
	symbolNode tmp;
    int fcnDefnIndex;
    buildSymbol(QUOTES_MAIN, 0, &tmp);
    int count = findSymbolFcnDefinition(&tmp, &fcnDefnIndex);
    if (count == 0) {
        debugAssert(ERR:checkFcnSanity():no 'main()' found);
    }
    
    // 1. Check for duplicate definitions including duplicates of 'main()'
    int i;
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        if (symbolTable[i].type == nodeFunctionDefinition) {
            count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count > 1) {
                debugAssert(ERR:checkFcnSanity():multiple definitions found:);
                printf("%s", symbolTable[i].name);
                break;
            }
        }
    }
    
    // 2. Check number of parameters match function call and definition
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        if (symbolTable[i].type == nodeFunctionCall) {
            // Make sure there's a matching definition.
            count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count == 0) {
                debugAssert(ERR:checkFcnSanity():no definition found:check parameter count:);
                printf("%s", symbolTable[i].name);
                break;
            }
        }
    }
}

#if 0
void initVarTable(void) {
	int i;
	for (i = VAR_ITEMS - 1; i >= 0; i--) {
		symbolTable[i].name[0] = EOS;
		symbolTable[i].val = DEFAULT_VAR_VALUE;	// Initialize variable to 0
        symbolTable[i].fcnDefnLink = VAR_FCN_LINK_UNDEFINED;
	}
}
#endif

// Return index of variable/constant in symbol table
int insertSymbol(symbolNode* pVar) {
	if (symbolTableFreeIndex < VAR_ITEMS) {
		symbolTable[symbolTableFreeIndex] = *pVar;
		return symbolTableFreeIndex++;
	}
	return VAR_TABLE_LIMIT;
}

// A little danger here. Forces symbol to be placed at a specific index.
// Used to insert function name at start of symbol list for that function.
void insertSymbolAtIndex(symbolNode* pVar, const unsigned int index) {
    if (index >= VAR_ITEMS) {
        debugAssert(ERR:insertSymbolAtIndex():index >= VAR_ITEMS);
        return;
    }
    symbolTable[index] = *pVar;
}

// Return index of variable (includes function names), or VAR_NOT_FOUND if not found.
int findSymbolByName(const char* pVarName) {
    symbolNode thisNode;
    buildSymbol(pVarName, 0, &thisNode);
    return findSymbol(&thisNode);
}

// Return index where variable is located in symbolTable, or VAR_NOT_FOUND if not found.
// TODO: nodeConst can be anywhere in symbol table since its value never varies. May want to reuse these constants.
int findSymbol(symbolNode* pVar) {
	int i;
    // Only search within the scope of the function currently being parsed.
	for (i = symbolTableLastFunctionIndex; i < symbolTableFreeIndex; ++i) {
		if (strncmp(symbolTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
			return i;
		}
	}
	return VAR_NOT_FOUND;
}

// Returns the number of function definitions that match the symbolNode. Also return
//  the index where it can be found. That symbol contains the index into the statement list
//  for the actual location of the definition.
int findSymbolFcnDefinition(symbolNode* pVar, int* pFcnDefnIndex) {
    if (pFcnDefnIndex == NULL) {
        debugAssert(ERR:findSymbolFcnDefinition():pFcnDefnIndex == NULL);
        return 0;
    }
    unsigned int count = 0;
    int i;
    //printf("symbolTableFreeIndex=%d", symbolTableFreeIndex);
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        //printf("\ntype=%d, val=%d, name=%s", symbolTable[i].type, symbolTable[i].val, symbolTable[i].name);
        //printf("\ntype=%d, val=%d, name=%s", pVar->type, pVar->val, pVar->name);
		if (symbolTable[i].type == nodeFunctionDefinition &&
            symbolTable[i].val == pVar->val /* contains number of arguments to this function */ &&
            strncmp(symbolTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
            *pFcnDefnIndex = symbolTable[i].fcnDefnLink;
            ++count;
        }
	}
    // 5x. Add test for intrinsic functions here. TODO: Find a better way to do this!
    if (strncmp(QUOTES_MOVEABSOLUTE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_MOVE_ABSOLUTE;
        //printf(QUOTES_MOVEABSOLUTE);
        ++count;
    } else if (strncmp(QUOTES_MOVERELATIVE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_MOVE_RELATIVE;
        ++count;
    } else if (strncmp(QUOTES_SLEEP, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_SLEEP;
        ++count;
    } else if (strncmp(QUOTES_SLEEP_UNTIL, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_SLEEP_UNTIL;
        ++count;
    } else if (strncmp(QUOTES_LED, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_LED;
        ++count;
    } else if (strncmp(QUOTES_RPM, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_THREE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_RPM;
        ++count;
    } else if (strncmp(QUOTES_RPM_x10k, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_THREE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_RPM_x10k;
        ++count;
    } else if (strncmp(QUOTES_ACCEL_MICROSEC, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_ACCEL_MICROSEC;
        ++count;
    } else if (strncmp(QUOTES_DEGREE_x10k_ABSOLUTE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_DEGREE_x10k_ABSOLUTE;
        ++count;
    } else if (strncmp(QUOTES_DEGREE_x10k_RELATIVE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_DEGREE_x10k_RELATIVE;
        ++count;
    }
    return count;
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

// e.g. '!variableName'
astNode* addNodeUnaryOperator(int operator, astNode* pRight) {
    return addNodeBinaryOperator(operator, NULL, pRight);
}

// e.g. 'c3 = 4 * c1;' where 'operator' is '='
astNode* addNodeVariableOperator(int operator, int varIndex, astNode* pRight) {
	astNode* pLeft  = getNextASTNode();
	pLeft->type     = nodeVariable;
	pLeft->value    = varIndex;
    return addNodeBinaryOperator(operator, pLeft, pRight);
}

// The 1 stands for 1 parameter with this call
astNode* addNodeInstrinsicFunction1(char* functionName, char* returnValue, astNode* parameter1) {
    astNode* p1 = addNodeSymbolIndex(addVarToSymbolTable(returnValue));
    astNode* p2 = addFcnCallArgument(NULL, parameter1);
    astNode* p3 = addFcnCallArgument(p2, p1);
    return addNodeFunctionCall(functionName, p3);
}

// The 2 stands for 2 parameters with this call. Note how return parameter, p1, is added last and it's the first parameter in the function call.
//  Parameters are added in the reverse order they appear in the function call.
astNode* addNodeInstrinsicFunction2(char* functionName, char* returnValue, astNode* parameter1, astNode* parameter2) {
    astNode* p1 = addNodeSymbolIndex(addVarToSymbolTable(returnValue));
    astNode* p2 = addFcnCallArgument(NULL, parameter2);
    astNode* p3 = addFcnCallArgument(p2, parameter1);
    astNode* p4 = addFcnCallArgument(p3, p1);
    return addNodeFunctionCall(functionName, p4);
}

astNode* addNodeFunctionCall(char* pFuncName, astNode* pArgList) {
    //printf("\naddNodeFunctionCall()");
    return addFunction(pFuncName, pArgList, NULL);
}

// Add either a function call (pStatmentList = NULL) or a function definition (pStatmentList != NULL)
astNode* addFunction(const char* pFuncName, astNode* pArgList, astNode* pStatementList) {
    // Essentially the same as addNodeFunctionCall() except this is a definition, not a call.
    // 0. Count number of arguments and assign that value to its syntax node
    //int argCount = countArguments(pArgList);
    //printf("\naddFunction():argCount=%d", argCount);
    // 1. See if pFuncName is already in the symbol list. It could be there from
    //     another function call or the function definition.
    // Make symbol table entry and add to symbol table
#if 0    
    int symbolIndex = findSymbolByName(pFuncName);
    if (symbolIndex == VAR_NOT_FOUND) {
        symbolNode funcNode;
        buildSymbol(pFuncName, argCount, &funcNode);
        funcNode.type = nodeFunctionCall;
        int symbolIndex = insertSymbol(&funcNode);
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
        // Add function name to this function's symbol table
        int argCount = countArguments(pArgList);
        int symbolIndex = findSymbolByName(pFuncName);
        if (symbolIndex == VAR_NOT_FOUND) {
            symbolNode funcNode;
            buildSymbol(pFuncName, argCount, &funcNode);
            funcNode.type = nodeFunctionCall;
            symbolIndex = insertSymbol(&funcNode);
            if (symbolIndex == VAR_TABLE_LIMIT) {
                return NULL;
            }
        } else {
            if (symbolTable[symbolIndex].type != nodeFunctionCall) {
                debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].type != nodeFunctionCall);
                return NULL;
            }
        }
        
        astNode* p = getNextASTNode();
        p->type = nodeFunctionCall;
        p->value = symbolIndex;
        p->pNext = pArgList;
        return p;
    } else {
        // This is a function definition not a function call
        if (pFuncName == NULL) {
            debugAssert(ERR:addNodeFunction():pFuncName == NULL);
        } else {
            printf("\nFunctionDefn: %s", pFuncName);
        }
        // TODO: Go through pStatementList, fixing up references to arguments so they get
        //  picked off the evaluation stack, rather than the symbol table.
        // Add function name to reserved space in symbol table.
        symbolNode functionNode;
        buildSymbol(pFuncName, functionParameterIndex /* equals num arguments */, &functionNode);
        // This is set to 1 because of the way function parameters are parsed. The first parameter doesn't actually get parsed until last
        //  but should have index 0. We correct for this by noting in the addFcnDefnArgument() call if it's the first parameter.
        functionParameterIndex = 0; // Reset for every new function definition. Don't need it any more for this function.
        functionNode.type = nodeFunctionDefinition;
        // statementOutputIndex contains the index into execution list. Put that value into fcnDefnLink
        //  of the function definition symbol.
        functionNode.fcnDefnLink = statementOutputIndex;
        insertSymbolAtIndex(&functionNode, symbolTableLastFunctionIndex);
        symbolTableLastFunctionIndex = symbolTableFreeIndex++;
        
        FILE* fp = NULL;
        int inMain = FALSE;
        if (strcmp(pFuncName, QUOTES_MAIN) == 0) {
            // Clears the file so it doesn't keep getting bigger each time this program is run.
            fp = fopen("tree.txt", "wb");
            statementOutputIndex = 0;
            inMain = TRUE;
        } else {
            fp = fopen("tree.txt", "ab");
        }
        writeStatement("0 0 Start 0", fp);
        /*printf("\nstatementList=%d", (int)$1);*/
        //walkList(pArgList, fp);   // Doesn't seem to be necessary since function parameters have alreay been added to the symbol table
        walkList(pStatementList, fp);
        if (inMain) {
            writeStatement("0 0 ProgramEnd 0", fp);
            printf("\nProgramEnd");
        }
        fclose(fp);
    }
    return NULL;
}

astNode* addNodeArray(char* pVarName, astNode* pASTNode) {
    // 1. Make new astNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pASTNode->value, pASTNode->pLeft->value, pASTNode->pRight->value);
    //printf("xxx root=%d\n", pASTNode->value);
	astNode* pArrayVar = getNextASTNode();
	pArrayVar->type = nodeArray;
    symbolNode pArrayNode;
    buildSymbol(pVarName, 0, &pArrayNode);
	pArrayVar->value = findSymbol(&pArrayNode);
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

astNode* addNodeSymbolIndex(int varIndex) {
	astNode* p  = getNextASTNode();
	p->type     = nodeVariable;
	p->value    = varIndex;
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

// A variable may not start with a number. One that does is considered a constant.
int isConstant(symbolNode* pVar) {
	if (isdigit((int)pVar->name[0]) || (pVar->name[0] == '-' && isdigit((int)pVar->name[1]))) {
		return TRUE;
	}
	return FALSE;
}

void buildSymbol(const char* name, int value, symbolNode* varNode) {
    if (name == NULL || varNode == NULL) {
        debugAssert(ERR:buildSymbol());
        return;
    }
	strncpy(varNode->name, name, VAR_NAME_LENGTH-1);
	varNode->name[VAR_NAME_LENGTH-1] = EOS;
	varNode->val = value;
    varNode->type = nodeVariable;
    varNode->fcnDefnLink = VAR_FCN_LINK_UNDEFINED;
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
    buildSymbol(var, maxRange, &tmp);
    int found = findSymbol(&tmp);
	if (found == VAR_NOT_FOUND) {
        if (insertSymbol(&tmp) == VAR_TABLE_LIMIT) {
            return VAR_TABLE_LIMIT;
        }
        if (symbolTableFreeIndex + maxRange >= VAR_ITEMS) {
            symbolTableFreeIndex--;    // Remove variable just inserted with insertSymbol().
            return VAR_TABLE_LIMIT;
        }
        symbolTableFreeIndex += maxRange;
    }
    return found;
}

// Return index in symbol table
int addVarToSymbolTable(char* var) {
	symbolNode tmp;
    buildSymbol(var, DEFAULT_VAR_VALUE, &tmp);
	int found = findSymbol(&tmp);
    if (var == NULL) {
        debugAssert(ERR:addVarToSymbolTable():var == NULL);
        return VAR_NOT_FOUND;
    }
	if (found == VAR_NOT_FOUND) {
		return insertSymbol(&tmp);
	}
	return found;
}

// TODO: Return value of this function can be void since we're inserting arguments right in the symbol table. 
//        The only AST is used in a function call, not the definition.
astNode* addFcnDefnArgument(astNode* pArgumentListNode, const char* pArgumentName, const int passByValueOrReference, const int firstOrSebsequentParameter) {
    if (pArgumentName == NULL) {
        debugAssert(ERR:addFcnDefnArgument():pArgumentName == NULL);
        return NULL;
    }
    //printf("\naddFcnDefnArgument():pArgumentName=%s, pArgumentListNode=%x", pArgumentName, (unsigned int)pArgumentListNode);
    // Add symbols to symbol table, marking them as function parameters.
    // Note that the variable value refers to its index on the execution stack.
    symbolNode argumentNode;
    buildSymbol(pArgumentName, functionParameterIndex++, &argumentNode);
    argumentNode.type = nodeArgumentValue;
    if (passByValueOrReference == VAR_PASS_BY_REFERENCE) {
        argumentNode.type = nodeArgumentReference;
    }
    insertSymbol(&argumentNode);
    return NULL;
#if 0    
    // Old code
	astNode* p = getNextASTNode();
    p->type = passByValueOrReference;
    // Create variable in symbol table
    symbolNode tmp;
    buildSymbol(pArgumentName, DEFAULT_VAR_VALUE, &tmp);
    tmp.type = passByValueOrReference;
    //p->Left = 
    p->pNext = pArgumentListNode;
    return NULL;
#endif    
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
    //printf("\naddFcnCallArgument()");
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
            sprintf(tmp, "%d %s Operator %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Operator: ");
        printOperator(pSyntaxNode->value);
		break;
	case (nodeVariable):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Var: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
		break;
    case (nodeArray):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Array: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        break;
    case (nodeIf):
        if (fp != NULL) {
            sprintf(tmp, "%d %s If %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EVAL0 %d", indent, position, pSyntaxNode->value);
            //sprintf(tmp, "%d %s Then %d\n", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If EVAL == 0 JMP Else %d", pSyntaxNode->value);
		//printf("THEN %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s JmpEndIf %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
            sprintf(tmp, "%d %s Else %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("JMP EndIf %d", pSyntaxNode->value);
		printf("\nElse %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pRight, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndIf %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf("\nEndIf %d", pSyntaxNode->value);
        
        break;
    case (nodeWhile):
        if (fp != NULL) {
            sprintf(tmp, "%d %s While %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("While %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EVALWHILE0 %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If EVALWHILE == 0 JMP EndWhile %d", pSyntaxNode->value);
		//printf("Do %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndWhile %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf("\nEndWhile %d", pSyntaxNode->value);
        
        break;
    case (nodeStatement):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Statement %d\n", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		//printf(" Statement: index,%d", pSyntaxNode->value);
        break;
    case (nodeArgumentCall):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Argument call%d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        //printf("\n Function argument:");
        break;
    case (nodeFunctionCall):
        if (fp != NULL) {
            sprintf(tmp, "%d %s FunctionCall %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf(" FunctionCall: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        //walkList(pSyntaxNode, fp);
        walkListBackwards(pSyntaxNode, fp);
        if (fp != NULL) {
            sprintf(tmp, "%d %s FunctionCallEnd %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf(" \nFunctionCallEnd: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        break;
	default:
		printf(" walkSyntaxTree: Unknown type:%d", pSyntaxNode->type);
		break;
	}
	//walkSyntaxTree(pSyntaxNode->pRight, "RIGHT", indent + 1);
	//printf("End pattern walk");
}

void writeStatement(const char* pTmp, FILE* fp) {
    fwrite(STATEMENT_PREFIX_PLUS_SPACE, 1, strlen(STATEMENT_PREFIX_PLUS_SPACE), fp);
    fwrite(pTmp, 1, strlen(pTmp), fp);
    fwrite(STATEMENT_SUFFIX_PLUS_SPACE_AND_EOL, 1, strlen(STATEMENT_SUFFIX_PLUS_SPACE_AND_EOL), fp);
    ++statementOutputIndex;
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

void walkListBackwards(astNode* pListNode, FILE* fp) {
#if REGRESS_1
    printf("\nwalkList(astNode* pListNode=%d)\n", (int)pListNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pListNode == NULL) {
		return;
	}

#if REGRESS_1    
    printf("walkSyntaxTree(pListNode->pLeft=%d, ROOT, 0)\n", (int)pListNode->pLeft);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkSyntaxTree(pListNode->pLeft, "ROOT", 0, fp);

    #if REGRESS_1    
    printf("\nwalkList(pListNode->pNext)=%d\n", (int)pListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkListBackwards(pListNode->pNext, fp);
}

unsigned int countArguments(astNode* pArgNode) {
    unsigned int argCount = 0;
    while (pArgNode != NULL) {
        // This node should either be for a function call or function definition
        //printf("\ncountArguments()");
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
    if (symbolTable[i].fcnDefnLink != VAR_FCN_LINK_UNDEFINED) {
        // Function definition link is defined
        printf(", fcnLink:%d", symbolTable[i].fcnDefnLink);
    }
    char tmp[64];
    sprintf(tmp, "%d %d %d", /*symbolTable[i].name,*/ symbolType, symbolTable[i].val, symbolTable[i].fcnDefnLink);
    if (fpSymbol != NULL) {
        fwrite(SYMBOL_PREFIX_PLUS_SPACE, 1, strlen(SYMBOL_PREFIX_PLUS_SPACE), fpSymbol);
        fwrite(tmp, 1, strlen(tmp), fpSymbol);
        fwrite(SYMBOL_SUFFIX_PLUS_SPACE_AND_EOL, 1, strlen(SYMBOL_SUFFIX_PLUS_SPACE_AND_EOL), fpSymbol);
    }
}

#if 0
// Calculate the symbol table index.
int getSymbolTableIndex(symbolNode* pSymbol) {
    if (pSymbol == NULL) {
        debugAssert(ERR:getSymbolTableIndex():pSymbol == NULL);
    }
    return (pSymbol - symbolTable) / sizeof(symbolTable[0]);
}
#endif

void printOperator(const int value) {
    switch (value) {
    case TILDE:
        printf("~");
        break;
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

