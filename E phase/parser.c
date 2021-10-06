/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

	#include <stdio.h>
	#include <stdlib.h>
	#include "vmLibrary.h"

	int yyerror(char *yaccProvideMessage);
	int yylex(void);
	extern int yylineno;
	extern char* yytext;
	extern char* yyval;
	extern FILE* yyin;
	extern FILE * yyout;
	
	int loopCounter=0;
	int FunDeclared=0;
	bool ArrayDeclaration=false;
	int scope=0;
	
	PatchList *ContinueList=NULL;
	PatchList *BreakList=NULL;
	NestLoops *LoopListStack=NULL;

	NestLoops * pushNestedLoop(){
		NestLoops *tmp=(NestLoops*)malloc(sizeof(NestLoops));
		tmp->ContinueList=ContinueList;
		tmp->BreakList=BreakList;
		if(LoopListStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=LoopListStack;
		}
		return tmp;
	}
	
	myStack *loopStack=NULL;

	myStack *pushloop()
	{
		myStack *tmp=(myStack*)malloc(sizeof(myStack));
		tmp->myNumb=loopCounter;
		if(loopStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=loopStack;
		}
		return tmp;
	}

	int poploop()
	{	
		myStack *tmp=loopStack;
		int savedOffset;
		if(tmp==NULL) return 0;
		else{
			savedOffset=loopStack->myNumb;
			tmp=tmp->next;
			free(loopStack);
			loopStack=tmp;
			return savedOffset;
		}
	}
	myStack *OffsetStack=NULL;

	myStack *pushOffset(int n)
	{
		myStack *tmp=(myStack*)malloc(sizeof(myStack));
		tmp->myNumb=n;
		if(OffsetStack==NULL){
			tmp->next=NULL;
		}else{
			tmp->next=OffsetStack;
		}
		return tmp;
	}

	int popOffset()
	{	
		myStack *tmp=OffsetStack;
		int savedOffset;
		if(tmp==NULL) return 0;
		else{
			savedOffset=OffsetStack->myNumb;
			tmp=tmp->next;
			free(OffsetStack);
			OffsetStack=tmp;
			return savedOffset;
		}
	}

	void LvalueAction(char *name,int line,char *error)
	{//ta actions poy simbainoyne otan sinantisoyme ena lvalue
		if(isLibFunc(name)) {printf("Error:(line %d)Can not %s %s[Library Function]\n",line,error,name);
					ErrorCounter++;
		}
		else if(ReturnNode(HashTable,name,scope)!=NULL){
			if(ReturnNode(HashTable,name,scope)->type==USERFUNC){
				printf("Error:(line %d)Can not %s %s[User Function]\n",line,error,name);
				ErrorCounter++;
			}
		}
	}

#line 169 "parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    FOR = 261,
    FUNCTION = 262,
    RETURN = 263,
    BREAK = 264,
    CONTINUE = 265,
    AND = 266,
    NOT = 267,
    OR = 268,
    LOCAL = 269,
    TRUE = 270,
    FALSE = 271,
    NIL = 272,
    ASSIGN = 273,
    PLUS = 274,
    MINUS = 275,
    MULTI = 276,
    DIV = 277,
    MOD = 278,
    EQUAL = 279,
    NOTEQUAL = 280,
    INCR = 281,
    DECR = 282,
    LESS = 283,
    GREAT = 284,
    GREATEQUAL = 285,
    LESSEQUAL = 286,
    LPAR = 287,
    RPAR = 288,
    LBRACKET = 289,
    RBRACKET = 290,
    LANGLE = 291,
    RANGLE = 292,
    SEMICOLON = 293,
    COMMA = 294,
    COLON = 295,
    DCOLON = 296,
    DOT = 297,
    DDOT = 298,
    ID = 299,
    INT = 300,
    REAL = 301,
    YYSTRING = 302,
    UMINUS = 303
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define WHILE 260
#define FOR 261
#define FUNCTION 262
#define RETURN 263
#define BREAK 264
#define CONTINUE 265
#define AND 266
#define NOT 267
#define OR 268
#define LOCAL 269
#define TRUE 270
#define FALSE 271
#define NIL 272
#define ASSIGN 273
#define PLUS 274
#define MINUS 275
#define MULTI 276
#define DIV 277
#define MOD 278
#define EQUAL 279
#define NOTEQUAL 280
#define INCR 281
#define DECR 282
#define LESS 283
#define GREAT 284
#define GREATEQUAL 285
#define LESSEQUAL 286
#define LPAR 287
#define RPAR 288
#define LBRACKET 289
#define RBRACKET 290
#define LANGLE 291
#define RANGLE 292
#define SEMICOLON 293
#define COMMA 294
#define COLON 295
#define DCOLON 296
#define DOT 297
#define DDOT 298
#define ID 299
#define INT 300
#define REAL 301
#define YYSTRING 302
#define UMINUS 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 133 "parser.y" /* yacc.c:355  */

	char* strValue;
	int intValue;
	double realValue;
	struct expr* expression;
	struct ST_node* sym;
	struct indexedexprs *indexexpr; 
	struct M *Mquad;

#line 315 "parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 332 "parser.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   528

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  198

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   151,   151,   153,   154,   156,   160,   163,   166,   169,
     172,   181,   190,   193,   194,   199,   203,   209,   215,   221,
     227,   233,   242,   251,   260,   269,   278,   287,   298,   310,
     314,   317,   325,   335,   351,   351,   367,   382,   382,   398,
     402,   402,   419,   422,   423,   426,   430,   434,   454,   468,
     476,   478,   481,   488,   489,   493,   495,   498,   506,   512,
     515,   519,   525,   531,   535,   539,   544,   548,   563,   577,
     582,   586,   590,   594,   594,   595,   595,   600,   601,   603,
     622,   622,   628,   629,   630,   637,   647,   650,   653,   656,
     659,   662,   666,   666,   678,   680,   680,   693,   695,   701,
     706,   709,   714,   723,   728,   753,   757,   759,   759,   772,
     799,   807
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "TRUE",
  "FALSE", "NIL", "ASSIGN", "PLUS", "MINUS", "MULTI", "DIV", "MOD",
  "EQUAL", "NOTEQUAL", "INCR", "DECR", "LESS", "GREAT", "GREATEQUAL",
  "LESSEQUAL", "LPAR", "RPAR", "LBRACKET", "RBRACKET", "LANGLE", "RANGLE",
  "SEMICOLON", "COMMA", "COLON", "DCOLON", "DOT", "DDOT", "ID", "INT",
  "REAL", "YYSTRING", "UMINUS", "$accept", "program", "statements",
  "statement", "expr", "term", "$@1", "$@2", "assignexpr", "$@3",
  "primary", "lvalue", "tableitem", "member", "call", "callsuffix",
  "normcall", "methodcall", "elist", "listexpr", "objectdef", "indexed",
  "indexedelemlist", "indexedelem", "block", "$@4", "$@5", "funcname",
  "funcprefix", "funcargs", "$@6", "funcstart", "funcend", "funcbody",
  "funcdef", "const", "idlist", "$@7", "idlistlist", "$@8", "ifprefix",
  "elseprefix", "ifstmt", "whilestart", "whilecond", "whilestmt", "N", "M",
  "forprefix", "$@9", "forstmt", "returnstmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

#define YYPACT_NINF -169

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-169)))

#define YYTABLE_NINF -74

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-74)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     124,   -10,  -169,  -169,   -38,     4,    -9,     1,   213,    -3,
    -169,  -169,  -169,   213,    -4,    -4,   167,   190,    22,  -169,
      21,  -169,  -169,  -169,  -169,    34,  -169,   124,   263,  -169,
    -169,  -169,   334,  -169,  -169,   -25,  -169,  -169,    37,  -169,
    -169,   124,  -169,    54,  -169,  -169,  -169,  -169,   213,    56,
    -169,  -169,  -169,   284,  -169,  -169,  -169,  -169,  -169,    59,
      55,   -25,    55,   380,    66,   213,   242,    68,    72,    62,
      75,   124,  -169,  -169,  -169,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,  -169,   213,
     213,    61,    65,    87,    94,   104,  -169,  -169,  -169,   213,
     213,    84,  -169,  -169,   138,   213,   124,   213,   401,   213,
    -169,   110,  -169,   114,    95,   213,  -169,  -169,  -169,   111,
    -169,  -169,   124,   471,   458,    70,    70,  -169,  -169,  -169,
     484,   484,   497,   497,   497,   497,   115,    33,  -169,   117,
    -169,  -169,   213,   121,   359,  -169,   113,   119,  -169,  -169,
     124,   422,  -169,   128,  -169,   125,   114,   213,   213,   242,
      62,   127,  -169,  -169,   213,   443,  -169,  -169,  -169,   133,
    -169,  -169,  -169,  -169,  -169,   134,   326,  -169,  -169,  -169,
     139,   136,  -169,  -169,   124,   213,  -169,  -169,  -169,   129,
    -169,  -169,   305,  -169,  -169,  -169,   136,  -169
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,   102,   107,    78,     0,     0,     0,     0,     0,
      90,    91,    89,     0,     0,     0,     0,    64,    75,    14,
       0,    47,    86,    87,    88,     0,     2,     4,     0,    29,
      15,    39,    42,    53,    50,    43,    44,    12,     0,    13,
      46,     0,     6,     0,     7,   105,     8,     9,     0,     0,
      77,    79,   110,     0,    10,    11,    32,    48,    31,     0,
      33,     0,    36,     0,     0,     0,    66,     0,     0,    71,
       0,     0,    49,     1,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,    64,
       0,     0,     0,     0,     0,     0,    57,    59,    60,    64,
       0,     0,    80,    82,   100,     0,     0,    64,     0,    64,
     111,     0,    30,    45,     0,     0,    63,    67,    68,     0,
      69,    74,     4,    27,    28,    16,    17,    18,    19,    20,
      25,    26,    23,    21,    22,    24,     0,     0,    51,     0,
      35,    38,     0,     0,     0,    54,    94,     0,    85,    99,
       0,     0,   104,     0,    98,     0,     0,    64,     0,    66,
      71,     0,    61,    52,    64,    41,    56,    55,    92,     0,
      83,   101,   103,   105,   106,     0,     0,    65,    70,    76,
       0,    97,    81,    84,     0,     0,    58,    72,    62,     0,
      93,   105,     0,    95,   109,   108,    97,    96
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,   -26,   -39,    -5,  -169,  -169,  -169,  -169,  -169,
    -169,    -1,  -169,  -169,    12,  -169,  -169,  -169,   -74,    17,
    -169,  -169,    18,    58,    38,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,   -12,  -169,  -169,  -169,    -8,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -168,  -169,  -169,  -169,
    -169,  -169
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    93,    94,    30,    95,
      31,    32,    33,    34,    35,    96,    97,    98,    67,   116,
      36,    68,   120,    69,    37,    70,    71,    51,    38,   103,
     146,   147,   183,   148,    39,    40,   169,   181,   190,   196,
      41,   150,    42,    43,   106,    44,   107,   185,    45,    49,
      46,    47
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,    74,   104,    56,    64,   184,    50,    99,    58,   100,
       9,    63,    66,    60,    62,   136,     8,   101,     9,    10,
      11,    12,    48,   194,    13,   143,    61,    61,    59,    54,
      14,    15,   122,   153,    73,   155,    16,    20,    17,    55,
      21,    57,    52,   108,    75,    20,    76,   111,    21,    22,
      23,    24,    77,    78,    79,    80,    81,    82,    83,   -73,
     114,    84,    85,    86,    87,    72,     4,   152,   163,   102,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   175,    66,   137,   105,    89,   109,    90,
     180,    79,    80,    81,    66,   144,   161,    91,    92,   113,
     151,   119,    66,   117,    66,   138,    75,   118,    76,   139,
     159,   171,   121,   140,    77,    78,    79,    80,    81,    82,
      83,   141,   142,    84,    85,    86,    87,     1,   145,     2,
       3,     4,     5,     6,     7,   158,     8,   165,     9,    10,
      11,    12,   149,   156,    13,   191,   157,    65,   162,   164,
      14,    15,    66,   176,   166,    18,    16,   168,    17,    66,
      18,   173,    19,   174,   179,    20,   182,   186,    21,    22,
      23,    24,   188,   193,     4,   189,   177,   160,   178,     8,
     192,     9,    10,    11,    12,   170,     0,    13,   197,     0,
       0,     0,     0,    14,    15,     0,     0,     0,     0,    16,
       0,    17,     8,     0,     9,    10,    11,    12,    20,     0,
      13,    21,    22,    23,    24,     0,    14,    15,     0,     0,
       0,     0,    16,     0,    17,     8,    65,     9,    10,    11,
      12,    20,     0,    13,    21,    22,    23,    24,     0,    14,
      15,     0,     0,     0,     0,    16,     0,    17,     0,     0,
       0,     0,     0,    75,    20,    76,     0,    21,    22,    23,
      24,    77,    78,    79,    80,    81,    82,    83,     0,     0,
      84,    85,    86,    87,    75,     0,    76,     0,     0,     0,
       0,   115,    77,    78,    79,    80,    81,    82,    83,     0,
       0,    84,    85,    86,    87,    75,     0,    76,     0,     0,
       0,    88,     0,    77,    78,    79,    80,    81,    82,    83,
       0,     0,    84,    85,    86,    87,    75,     0,    76,     0,
       0,     0,   110,     0,    77,    78,    79,    80,    81,    82,
      83,     0,     0,    84,    85,    86,    87,    75,     0,    76,
       0,     0,     0,   195,     0,    77,    78,    79,    80,    81,
      82,    83,   -40,     0,    84,    85,    86,    87,     0,     0,
     -34,   -37,     0,   187,     0,     0,    89,     0,    90,     0,
      75,     0,    76,     0,     0,     0,    91,    92,    77,    78,
      79,    80,    81,    82,    83,     0,     0,    84,    85,    86,
      87,    75,     0,    76,   167,     0,     0,     0,     0,    77,
      78,    79,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    75,   112,    76,     0,     0,     0,     0,     0,
      77,    78,    79,    80,    81,    82,    83,     0,     0,    84,
      85,    86,    87,    75,   154,    76,     0,     0,     0,     0,
       0,    77,    78,    79,    80,    81,    82,    83,     0,     0,
      84,    85,    86,    87,    75,   172,    76,     0,     0,     0,
       0,     0,    77,    78,    79,    80,    81,    82,    83,    75,
       0,    84,    85,    86,    87,     0,     0,    77,    78,    79,
      80,    81,    82,    83,     0,     0,    84,    85,    86,    87,
      77,    78,    79,    80,    81,    82,    83,     0,     0,    84,
      85,    86,    87,    77,    78,    79,    80,    81,   -74,   -74,
       0,     0,    84,    85,    86,    87,    77,    78,    79,    80,
      81,     0,     0,     0,     0,   -74,   -74,   -74,   -74
};

static const yytype_int16 yycheck[] =
{
       5,    27,    41,     8,    16,   173,    44,    32,    13,    34,
      14,    16,    17,    14,    15,    89,    12,    42,    14,    15,
      16,    17,    32,   191,    20,    99,    14,    15,    32,    38,
      26,    27,    71,   107,     0,   109,    32,    41,    34,    38,
      44,    44,    38,    48,    11,    41,    13,    59,    44,    45,
      46,    47,    19,    20,    21,    22,    23,    24,    25,    37,
      65,    28,    29,    30,    31,    44,     7,   106,    35,    32,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   157,    89,    90,    32,    32,    32,    34,
     164,    21,    22,    23,    99,   100,   122,    42,    43,    33,
     105,    39,   107,    35,   109,    44,    11,    35,    13,    44,
     115,   150,    37,    26,    19,    20,    21,    22,    23,    24,
      25,    27,    18,    28,    29,    30,    31,     3,    44,     5,
       6,     7,     8,     9,    10,    40,    12,   142,    14,    15,
      16,    17,     4,    33,    20,   184,    32,    36,    33,    32,
      26,    27,   157,   158,    33,    36,    32,    44,    34,   164,
      36,    33,    38,    38,    37,    41,    33,    33,    44,    45,
      46,    47,    33,    44,     7,    39,   159,   119,   160,    12,
     185,    14,    15,    16,    17,   147,    -1,    20,   196,    -1,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    32,
      -1,    34,    12,    -1,    14,    15,    16,    17,    41,    -1,
      20,    44,    45,    46,    47,    -1,    26,    27,    -1,    -1,
      -1,    -1,    32,    -1,    34,    12,    36,    14,    15,    16,
      17,    41,    -1,    20,    44,    45,    46,    47,    -1,    26,
      27,    -1,    -1,    -1,    -1,    32,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    11,    41,    13,    -1,    44,    45,    46,
      47,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    11,    -1,    13,    -1,    -1,    -1,
      -1,    39,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    30,    31,    11,    -1,    13,    -1,    -1,
      -1,    38,    -1,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    11,    -1,    13,    -1,
      -1,    -1,    38,    -1,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    29,    30,    31,    11,    -1,    13,
      -1,    -1,    -1,    38,    -1,    19,    20,    21,    22,    23,
      24,    25,    18,    -1,    28,    29,    30,    31,    -1,    -1,
      26,    27,    -1,    37,    -1,    -1,    32,    -1,    34,    -1,
      11,    -1,    13,    -1,    -1,    -1,    42,    43,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    11,    -1,    13,    35,    -1,    -1,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    28,    29,
      30,    31,    11,    33,    13,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    11,    33,    13,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    11,    33,    13,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    11,
      -1,    28,    29,    30,    31,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     6,     7,     8,     9,    10,    12,    14,
      15,    16,    17,    20,    26,    27,    32,    34,    36,    38,
      41,    44,    45,    46,    47,    50,    51,    52,    53,    54,
      57,    59,    60,    61,    62,    63,    69,    73,    77,    83,
      84,    89,    91,    92,    94,    97,    99,   100,    32,    98,
      44,    76,    38,    53,    38,    38,    53,    44,    53,    32,
      60,    63,    60,    53,    83,    36,    53,    67,    70,    72,
      74,    75,    44,     0,    51,    11,    13,    19,    20,    21,
      22,    23,    24,    25,    28,    29,    30,    31,    38,    32,
      34,    42,    43,    55,    56,    58,    64,    65,    66,    32,
      34,    42,    32,    78,    52,    32,    93,    95,    53,    32,
      38,    83,    33,    33,    53,    39,    68,    35,    35,    39,
      71,    37,    52,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    67,    53,    44,    44,
      26,    27,    18,    67,    53,    44,    79,    80,    82,     4,
      90,    53,    52,    67,    33,    67,    33,    32,    40,    53,
      72,    51,    33,    35,    32,    53,    33,    35,    44,    85,
      73,    52,    33,    33,    38,    67,    53,    68,    71,    37,
      67,    86,    33,    81,    95,    96,    33,    37,    33,    39,
      87,    52,    53,    44,    95,    38,    88,    87
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    51,    51,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      54,    54,    54,    54,    55,    54,    54,    56,    54,    54,
      58,    57,    59,    59,    59,    59,    59,    60,    60,    60,
      60,    61,    61,    62,    62,    62,    63,    63,    63,    64,
      64,    65,    66,    67,    67,    68,    68,    69,    69,    70,
      71,    71,    72,    74,    73,    75,    73,    76,    76,    77,
      79,    78,    80,    81,    82,    83,    84,    84,    84,    84,
      84,    84,    86,    85,    85,    88,    87,    87,    89,    90,
      91,    91,    92,    93,    94,    95,    96,    98,    97,    99,
     100,   100
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     2,     2,     2,     0,     3,     2,     0,     3,     1,
       0,     4,     1,     1,     1,     3,     1,     1,     2,     2,
       1,     3,     4,     1,     3,     4,     4,     2,     6,     1,
       1,     3,     5,     2,     0,     3,     0,     3,     3,     2,
       3,     0,     5,     0,     3,     0,     5,     1,     0,     2,
       0,     4,     0,     0,     3,     3,     1,     1,     1,     1,
       1,     1,     0,     3,     0,     0,     4,     0,     4,     1,
       2,     4,     1,     3,     3,     0,     0,     0,     8,     7,
       2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 2:
#line 151 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: program->statements(Successful match)\n",yylineno);}
#line 1623 "parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 153 "parser.y" /* yacc.c:1646  */
    {}
#line 1629 "parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 154 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statements->empty rule\n",yylineno);}
#line 1635 "parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 156 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->expr;\n",yylineno);
				(yyval.expression)=(yyvsp[-1].expression);
				resetTmp();			
				}
#line 1644 "parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 160 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->ifstatement\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);				
				}
#line 1652 "parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 163 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->whilestatement\n",yylineno);
				(yyval.expression)=NULL;				
				}
#line 1660 "parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 166 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->forstatement\n",yylineno);
				(yyval.expression)=NULL;				
				}
#line 1668 "parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 169 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->returnstatement\n",yylineno);
				(yyval.expression)=newexpr_constnil();			
				}
#line 1676 "parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 172 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->break;\n",yylineno);
							if(loopCounter==0){
								printf("Error:(line %d) break not inside loop %s\n",yylineno,yytext);
								ErrorCounter++;
							}else{
								BreakList=add_node(BreakList,nextquadlabel());
								emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
							}
							}
#line 1690 "parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 181 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->continue;\n",yylineno);
							if(loopCounter==0){
								printf("Error:(line %d) continue not inside loop %s\n",yylineno,yytext);
								ErrorCounter++;
							}else{
								ContinueList=add_node(ContinueList,nextquadlabel());
								emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
							}
							}
#line 1704 "parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 190 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->block\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);
				}
#line 1712 "parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 193 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->funcdef\n",yylineno);}
#line 1718 "parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 194 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: statement->;\n",yylineno);
				(yyval.expression)=NULL;
				resetTmp();	
				}
#line 1727 "parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 199 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->assignexpr\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);
				(yyval.expression)->type=assignexpr_e;				
				}
#line 1736 "parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 203 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr+expr\n",yylineno);
				(yyval.expression)=newexpr(arithmexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(add,(yyval.expression),(yyvsp[-2].expression),(yyvsp[0].expression),-1,nextquadlabel());
				}
#line 1747 "parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 209 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr-expr\n",yylineno);
				(yyval.expression)=newexpr(arithmexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(sub,(yyval.expression),(yyvsp[-2].expression),(yyvsp[0].expression),-1,nextquadlabel());	
				}
#line 1758 "parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 215 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr*expr\n",yylineno);
				(yyval.expression)=newexpr(arithmexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(mul,(yyval.expression),(yyvsp[-2].expression),(yyvsp[0].expression),-1,nextquadlabel());
				}
#line 1769 "parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 221 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr/expr\n",yylineno);
				(yyval.expression)=newexpr(arithmexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(divv,(yyval.expression),(yyvsp[-2].expression),(yyvsp[0].expression),-1,nextquadlabel());
				}
#line 1780 "parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 227 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->exprMODexpr\n",yylineno);
				(yyval.expression)=newexpr(arithmexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(mod,(yyval.expression),(yyvsp[-2].expression),(yyvsp[0].expression),-1,nextquadlabel());
				}
#line 1791 "parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 233 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr>expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_greater,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1805 "parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 242 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr>=expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_greatereq,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1819 "parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 251 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr<expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_less,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1833 "parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 260 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr<=expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_lesseq,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1847 "parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 269 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: xpr->expr==expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_eq,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1861 "parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 278 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->expr!=expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				emit(if_noteq,NULL,(yyvsp[-2].expression),(yyvsp[0].expression),nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,nextquadlabel());
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,nextquadlabel());
				}
#line 1875 "parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 287 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->exprANDexpr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
				//emit(and,$$,$1,$3,-1,nextquadlabel());//fixed  the and for the Tcode
				emit(if_eq,newexpr_constbool(false),(yyvsp[-2].expression),NULL,nextquadlabel()+4,nextquadlabel());
				emit(if_eq,newexpr_constbool(false),(yyvsp[0].expression),NULL,nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,-1);
				}
#line 1891 "parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 298 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->exprORexpr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue=(yyval.expression)->sym->name;
			
				//emit(or,$$,$1,$3,-1,nextquadlabel());//fixed the or for the Tcode
				emit(if_eq,newexpr_constbool(true),(yyvsp[-2].expression),NULL,nextquadlabel()+4,nextquadlabel());
				emit(if_eq,newexpr_constbool(true),(yyvsp[0].expression),NULL,nextquadlabel()+3,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,-1);
				}
#line 1908 "parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 310 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: expr->term\n",yylineno);
				(yyval.expression) = (yyvsp[0].expression);
				}
#line 1916 "parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 314 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->(expr)\n",yylineno);
				(yyval.expression)=(yyvsp[-1].expression);			
				}
#line 1924 "parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 317 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->-expr\n",yylineno);
					check_arithmetic((yyvsp[0].expression),yylineno);
					(yyval.expression)=newexpr(arithmexpr_e);
					(yyval.expression)->sym=InsertTmpName(scope);
					(yyval.expression)->value.strValue = (yyval.expression)->sym->name;
					//emit(uminus,$$,$2,NULL,-1,nextquadlabel());//
					emit(mul,(yyval.expression),(yyvsp[0].expression),newexpr_constint(-1),-1,nextquadlabel());
					}
#line 1937 "parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 325 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->!expr\n",yylineno);
				(yyval.expression)=newexpr(boolexpr_e);
				(yyval.expression)->sym=InsertTmpName(scope);
				(yyval.expression)->value.strValue = (yyval.expression)->sym->name;
			//	emit(not,$$,$2,NULL,-1,nextquadlabel());//fixed the Not for the Tcode
				emit(if_eq,NULL,(yyvsp[0].expression),newexpr_constbool(false),nextquadlabel()+3,nextquadlabel());//here changing to arg2---------------------
				emit(assign,(yyval.expression),newexpr_constbool(false),NULL,-1,-1);
				emit(jump,NULL,NULL,NULL,nextquadlabel()+2,nextquadlabel());
				emit(assign,(yyval.expression),newexpr_constbool(true),NULL,-1,-1);			
				}
#line 1952 "parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 335 "parser.y" /* yacc.c:1646  */
    {
				//if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"increment"); dont uncomment its bad for ur health
				check_arithmetic((yyvsp[0].expression),yylineno);
				if((yyvsp[0].expression)->type==tableitem_e){
					
					(yyval.expression)=emit_iftableitem((yyvsp[0].expression),scope);
					emit(add,(yyval.expression),(yyval.expression),newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,(yyval.expression),(yyvsp[0].expression),(yyvsp[0].expression)->index,-1,nextquadlabel());	
				}else{
					emit(add,(yyvsp[0].expression),(yyvsp[0].expression),newexpr_constint(1),-1,nextquadlabel());
					(yyval.expression)=newexpr(arithmexpr_e);
					(yyval.expression)->sym=InsertTmpName(scope);
					emit(assign,(yyval.expression),(yyvsp[0].expression),NULL,-1,nextquadlabel());
				}
				 fprintf(yyout,"#Line %d: term->++lvalue\n",yylineno);
				 }
#line 1973 "parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 351 "parser.y" /* yacc.c:1646  */
    {
		//	if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"increment"); dont uncomment its bad for ur health
			check_arithmetic((yyvsp[0].expression),yylineno);		
			}
#line 1982 "parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 354 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->lvalue++\n",yylineno);
				(yyval.expression)=newexpr(var_e);
				(yyval.expression)->sym=InsertTmpName(scope);				
				if((yyvsp[-2].expression)->type==tableitem_e){
					expr* tmp=emit_iftableitem((yyvsp[-2].expression),scope);
					emit(assign,(yyval.expression),tmp,NULL,-1,nextquadlabel());
					emit(add,tmp,tmp,newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,tmp,(yyvsp[-2].expression),(yyvsp[-2].expression)->index,-1,nextquadlabel());
				}else{
					emit(assign,(yyval.expression),(yyvsp[-2].expression),NULL,-1,nextquadlabel());
					emit(add,(yyvsp[-2].expression),(yyvsp[-2].expression),newexpr_constint(1),-1,nextquadlabel());
				}				
				}
#line 2000 "parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 367 "parser.y" /* yacc.c:1646  */
    {
				//if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"decrement"); dont uncomment its bad for ur health
				check_arithmetic((yyvsp[0].expression),yylineno);
				fprintf(yyout,"#Line %d: term->--lvalue\n",yylineno);
				if((yyvsp[0].expression)->type==tableitem_e){
					(yyval.expression)=emit_iftableitem((yyvsp[0].expression),scope);
					emit(sub,(yyval.expression),(yyval.expression),newexpr_constint(1),-1,nextquadlabel());
					emit(tablesetelem,(yyval.expression),(yyvsp[0].expression),(yyvsp[0].expression)->index,-1,nextquadlabel());	
				}else{
					emit(sub,(yyvsp[0].expression),(yyvsp[0].expression),newexpr_constint(1),-1,nextquadlabel());
					(yyval.expression)=newexpr(arithmexpr_e);
					(yyval.expression)->sym=InsertTmpName(scope);
					emit(assign,(yyval.expression),(yyvsp[0].expression),NULL,-1,nextquadlabel());
				}
				}
#line 2020 "parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 382 "parser.y" /* yacc.c:1646  */
    {
		//	if(yyval.strValue) LvalueAction(yylval.strValue,yylineno,"decrement"); dont uncomment its bad for ur health
			check_arithmetic((yyvsp[0].expression),yylineno);}
#line 2028 "parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 385 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->lvalue--\n",yylineno);
			(yyval.expression)=newexpr(var_e);
			(yyval.expression)->sym=InsertTmpName(scope);				
			if((yyvsp[-2].expression)->type==tableitem_e){
				expr* tmp=emit_iftableitem((yyvsp[-2].expression),scope);
				emit(assign,(yyval.expression),tmp,NULL,-1,nextquadlabel());
				emit(sub,tmp,tmp,newexpr_constint(1),-1,nextquadlabel());
				emit(tablesetelem,tmp,(yyvsp[-2].expression),(yyvsp[-2].expression)->index,-1,nextquadlabel());
			}else{
				emit(assign,(yyval.expression),(yyvsp[-2].expression),NULL,-1,nextquadlabel());
				emit(sub,(yyvsp[-2].expression),(yyvsp[-2].expression),newexpr_constint(1),-1,nextquadlabel());
			}
			}
#line 2046 "parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 398 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: term->primary\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);				
				}
#line 2054 "parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 402 "parser.y" /* yacc.c:1646  */
    {if(!ArrayDeclaration){
		 LvalueAction(yylval.strValue,yylineno,"assign any value to");
		}
		ArrayDeclaration=false;
		}
#line 2064 "parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 406 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: assignexpr->lvalue=expr\n",yylineno);
				if((yyvsp[-3].expression)->type==tableitem_e){
					emit(tablesetelem,(yyvsp[0].expression),(yyvsp[-3].expression),(yyvsp[-3].expression)->index,-1,nextquadlabel());
					(yyval.expression)=emit_iftableitem((yyvsp[-3].expression),scope);
					(yyval.expression)->type=assignexpr_e;		
				}else{
					emit(assign,(yyvsp[-3].expression),(yyvsp[0].expression),NULL,-1,nextquadlabel());
					(yyval.expression)=newexpr(assignexpr_e);
					(yyval.expression)->sym=InsertTmpName(scope);
					emit(assign,(yyval.expression),(yyvsp[-3].expression),NULL,-1,nextquadlabel());
				}			
				}
#line 2081 "parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 419 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: primary->lvalue\n",yylineno);
				(yyval.expression)=emit_iftableitem((yyvsp[0].expression),scope);
				}
#line 2089 "parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 422 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: primary->call\n",yylineno);}
#line 2095 "parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 423 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: primary->objectdef\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);				
				}
#line 2103 "parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 426 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: primary->(funcdef)\n",yylineno);
					(yyval.expression)=newexpr(programfunc_e);
					(yyval.expression)->sym=(yyvsp[-1].sym);					
					}
#line 2112 "parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 430 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: primary->const\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);					
				}
#line 2120 "parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 434 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: lvalue->id\n",yylineno);
				ST_node *tmp=ReturnNode(HashTable,yylval.strValue,scope);
				if(tmp!=NULL&&tmp->type!=USERFUNC&&!isLibFunc(yytext)){		
					if(tmp->scope!=0)//ean den briskomaste se global scope
					{	
						if(PendingFunction(HeadNTail,tmp->scope,scope)) {
							printf("Error:(line %d) No access in %s\n",yylineno,yytext);//ean parembalete sinartisi anamesa
							ErrorCounter++;
						}
					}
				}else if(tmp!=NULL&&tmp->type==USERFUNC){}//ean einai userfunction simainei oti anaferetai s aytin
				else if(isLibFunc(yytext)) {}	
				else
				{
					if(scope==0) tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,GLOBAL,currscopeoffset(),-1,-1,currscopespace());
					else tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,LOCCAL,currscopeoffset(),-1,-1,currscopespace());
					inccurrscopeoffset();
				}
				(yyval.expression)=lvalue_expr(tmp);
				}
#line 2145 "parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 454 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: lvalue->local id\n",yylineno);
				ST_node *tmp=SearchNameInScope(HashTable,yytext,scope);
				if(isLibFunc(yytext)&&scope!=0) {printf("Error:(line %d)Cant have the same name with Library Function\n",yylineno);
								ErrorCounter++;			
				}
				else if(tmp){//den to eisagei oyte petaei error gt simainei oti to xei brei kai anaferetai s ayto to simbolo
				}else{
					if(scope==0) tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,GLOBAL,currscopeoffset(),-1,-1,currscopespace());
					else tmp=InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,LOCCAL,currscopeoffset(),-1,-1,currscopespace());
					inccurrscopeoffset();					
				}
				if(tmp) (yyval.expression)=lvalue_expr(tmp);
				else (yyval.expression)=newexpr_constnil();
				}
#line 2164 "parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 468 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: lvalue->::id\n",yylineno);
				ST_node *tmp=SearchNameInScope(HashTable,yylval.strValue,0);
				if(tmp==NULL){//den iparxei global metavliti me ayto to onoma
					printf("Error:(line %d)No Global ID named %s\n",yylineno,yytext);
					ErrorCounter++;
					(yyval.expression)=newexpr_constnil();
				}else (yyval.expression)=lvalue_expr(tmp);
				}
#line 2177 "parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 476 "parser.y" /* yacc.c:1646  */
    { (yyval.expression)=(yyvsp[0].expression); fprintf(yyout,"#Line %d: lvalue->member\n",yylineno);}
#line 2183 "parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 478 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: member->lvalue.id\n",yylineno);
				(yyval.expression)=member_item((yyvsp[-2].expression),(yyvsp[0].strValue),scope);
		}
#line 2191 "parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 481 "parser.y" /* yacc.c:1646  */
    {ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->lvalue[expr]\n",yylineno);
						(yyvsp[-3].expression)=emit_iftableitem((yyvsp[-3].expression),scope);
						(yyval.expression)=newexpr(tableitem_e);
						(yyval.expression)->sym=(yyvsp[-3].expression)->sym;
						(yyval.expression)->index=(yyvsp[-1].expression);
		}
#line 2202 "parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 488 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: member->tableitem\n",yylineno);}
#line 2208 "parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 489 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: member->call.id\n",yylineno);
				(yyval.expression)->value.strValue=(yyvsp[0].strValue);
				(yyvsp[-2].expression)->type=tableitem_e;				
				}
#line 2217 "parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 493 "parser.y" /* yacc.c:1646  */
    {ArrayDeclaration=true;fprintf(yyout,"#Line %d: member->call[expr]\n",yylineno);}
#line 2223 "parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 495 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: call->call(elist)\n",yylineno);
						(yyval.expression)=make_call((yyvsp[-3].expression),(yyvsp[-1].expression),scope);
					}
#line 2231 "parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 498 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: call->lvalue callsuffix\n",yylineno);
					if((yyvsp[0].expression)->method){
						expr *self=(yyvsp[-1].expression);
						(yyvsp[-1].expression)=emit_iftableitem(member_item(self,(yyvsp[0].expression)->value.strValue,scope),scope);
						(yyvsp[0].expression)=elist_add_front((yyvsp[0].expression),self);
					}					
					(yyval.expression)=make_call((yyvsp[-1].expression),(yyvsp[0].expression),scope);
					}
#line 2244 "parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 506 "parser.y" /* yacc.c:1646  */
    {	fprintf(yyout,"#Line %d: call->(funcdef)(elist)\n",yylineno);
							expr* func=newexpr(programfunc_e);
							func->sym=(yyvsp[-4].sym);
							(yyval.expression)=make_call(func,(yyvsp[-1].expression),scope);						
		}
#line 2254 "parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 512 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: callsuffix->normcall\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);				
				}
#line 2262 "parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 515 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: callsuffix->methodcall\n",yylineno);
				(yyval.expression)=(yyvsp[0].expression);
				}
#line 2270 "parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 519 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: normcall->(elist)\n",yylineno);
				if((yyvsp[-1].expression))(yyval.expression)=(yyvsp[-1].expression);
				else (yyval.expression)->next=NULL;
				(yyval.expression)->method=false;				
				}
#line 2280 "parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 525 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: methodcall->..id(elist)\n",yylineno);
					if((yyvsp[-1].expression)) (yyval.expression)=(yyvsp[-1].expression);
					(yyval.expression)->method=true;
					(yyval.expression)->value.strValue=(yyvsp[-3].strValue);
					}
#line 2290 "parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 531 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: elist->expr listexpr\n",yylineno);
						(yyval.expression)=(yyvsp[-1].expression);
						(yyval.expression)->next=(yyvsp[0].expression);				
						}
#line 2299 "parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 535 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: elist->empty rule\n",yylineno);
			(yyval.expression)=newexpr_constnil();		
			}
#line 2307 "parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 539 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: listexpr->, expr listexpr\n",yylineno);
													if((yyvsp[-1].expression)){
														(yyval.expression)=(yyvsp[-1].expression);
														(yyval.expression)->next=(yyvsp[0].expression);	
													}	}
#line 2317 "parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 544 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: listexpr->empty rule\n",yylineno);
						(yyval.expression)=newexpr_constnil();			
						}
#line 2325 "parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 548 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: objectdef->[elist]\n",yylineno);
						expr *tmp,*tmp2;

						tmp2=(yyvsp[-1].expression);
						tmp=newexpr(newtable_e);
						
						tmp->sym=InsertTmpName(scope);
						emit(tablecreate,tmp,NULL,NULL,-1,nextquadlabel());
						int i=0;
						while(tmp2->next){
							emit(tablesetelem,tmp,tmp2,newexpr_constint(i++),-1,nextquadlabel());
							tmp2=tmp2->next;
						}
						(yyval.expression)=tmp;
						}
#line 2345 "parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 563 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: objectdef->[indexed]\n",yylineno);
						expr *tmp;
	
						indexedexprs* tmp2=(yyvsp[-1].indexexpr);
						tmp=newexpr(newtable_e);
						tmp->sym=InsertTmpName(scope);
						emit(tablecreate,tmp,NULL,NULL,-1,nextquadlabel());
						while(tmp2){
							emit(tablesetelem,tmp,tmp2->key,tmp2->value,-1,nextquadlabel());
							tmp2=tmp2->next;
						}
						(yyval.expression)=tmp;
						}
#line 2363 "parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 577 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: indexed->indexedelem indexedelemlist\n",yylineno);
						(yyval.indexexpr)=(yyvsp[-1].indexexpr);
						(yyval.indexexpr)->next=(yyvsp[0].indexexpr);
						}
#line 2372 "parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 582 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: indexedelemlist->,indexedelem indexedelemlist\n",yylineno);
						(yyval.indexexpr)=(yyvsp[-1].indexexpr);
						(yyval.indexexpr)->next=(yyvsp[0].indexexpr);
						}
#line 2381 "parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 586 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: indexedelemlist->empty rule\n",yylineno);
						(yyval.indexexpr)=NULL;					
						}
#line 2389 "parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 590 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: indexedelem->{expr:expr}\n",yylineno);
						(yyval.indexexpr)=insertIndexValues((yyvsp[-3].expression),(yyvsp[-1].expression));
						}
#line 2397 "parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 594 "parser.y" /* yacc.c:1646  */
    {scope++;}
#line 2403 "parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 594 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: block->{}\n",yylineno);DisableSymbols(HeadNTail,scope);scope--;}
#line 2409 "parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 595 "parser.y" /* yacc.c:1646  */
    {scope++;}
#line 2415 "parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 595 "parser.y" /* yacc.c:1646  */
    {DisableSymbols(HeadNTail,scope);scope--;
			fprintf(yyout,"#Line %d: block->{statement statements}\n",yylineno);
			(yyval.expression)=(yyvsp[-2].expression);
			}
#line 2424 "parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 600 "parser.y" /* yacc.c:1646  */
    {(yyval.strValue)=(yyvsp[0].strValue); fprintf(yyout,"#Line %d: funcname->id\n",yylineno);}
#line 2430 "parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 601 "parser.y" /* yacc.c:1646  */
    {(yyval.strValue)=newtmpfuncname(); fprintf(yyout,"#Line %d: funcname->empty rule\n",yylineno);}
#line 2436 "parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 603 "parser.y" /* yacc.c:1646  */
    {FunDeclared++;
			ST_node *tmp=SearchNameInScope(HashTable,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(tmp){
				printf("Error:(line %d) Redeclaration of symbol %s\n",yylineno,yytext);
				ErrorCounter++;
			}else{
				tmp=InsertSymbol(HashTable,HeadNTail,(yyvsp[0].strValue),scope,yylineno,USERFUNC,0,nextquadlabel(),0,currscopespace());
			}
			(yyval.sym)=tmp;
			emit(funcstart,lvalue_expr((yyval.sym)),NULL,NULL,-1,nextquadlabel());
			OffsetStack=pushOffset(currscopeoffset());//ok
			enterscopespace();
			resetformalargsoffset();
			fprintf(yyout,"#Line %d: funcprefix->function funcname\n",yylineno);
		}
#line 2459 "parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 622 "parser.y" /* yacc.c:1646  */
    {scope++;}
#line 2465 "parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 622 "parser.y" /* yacc.c:1646  */
    {
					enterscopespace();
					resetfunctionlocaloffset();
					scope--;
					fprintf(yyout,"#Line %d: funcargs->(idlist)\n",yylineno);
		}
#line 2476 "parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 628 "parser.y" /* yacc.c:1646  */
    {loopStack=pushloop();loopCounter=0;}
#line 2482 "parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 629 "parser.y" /* yacc.c:1646  */
    {loopCounter=poploop();}
#line 2488 "parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 630 "parser.y" /* yacc.c:1646  */
    {
			exitscopespace();
			
			FunDeclared--;
			fprintf(yyout,"#Line %d: funcbody->body\n",yylineno);		
		}
#line 2499 "parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 637 "parser.y" /* yacc.c:1646  */
    {
			fprintf(yyout,"#Line %d: funcdef->funcprefix funcargs funcbody\n",yylineno);
			exitscopespace();

			(yyvsp[-2].sym)->VariablesNO=FunctionLocalOffset;
			restorecurrscopeoffset(popOffset());
			(yyval.sym)=(yyvsp[-2].sym);
			emit(funcend,lvalue_expr((yyvsp[-2].sym)),NULL,NULL,-1,nextquadlabel());
		}
#line 2513 "parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 647 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->INT\n",yylineno);
			(yyval.expression)=newexpr_constint((yyvsp[0].intValue));
			}
#line 2521 "parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 650 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->REAL\n",yylineno);
			(yyval.expression)=newexpr_constreal((yyvsp[0].realValue));
			}
#line 2529 "parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 653 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->STRING\n",yylineno);
				(yyval.expression)=newexpr_conststring((yyvsp[0].strValue));
				}
#line 2537 "parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 656 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->NIL\n",yylineno);
			(yyval.expression)=newexpr_constnil();
			}
#line 2545 "parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 659 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->TRUE\n",yylineno);
			(yyval.expression)=newexpr_constbool(true);
			}
#line 2553 "parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 662 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: const->FALSE\n",yylineno);
			(yyval.expression)=newexpr_constbool(false);
			}
#line 2561 "parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 666 "parser.y" /* yacc.c:1646  */
    {	if(FunDeclared) AddArgs(HeadNTail,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);		
				ErrorCounter++;	
			}else {
				InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,FORMAL,currscopeoffset(),-1,-1,currscopespace());
				inccurrscopeoffset();
			}
		}
#line 2578 "parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 677 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: idlist->id idlist\n",yylineno);}
#line 2584 "parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 678 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: idlist->empty rule\n",yylineno);}
#line 2590 "parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 680 "parser.y" /* yacc.c:1646  */
    {
			if(FunDeclared) AddArgs(HeadNTail,yytext,scope);
			if(isLibFunc(yytext)){
				printf("Error:(line %d)Formal can not use same name with Library Functions\n",yylineno);
				ErrorCounter++;
			}else if(SearchNameInScope(HashTable,yytext,scope)){
				printf("Error:(line %d)Formal Redeclaration(ID %s)\n",yylineno,yytext);		
				ErrorCounter++;	
			}else {
				InsertSymbol(HashTable,HeadNTail,yytext,scope,yylineno,FORMAL,currscopeoffset(),-1,-1,currscopespace());
				inccurrscopeoffset();
			}
		}
#line 2608 "parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 692 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: idlistlist->, id idlistlist\n",yylineno);}
#line 2614 "parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 693 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: idlistlist->empty rule\n",yylineno);}
#line 2620 "parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 695 "parser.y" /* yacc.c:1646  */
    {	fprintf(yyout,"#Line %d: ifprefix->if(expr)\n",yylineno);
					emit(if_eq,(yyvsp[-1].expression),newexpr_constbool(true),NULL,nextquadlabel()+2,nextquadlabel());
					(yyval.intValue)=nextquadlabel();
					emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
#line 2630 "parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 701 "parser.y" /* yacc.c:1646  */
    {	fprintf(yyout,"#Line %d: elseprefix->else\n",yylineno);
			(yyval.intValue)=nextquadlabel();
			emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
#line 2639 "parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 706 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: ifstmt->ifprefix statement\n",yylineno);
					patchlabel((yyvsp[-1].intValue),nextquadlabel());					
					}
#line 2647 "parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 709 "parser.y" /* yacc.c:1646  */
    {	fprintf(yyout,"#Line %d: ifstmt->ifprefix statement elseprefix statement\n",yylineno);
								patchlabel((yyvsp[-3].intValue),(yyvsp[-1].intValue)+1);	
								patchlabel((yyvsp[-1].intValue),nextquadlabel());			
		}
#line 2656 "parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 714 "parser.y" /* yacc.c:1646  */
    {	(yyval.intValue)=nextquadlabel();
			++loopCounter;
			if(loopCounter>1||FunDeclared) {
				LoopListStack=pushNestedLoop();
				BreakList=NULL;
				ContinueList=NULL;
			}	
		}
#line 2669 "parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 723 "parser.y" /* yacc.c:1646  */
    { emit(if_eq,(yyvsp[-1].expression),newexpr_constbool(true),NULL,nextquadlabel()+2,nextquadlabel());
				(yyval.intValue)=nextquadlabel();
				emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
#line 2678 "parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 728 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: whilestatement->while(expr)statement\n",yylineno);
						emit(jump,NULL,NULL,NULL,(yyvsp[-2].intValue),nextquadlabel());//check it here
						patchlabel((yyvsp[-1].intValue),nextquadlabel());	//ayto patsarei apo pano
						
							while(ContinueList!=NULL){
								patchlabel(ContinueList->quadNo,(yyvsp[-2].intValue));
								ContinueList=ContinueList->next;
							}
						
							while(BreakList!=NULL){
								patchlabel(BreakList->quadNo,nextquadlabel());
								BreakList=BreakList->next;
							}
											
						if(LoopListStack){
							BreakList=LoopListStack->BreakList;
							ContinueList=LoopListStack->ContinueList;
							NestLoops *tmp=LoopListStack;
							tmp=LoopListStack->next;
							free(LoopListStack);
							LoopListStack=tmp;
						}
						--loopCounter;				
		}
#line 2707 "parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 753 "parser.y" /* yacc.c:1646  */
    {(yyval.intValue)=nextquadlabel();
		emit(jump,NULL,NULL,NULL,-1,nextquadlabel());
		}
#line 2715 "parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 757 "parser.y" /* yacc.c:1646  */
    {(yyval.intValue)=nextquadlabel();}
#line 2721 "parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 759 "parser.y" /* yacc.c:1646  */
    {	++loopCounter;
			if(loopCounter>1||FunDeclared){
				LoopListStack=pushNestedLoop();
				BreakList=NULL;
				ContinueList=NULL;
			}		
		}
#line 2733 "parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 765 "parser.y" /* yacc.c:1646  */
    {
							(yyval.Mquad)=(struct M*)malloc(sizeof(struct M));
							(yyval.Mquad)->test=(yyvsp[-2].intValue);
							(yyval.Mquad)->enter=nextquadlabel();
							emit(if_eq,NULL,(yyvsp[-1].expression),newexpr_constbool(true),-1,nextquadlabel());
							}
#line 2744 "parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 772 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: forstatement->for(elist;expr;elist) statement\n",yylineno);
							patchlabel((yyvsp[-6].Mquad)->enter,(yyvsp[-2].intValue)+1);
							patchlabel((yyvsp[-5].intValue),nextquadlabel());
							patchlabel((yyvsp[-2].intValue),(yyvsp[-6].Mquad)->test);
							patchlabel((yyvsp[0].intValue),(yyvsp[-5].intValue)+1);
												
							while(ContinueList!=NULL){
								patchlabel(ContinueList->quadNo,(yyvsp[-5].intValue)+1);
								ContinueList=ContinueList->next;
							}
									
							while(BreakList!=NULL){
								patchlabel(BreakList->quadNo,nextquadlabel());
								BreakList=BreakList->next;
							}

							if(LoopListStack){
								BreakList=LoopListStack->BreakList;
								ContinueList=LoopListStack->ContinueList;
								NestLoops *tmp=LoopListStack;
								tmp=LoopListStack->next;
								free(LoopListStack);
								LoopListStack=tmp;
							}
							--loopCounter;
							}
#line 2775 "parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 799 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: returnstament->return;\n",yylineno);
					if(FunDeclared){
						emit(retur,NULL,NULL,NULL,-1,nextquadlabel());
					}else{
						printf("Error:(line %d)Return used outside of a function\n",yylineno);
						ErrorCounter++;
					}				
					}
#line 2788 "parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 807 "parser.y" /* yacc.c:1646  */
    {fprintf(yyout,"#Line %d: returnstament->return expr;\n",yylineno);
					if(FunDeclared){
						emit(retur,NULL,NULL,(yyvsp[-1].expression),-1,nextquadlabel());
					}else{
						printf("Error:(line %d)Return used outside of a function\n",yylineno);
						ErrorCounter++;
					}					
					}
#line 2801 "parser.c" /* yacc.c:1646  */
    break;


#line 2805 "parser.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  return yyresult;
}
#line 817 "parser.y" /* yacc.c:1906  */


int yyerror(char *yaccProvideMessage)
{
	fprintf(stderr,"%s :at line %d \n",yaccProvideMessage,yylineno);
	ErrorCounter++;
}

void initializeStructs()
{//ginontai arxikopoiisi toy hashtale toy pinaka me ta scopelists
//kai ta library functions poy eisagontai ston pinaka
	int i;
	for(i=0;i<hashTSize;i++)
	{
		HashTable[i]=NULL;	
	}	
	for(i=0;i<2;i++)
	{
		HeadNTail[i]=NULL;
	}

	InsertSymbol(HashTable,HeadNTail,"print",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"input",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"objectmemberkeys",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"objecttotalmembers",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"objectcopy",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"totalarguments",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"argument",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"typeof",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"strtonum",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"sqrt",0,0,LIBFUNC,0,-1,0,programvar);
	InsertSymbol(HashTable,HeadNTail,"cos",0,0,LIBFUNC,0,-1,0,programvar);//ok
	InsertSymbol(HashTable,HeadNTail,"sin",0,0,LIBFUNC,0,-1,0,programvar);//ok
	
}

int main(int argc,char **argv)
{
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
			fprintf(stderr,"Cannot read file:%s\n",argv[1]);
			return 1;
		}
	}else yyin=stdin;
	
	initializeStructs();
	yyparse();//2 fash
	generateT();
	patch_incomplete_jumps();
	//Print_HashTable(HashTable);
	//ScopeNode(HeadNTail);
	
	if(ErrorCounter) printf("ERROR NO: %d.Correct them to produce quads!\n",ErrorCounter);
	else{
		//printQuads();//3h fash
		//printTcode();//4h fash
		//printTables();
		produceFile();
		run_virtual_machine();
	}
	return 0;
}
