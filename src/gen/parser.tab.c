/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     DNUM = 259,
     PNUM = 260,
     FQUANT = 261,
     SQUANT = 262,
     SSELECT = 263,
     PSELECT = 264,
     COND = 265,
     F_ADD = 266,
     F_SUB = 267,
     F_MOD = 268,
     CMP = 269,
     IDENT = 270,
     UNION = 271,
     INTER = 272,
     DIV = 273,
     RANGE = 274,
     IF = 275,
     XQUANT = 276,
     EXIT = 277,
     EOL = 278,
     UMINUS = 279
   };
#endif
/* Tokens.  */
#define NUM 258
#define DNUM 259
#define PNUM 260
#define FQUANT 261
#define SQUANT 262
#define SSELECT 263
#define PSELECT 264
#define COND 265
#define F_ADD 266
#define F_SUB 267
#define F_MOD 268
#define CMP 269
#define IDENT 270
#define UNION 271
#define INTER 272
#define DIV 273
#define RANGE 274
#define IF 275
#define XQUANT 276
#define EXIT 277
#define EOL 278
#define UMINUS 279




/* Copy the first part of user declarations.  */
#line 2 "src/app/parser.y"


#include <stdlib.h>

#include "parser.tab.h"

#include "deimain.h"
#include "struct.h"
#include "ast.h"
#include "eval.h"



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 15 "src/app/parser.y"
{
	struct ast *a;		/* AST nodes */
	struct symbol *s;	/* identifier names */
	struct value *v;	/* value chain building */
	int i;						/* straight number */
	int fn;						/* enum function */
}
/* Line 193 of yacc.c.  */
#line 165 "./src/gen/parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 178 "./src/gen/parser.tab.c"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   233

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNRULES -- Number of states.  */
#define YYNSTATES  151

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    27,    29,     2,
      32,    33,    26,    24,    40,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,     2,
       2,     2,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    35,    28,     2,     2,     2,     2,     2,
      39,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    38,     2,     2,     2,     2,
       2,     2,     2,    36,    30,    37,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    31,
      34,    38,    40,    42,    44,    48,    52,    56,    60,    64,
      66,    68,    72,    76,    80,    84,    88,    92,    95,    98,
     101,   104,   107,   109,   111,   113,   116,   120,   124,   127,
     133,   139,   144,   147,   151,   156,   162,   164,   166,   168,
     171,   172,   176,   184,   187,   194,   200,   205,   207,   211,
     213,   216,   220,   226,   230,   234,   237,   239,   244,   251,
     256,   261,   265
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    44,    24,    44,    -1,    44,    25,    44,
      -1,    44,    26,    44,    -1,    44,    18,    44,    -1,    44,
      27,    44,    -1,    44,    28,    44,    -1,    44,    14,    44,
      -1,    25,    44,    -1,    32,    44,    33,    -1,     3,    -1,
      15,    -1,    45,    -1,    45,    29,    45,    -1,    45,    30,
      45,    -1,    45,    17,    45,    -1,    45,    16,    45,    -1,
      34,    45,    35,    -1,    46,    -1,    52,    -1,    36,    53,
      37,    -1,    46,    11,    47,    -1,    46,    12,    48,    -1,
      46,    13,    49,    -1,     7,    50,    51,    -1,     3,    50,
      51,    -1,     8,    51,    -1,     3,    51,    -1,     9,    51,
      -1,     7,    50,    -1,     3,    50,    -1,     3,    -1,     8,
      -1,     9,    -1,     5,    38,    -1,     7,    50,    51,    -1,
       3,    50,    51,    -1,    50,    51,    -1,     7,    50,    20,
      10,    51,    -1,     3,    50,    20,    10,    51,    -1,    50,
      20,    10,    51,    -1,     9,    51,    -1,     5,    38,    51,
      -1,     9,    20,    10,    51,    -1,     5,    38,    20,    10,
      51,    -1,     8,    -1,     3,    -1,     6,    -1,     3,    21,
      -1,    -1,     4,    39,     3,    -1,     4,    39,    34,    54,
      19,    54,    35,    -1,    39,     3,    -1,    39,    34,    54,
      19,    54,    35,    -1,     4,    39,    36,    53,    37,    -1,
      39,    36,    53,    37,    -1,    54,    -1,    54,    40,    53,
      -1,     3,    -1,    25,     3,    -1,    55,    44,    23,    -1,
      55,    15,    41,    44,    23,    -1,    55,     1,    23,    -1,
      55,    22,    23,    -1,    55,    23,    -1,    23,    -1,    55,
      42,    44,    23,    -1,    55,    42,    15,    41,    44,    23,
      -1,    55,    42,     1,    23,    -1,    55,    42,    22,    23,
      -1,    55,    42,    23,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    43,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    57,    58,    59,    60,    61,    62,
      65,    66,    67,    68,    69,    72,    73,    74,    75,    76,
      79,    80,    81,    82,    83,    84,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    99,   100,   103,   104,
     105,   108,   109,   110,   111,   112,   113,   116,   117,   120,
     121,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "DNUM", "PNUM", "FQUANT",
  "SQUANT", "SSELECT", "PSELECT", "COND", "F_ADD", "F_SUB", "F_MOD", "CMP",
  "IDENT", "UNION", "INTER", "DIV", "RANGE", "IF", "XQUANT", "EXIT", "EOL",
  "'+'", "'-'", "'*'", "'%'", "'^'", "'&'", "'|'", "UMINUS", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "'s'", "'d'", "','", "':'", "'@'", "$accept",
  "math", "set", "func", "a_args", "s_args", "m_args", "ssel", "fquant",
  "die", "list", "nnum", "line", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    43,    45,    42,    37,    94,    38,
     124,   279,    40,    41,    91,    93,   123,   125,   115,   100,
      44,    58,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    45,    45,    45,    45,    45,    45,
      46,    46,    46,    46,    46,    47,    47,    47,    47,    47,
      48,    48,    48,    48,    48,    48,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    50,    50,    51,    51,
      51,    52,    52,    52,    52,    52,    52,    53,    53,    54,
      54,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     1,     1,     1,     2,     3,     3,     2,     5,
       5,     4,     2,     3,     4,     5,     1,     1,     1,     2,
       0,     3,     7,     2,     6,     5,     4,     1,     3,     1,
       2,     3,     5,     3,     3,     2,     1,     4,     6,     4,
       4,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      72,    66,     0,     1,     0,    11,     0,    12,     0,    65,
       0,     0,     0,     0,     0,     0,     0,    13,    19,    20,
      63,     0,     0,    64,    12,     9,     0,     0,    59,     0,
       0,    57,    53,     0,     0,     0,    12,     0,    71,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,    10,
      18,    60,    21,     0,     0,     0,    69,     0,    70,    67,
       8,     5,     2,     3,     4,     6,     7,    17,    16,    14,
      15,    50,     0,    50,    50,    22,    32,     0,     0,    33,
      34,    23,    47,     0,     0,    46,    50,    24,    50,     0,
       0,    62,    58,     0,    56,     0,    47,    48,    50,    28,
      47,    50,     0,    27,    29,    31,    35,    30,    50,    50,
      50,     0,    42,     0,    38,     0,    55,     0,    68,    49,
      26,    25,     0,    37,     0,    43,     0,    36,    50,    50,
       0,    54,    50,    50,    50,    44,    41,    52,    40,    45,
      39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    16,    17,    18,    85,    91,    97,    98,   109,    19,
      30,    31,     2
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -83
static const yytype_int16 yypact[] =
{
     -20,   -83,    49,   -83,   -14,   -83,   -26,   -22,     1,   -83,
      65,    65,    75,     3,    29,    83,   131,   151,   150,   -83,
     -83,    90,    65,   -83,   -83,   -83,   107,   112,   -83,    22,
      -7,    -9,   -83,     3,     3,    19,     7,    28,   -83,   146,
      65,    65,   -83,    65,    65,    65,    65,    65,    75,    75,
      75,    75,   221,   143,   203,   -83,     3,     3,   161,   -83,
     -83,   -83,   -83,     3,    35,    18,   -83,    65,   -83,   -83,
     191,    30,   165,   165,    30,    30,    30,   -83,   -83,   -83,
     -83,   104,     4,     2,     2,   -83,     4,    21,     4,   -83,
     -83,   -83,     4,    44,     4,   -83,    14,   -83,    15,    47,
      36,   -83,   -83,     3,   -83,   176,    68,   -83,     2,   -83,
     -83,     2,    68,   -83,   -83,   -83,   -83,   -83,   110,   117,
     133,    84,   -83,    85,   -83,     3,   -83,    61,   -83,   -83,
     -83,   -83,    92,   -83,    93,   -83,   108,   -83,     2,     2,
     103,   -83,     2,     2,     2,   -83,   -83,   -83,   -83,   -83,
     -83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -83,     0,    27,   -83,   -83,   -83,   -83,   139,   -82,   -83,
     -30,   -33,   -83
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      64,   113,   114,     1,    65,   112,    28,   110,   107,    20,
      25,    26,    95,    21,   122,    39,   124,   112,   112,    22,
     107,   107,    58,    99,    23,    61,   130,   100,    29,   131,
      62,    63,    32,   102,   121,   123,   133,   135,   137,    27,
      70,    71,    66,    72,    73,    74,    75,    76,    67,     3,
       4,    68,     5,     6,   103,   104,   145,   146,    47,   116,
     148,   149,   150,    33,     7,    34,   125,   105,     5,     6,
     127,     8,     9,   126,    10,    77,    78,    79,    80,     6,
      24,    11,   119,    12,    35,    13,     5,     6,    14,   129,
      10,    15,   140,    55,   138,   139,   141,    11,    36,    12,
       0,    13,   142,   143,    14,    37,    38,   106,    10,    12,
     107,    13,    95,   112,    14,    11,   107,    12,   144,    13,
     112,    40,    14,   107,    56,    41,    57,     0,    48,    49,
     132,    43,    44,    45,    46,    47,   112,   134,   147,   107,
      59,    50,    51,     0,     0,    40,    86,    60,    87,    41,
      88,    89,    90,   136,    42,    43,    44,    45,    46,    47,
      40,    52,    53,    54,    41,     0,     0,    48,    49,    69,
      43,    44,    45,    46,    47,    40,     0,     0,     0,    41,
      50,    51,     0,    41,   101,    43,    44,    45,    46,    47,
      40,    45,    46,    47,    41,     0,     0,     0,     0,   128,
      43,    44,    45,    46,    47,    -1,    92,     0,    93,    41,
      94,    95,    96,     0,     0,    43,    44,    45,    46,    47,
     108,   111,     0,     0,    81,   115,     0,   117,    82,    83,
      84,   118,     0,   120
};

static const yytype_int16 yycheck[] =
{
      33,    83,    84,    23,    34,     3,     3,     3,     6,    23,
      10,    11,     8,    39,    96,    15,    98,     3,     3,    41,
       6,     6,    22,    56,    23,     3,   108,    57,    25,   111,
      37,    40,     3,    63,    20,    20,   118,   119,   120,    12,
      40,    41,    23,    43,    44,    45,    46,    47,    41,     0,
       1,    23,     3,     4,    19,    37,   138,   139,    28,    38,
     142,   143,   144,    34,    15,    36,    19,    67,     3,     4,
     103,    22,    23,    37,    25,    48,    49,    50,    51,     4,
      15,    32,    38,    34,     1,    36,     3,     4,    39,    21,
      25,    42,   125,     3,    10,    10,    35,    32,    15,    34,
      -1,    36,    10,    10,    39,    22,    23,     3,    25,    34,
       6,    36,     8,     3,    39,    32,     6,    34,    10,    36,
       3,    14,    39,     6,    34,    18,    36,    -1,    16,    17,
      20,    24,    25,    26,    27,    28,     3,    20,    35,     6,
      33,    29,    30,    -1,    -1,    14,     3,    35,     5,    18,
       7,     8,     9,    20,    23,    24,    25,    26,    27,    28,
      14,    11,    12,    13,    18,    -1,    -1,    16,    17,    23,
      24,    25,    26,    27,    28,    14,    -1,    -1,    -1,    18,
      29,    30,    -1,    18,    23,    24,    25,    26,    27,    28,
      14,    26,    27,    28,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    14,     3,    -1,     5,    18,
       7,     8,     9,    -1,    -1,    24,    25,    26,    27,    28,
      81,    82,    -1,    -1,     3,    86,    -1,    88,     7,     8,
       9,    92,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    23,    55,     0,     1,     3,     4,    15,    22,    23,
      25,    32,    34,    36,    39,    42,    44,    45,    46,    52,
      23,    39,    41,    23,    15,    44,    44,    45,     3,    25,
      53,    54,     3,    34,    36,     1,    15,    22,    23,    44,
      14,    18,    23,    24,    25,    26,    27,    28,    16,    17,
      29,    30,    11,    12,    13,     3,    34,    36,    44,    33,
      35,     3,    37,    40,    54,    53,    23,    41,    23,    23,
      44,    44,    44,    44,    44,    44,    44,    45,    45,    45,
      45,     3,     7,     8,     9,    47,     3,     5,     7,     8,
       9,    48,     3,     5,     7,     8,     9,    49,    50,    54,
      53,    23,    53,    19,    37,    44,     3,     6,    50,    51,
       3,    50,     3,    51,    51,    50,    38,    50,    50,    38,
      50,    20,    51,    20,    51,    19,    37,    54,    23,    21,
      51,    51,    20,    51,    20,    51,    20,    51,    10,    10,
      54,    35,    10,    10,    10,    51,    51,    35,    51,    51,
      51
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
#line 43 "src/app/parser.y"
    { (yyval.a) = newAst('+', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 3:
#line 44 "src/app/parser.y"
    { (yyval.a) = newAst('-', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 4:
#line 45 "src/app/parser.y"
    { (yyval.a) = newAst('*', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 5:
#line 46 "src/app/parser.y"
    { (yyval.a) = newAst(DIV, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 6:
#line 47 "src/app/parser.y"
    { (yyval.a) = newAst('%', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 7:
#line 48 "src/app/parser.y"
    { (yyval.a) = newAst('^', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 8:
#line 49 "src/app/parser.y"
    { (yyval.a) = newCmp((yyvsp[(2) - (3)].fn), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 9:
#line 50 "src/app/parser.y"
    { (yyval.a) = newAst('M', (yyvsp[(2) - (2)].a), NULL); ;}
    break;

  case 10:
#line 51 "src/app/parser.y"
    { (yyval.a) = (yyvsp[(2) - (3)].a); ;}
    break;

  case 11:
#line 52 "src/app/parser.y"
    { (yyval.a) = newNatint((yyvsp[(1) - (1)].i)); ;}
    break;

  case 12:
#line 53 "src/app/parser.y"
    { (yyval.a) = newSymcall((yyvsp[(1) - (1)].s)); ;}
    break;

  case 13:
#line 54 "src/app/parser.y"
    { (yyval.a) = newAst('S', (yyvsp[(1) - (1)].a), NULL); ;}
    break;

  case 14:
#line 57 "src/app/parser.y"
    { (yyval.a) = newAst('&', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 15:
#line 58 "src/app/parser.y"
    { (yyval.a) = newAst('|', (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 16:
#line 59 "src/app/parser.y"
    { (yyval.a) = newAst(INTER, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 17:
#line 60 "src/app/parser.y"
    { (yyval.a) = newAst(UNION, (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 18:
#line 61 "src/app/parser.y"
    { (yyval.a) = (yyvsp[(2) - (3)].a) ;}
    break;

  case 19:
#line 62 "src/app/parser.y"
    { (yyval.a) = newAst('Z', (yyvsp[(1) - (1)].a), NULL); ;}
    break;

  case 20:
#line 65 "src/app/parser.y"
    { (yyval.a) = newAst('R', (yyvsp[(1) - (1)].a), NULL); ;}
    break;

  case 21:
#line 66 "src/app/parser.y"
    { (yyval.a) = newSetres((yyvsp[(2) - (3)].v)); ;}
    break;

  case 22:
#line 67 "src/app/parser.y"
    { (yyval.a) = newFunc((yyvsp[(2) - (3)].fn), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 23:
#line 68 "src/app/parser.y"
    { (yyval.a) = newFunc((yyvsp[(2) - (3)].fn), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 24:
#line 69 "src/app/parser.y"
    { (yyval.a) = newFunc((yyvsp[(2) - (3)].fn), (yyvsp[(1) - (3)].a), (yyvsp[(3) - (3)].a)); ;}
    break;

  case 25:
#line 72 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(3) - (3)].i), (yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].i), C_none); ;}
    break;

  case 26:
#line 73 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(3) - (3)].i), (yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].i), C_none); ;}
    break;

  case 27:
#line 74 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i),  1, C_none); ;}
    break;

  case 28:
#line 75 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i),  1, C_none); ;}
    break;

  case 29:
#line 76 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i),  1, C_none); ;}
    break;

  case 30:
#line 79 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i), C_none); ;}
    break;

  case 31:
#line 80 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i), C_none); ;}
    break;

  case 32:
#line 81 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(1) - (1)].i),  1, C_none); ;}
    break;

  case 33:
#line 82 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(1) - (1)].i),  1, C_none); ;}
    break;

  case 34:
#line 83 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(1) - (1)].i), -1, C_none); ;}
    break;

  case 35:
#line 84 "src/app/parser.y"
    { (yyval.a) = newFargs( 1, (yyvsp[(1) - (2)].i), -1, C_none); ;}
    break;

  case 36:
#line 87 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(3) - (3)].i), (yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].i), C_none); ;}
    break;

  case 37:
#line 88 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(3) - (3)].i), (yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].i), C_none); ;}
    break;

  case 38:
#line 89 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i),  1, C_none); ;}
    break;

  case 39:
#line 90 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(5) - (5)].i), (yyvsp[(2) - (5)].i), (yyvsp[(1) - (5)].i), (yyvsp[(4) - (5)].fn)); ;}
    break;

  case 40:
#line 91 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(5) - (5)].i), (yyvsp[(2) - (5)].i), (yyvsp[(1) - (5)].i), (yyvsp[(4) - (5)].fn)); ;}
    break;

  case 41:
#line 92 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(4) - (4)].i), (yyvsp[(1) - (4)].i),  1, (yyvsp[(3) - (4)].fn)); ;}
    break;

  case 42:
#line 93 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(2) - (2)].i), (yyvsp[(1) - (2)].i), -1, C_none); ;}
    break;

  case 43:
#line 94 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].i), -1, C_none); ;}
    break;

  case 44:
#line 95 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(4) - (4)].i), (yyvsp[(1) - (4)].i), -1, (yyvsp[(3) - (4)].fn)); ;}
    break;

  case 45:
#line 96 "src/app/parser.y"
    { (yyval.a) = newFargs((yyvsp[(5) - (5)].i), (yyvsp[(1) - (5)].i), -1, (yyvsp[(4) - (5)].fn)); ;}
    break;

  case 46:
#line 99 "src/app/parser.y"
    { (yyval.i) = (yyvsp[(1) - (1)].i); ;}
    break;

  case 47:
#line 100 "src/app/parser.y"
    { (yyval.i) = (yyvsp[(1) - (1)].i); ;}
    break;

  case 48:
#line 103 "src/app/parser.y"
    { (yyval.i) = (yyvsp[(1) - (1)].i); ;}
    break;

  case 49:
#line 104 "src/app/parser.y"
    { (yyval.i) = (yyvsp[(1) - (2)].i); ;}
    break;

  case 50:
#line 105 "src/app/parser.y"
    { (yyval.i) =  1; ;}
    break;

  case 51:
#line 108 "src/app/parser.y"
    { (yyval.a) = newNatdie((yyvsp[(1) - (3)].i),  1, (yyvsp[(3) - (3)].i)); ;}
    break;

  case 52:
#line 109 "src/app/parser.y"
    { (yyval.a) = newNatdie((yyvsp[(1) - (7)].i), (yyvsp[(4) - (7)].i), (yyvsp[(6) - (7)].i)); ;}
    break;

  case 53:
#line 110 "src/app/parser.y"
    { (yyval.a) = newNatdie( 1,  1, (yyvsp[(2) - (2)].i)); ;}
    break;

  case 54:
#line 111 "src/app/parser.y"
    { (yyval.a) = newNatdie( 1, (yyvsp[(3) - (6)].i), (yyvsp[(5) - (6)].i)); ;}
    break;

  case 55:
#line 112 "src/app/parser.y"
    { (yyval.a) = newSetdie((yyvsp[(1) - (5)].i), (yyvsp[(4) - (5)].v)); ;}
    break;

  case 56:
#line 113 "src/app/parser.y"
    { (yyval.a) = newSetdie( 1, (yyvsp[(3) - (4)].v)); ;}
    break;

  case 57:
#line 116 "src/app/parser.y"
    { (yyval.v) = newValue((yyvsp[(1) - (1)].i), NULL); ;}
    break;

  case 58:
#line 117 "src/app/parser.y"
    { (yyval.v) = newValue((yyvsp[(1) - (3)].i), (yyvsp[(3) - (3)].v)); ;}
    break;

  case 59:
#line 120 "src/app/parser.y"
    { (yyval.i) = (yyvsp[(1) - (1)].i); ;}
    break;

  case 60:
#line 121 "src/app/parser.y"
    { (yyval.i) = -(yyvsp[(2) - (2)].i); ;}
    break;

  case 61:
#line 124 "src/app/parser.y"
    { printAst((yyvsp[(2) - (3)].a)); struct result *r = eval((yyvsp[(2) - (3)].a)); printf(" = %d\n", r->integer); freeResult(&r); freeAst(&(yyvsp[(2) - (3)].a)); printf("\nparsed!\n> "); ;}
    break;

  case 62:
#line 125 "src/app/parser.y"
    { setsym((yyvsp[(2) - (5)].s), (yyvsp[(4) - (5)].a)); printf("saved!\n> "); ;}
    break;

  case 63:
#line 126 "src/app/parser.y"
    { printf("error!\n> "); ;}
    break;

  case 64:
#line 127 "src/app/parser.y"
    { printf("closing!\n"); exit(0); ;}
    break;

  case 65:
#line 128 "src/app/parser.y"
    { printf("> "); ;}
    break;

  case 66:
#line 129 "src/app/parser.y"
    {  ;}
    break;

  case 67:
#line 130 "src/app/parser.y"
    { struct result *r = eval((yyvsp[(3) - (4)].a)); freeResult(&r); freeAst(&(yyvsp[(3) - (4)].a)); ;}
    break;

  case 68:
#line 131 "src/app/parser.y"
    { setsym((yyvsp[(3) - (6)].s), (yyvsp[(5) - (6)].a)); ;}
    break;

  case 69:
#line 132 "src/app/parser.y"
    { printf("silent error!\n") ;}
    break;

  case 70:
#line 133 "src/app/parser.y"
    { exit(0); ;}
    break;

  case 71:
#line 134 "src/app/parser.y"
    {  ;}
    break;

  case 72:
#line 135 "src/app/parser.y"
    {  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1870 "./src/gen/parser.tab.c"
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
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
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


#line 138 "src/app/parser.y"


