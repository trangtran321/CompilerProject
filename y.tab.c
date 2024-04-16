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
#line 1 "lab9.y" /* yacc.c:339  */

/* --------------------------------------------------------------------------------------------------------------------
            Trang Tran
            November 8, 2023
            Lab 9 : lab9.y

            **Description & Purpose::
              This program will identify all tokens that have been passed from LEX to YACC and determine if
			  the input is valid in terms of the production rules below. It uses the production rules according to the
			  BNF and implements both a symbol table and abstract symbol tree in order to build and print the grammar.
			  The ast.h file will connect this file with the abstract symbol tree print function. The symtable.h file
			  will connect us to the symbol table. In the syntax directed statements of each production rule, nodes of the
			  abstract symbol tree are created and linked to one another. When there is a declaration (of either variable
			  or function), the syntax directed statements in each production rule will check to make sure this is a
			  valid declaration.

              - Input:  The tokenized stream that LEX parsed from the user inputted character stream.
              - Output: The abstract symbol tree and symbol table for the inputted program is printed to the terminal.
			  			If the inputted program has invalid declarations or invalid use of rules, the user will be notified
						of the line in which the error occurred and what type of error it is.

    *********************Changes made to document by TRANG TRAN on AUGUST 23, 2023*********************
              --  Added inline comments and documentation prior to functions.
			  --  Added multiplication expression evaluation. (line 155)
			  --  Removed the the first 'expr' in front of the '-' for the unary minus expression. (line 167)
			  --  Added the parenthesis expression evaluation. (line 143)
			  --  Added the yylex() function to insure that there is no warning from compiler (line 73)
			  --  Proofread and cleaned up print statements
			  --  Corrected the ouput for variables found in line 172 to include the correct value.

	*********************Changes made to document by TRANG TRAN on SEPTEMBER 6, 2023*********************
			  --  Added global variable MAX to set the size of the symbol table and regs[] size is set to MAX
			  --  Linked linecount variable from lex file.
			  --  Added global variable 'CURRENT' to hold the current index in symbol table
			  --  Added union function to pair tokens with the appropriate data type
			  --  Defined which tokens need data types specification
			  --  Changed start of rules from 'list' to 'P'
			  --  Added rules P, DCLS, DCL so that variables can be declared and saved in the symbol table.
			  --  Stat rule is updated to search for whether a VARIABLE has been declared or not. If
			  	  it has been declared, the address will be used to get the value of the VARIABLE. If the
				  VARIABLE has not been declared, an error message will printed.
			  --  expr rule is updated to search for whether a VARIABLE has been declared before
			      its value can be used to compute an entire expression. If it has not been declared,
				  the value is set to '0' and an error message will be printed.

	*********************Changes made to document by TRANG TRAN on SEPTEMBER 16, 2023*********************
			  --  All tokens have been added to the document.
			  --  All production rules have been added to the document.
			  --  Continue, Break and PrototypeDescription production rules have been added to document to reflect
			  	  what is expected of graduate students.
			  --  None of the rules have functionality in terms of a calculator anymore or any real functionatlity other
			  	  than to set up an alphabet that can be expanded upon.
			  --  mydebug and linecount are external variables that LEX controls but is used in YACC file to output
			  	  the line number an error occurs and to turn the debug print statements on.

	*********************Changes made to document by TRANG TRAN on September 22, 2023************************
			  --  'ast.h' is included into the document
			  --  datatypes ASTnode*, enum DataTypes, and enum OPERATORS are added into the %union function.
			  	  This will connect the yacc file and the 'ast.c' file.
			  --  %type <node>, %type <datatype>, and %type <operator> are declare production rules as specific datatypes
			      in the abstract symbol tree.
			  --  All production rules have been assigned to nodes of the same name and all data for each production
			      rule is stored in either pointers s1, s2, operator, name, or value as class properties of the object
				  ASTnode.
	*********************Changes made to document by TRANG TRAN on October 20, 2023************************
			  --  PrototypeDeclaration production rule was deleted. Instead, FunctionBody production rule was added. This
			  	  allows us to have the same parse for the declaration of a function - but then decide whether or not
				  this first pattern belongs to a full function definition or just a prototype declaration.
			  --  Global variables level, offset, goffset and maxoffset are declared to be used in the symbol table.
			  --  Included the 'symtable.h' file in order to link the code that builds a symbol table.
			  --  Added syntax-directed instructions to rules that declare, define and use variables and functions - see
			      each rule for specific changes made to rule.
	*********************Changes made to document by TRANG TRAN on  November 29, 2023************************
			  --  Added 'param' flag in symbol table. It is only used in the case that an array was passed as a parameter.
			  	  The flag is detected in MIPS and signals when to load an array address' value into $a0. This flag is only
				  for parameters.
			  --  Added var_check() to 'write' rule so that there is no illegal use of an A_VAR.
			  --  Left factored Varlist rule
			  --  Added commands in main to print MIPS code into an .asm file


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "emit.h"

extern int linecount;
extern int mydebug;

int level = 0; 	//This keeps track of how many compound statements deep we are in
int offset = 0; 	//This is how many words have we seen at a global or inside a function
int goffset; 	// Holder for global offset when we enter and exit function definition
int maxoffset; 		//total number of words a function needs

/*  * @precondition: There is input to be evaluated.
    * @postcondition: None
    * @param: None
	* @return: Returns the type of token that was found in LEX
    * @description: */
int yylex();

/* @description: This function will be called if there is an error */
void yyerror (s)  /* Called by yyparse on error */ //start yyerror
     char *s;
{
  printf ("Error found in line %d: %s \n", linecount, s);
} //end yyerror

#line 180 "y.tab.c" /* yacc.c:339  */

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
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    T_NUM = 258,
    T_ID = 259,
    T_STRING = 260,
    T_INT = 261,
    T_BOOLEAN = 262,
    T_VOID = 263,
    T_BEGIN = 264,
    T_END = 265,
    T_RETURN = 266,
    T_IF = 267,
    T_THEN = 268,
    T_ELSE = 269,
    T_ENDIF = 270,
    T_WHILE = 271,
    T_DO = 272,
    T_READ = 273,
    T_WRITE = 274,
    T_TRUE = 275,
    T_FALSE = 276,
    T_NOT = 277,
    T_OR = 278,
    T_AND = 279,
    T_CONT = 280,
    T_BREAK = 281,
    T_LE = 282,
    T_GR = 283,
    T_LEEQU = 284,
    T_GREQU = 285,
    T_NEQU = 286,
    T_EQUAL = 287,
    T_PLUS = 288,
    T_MINUS = 289,
    T_MUL = 290,
    T_DIV = 291,
    T_ASSIGN = 292
  };
#endif
/* Tokens.  */
#define T_NUM 258
#define T_ID 259
#define T_STRING 260
#define T_INT 261
#define T_BOOLEAN 262
#define T_VOID 263
#define T_BEGIN 264
#define T_END 265
#define T_RETURN 266
#define T_IF 267
#define T_THEN 268
#define T_ELSE 269
#define T_ENDIF 270
#define T_WHILE 271
#define T_DO 272
#define T_READ 273
#define T_WRITE 274
#define T_TRUE 275
#define T_FALSE 276
#define T_NOT 277
#define T_OR 278
#define T_AND 279
#define T_CONT 280
#define T_BREAK 281
#define T_LE 282
#define T_GR 283
#define T_LEEQU 284
#define T_GREQU 285
#define T_NEQU 286
#define T_EQUAL 287
#define T_PLUS 288
#define T_MINUS 289
#define T_MUL 290
#define T_DIV 291
#define T_ASSIGN 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 121 "lab9.y" /* yacc.c:355  */

	int value;
	char* string;
	ASTnode* node;
	enum DataTypes datatype;
	enum OPERATORS operator;

#line 302 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 319 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   138

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  140

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

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
      42,    43,     2,     2,    41,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   168,   168,   182,   187,   197,   201,   211,   237,   257,
     276,   298,   325,   329,   333,   352,   381,   351,   441,   449,
     466,   471,   483,   488,   502,   524,   559,   558,   582,   586,
     598,   602,   611,   616,   621,   626,   631,   636,   641,   646,
     651,   656,   666,   676,   689,   698,   713,   725,   729,   740,
     751,   763,   783,   807,   816,   823,   836,   865,   908,   913,
     944,   949,   954,   959,   964,   969,   987,   992,  1017,  1022,
    1040,  1045,  1072,  1077,  1082,  1086,  1101,  1106,  1113,  1118,
    1123,  1131,  1139,  1172,  1228,  1232,  1244,  1255
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_NUM", "T_ID", "T_STRING", "T_INT",
  "T_BOOLEAN", "T_VOID", "T_BEGIN", "T_END", "T_RETURN", "T_IF", "T_THEN",
  "T_ELSE", "T_ENDIF", "T_WHILE", "T_DO", "T_READ", "T_WRITE", "T_TRUE",
  "T_FALSE", "T_NOT", "T_OR", "T_AND", "T_CONT", "T_BREAK", "T_LE", "T_GR",
  "T_LEEQU", "T_GREQU", "T_NEQU", "T_EQUAL", "T_PLUS", "T_MINUS", "T_MUL",
  "T_DIV", "T_ASSIGN", "';'", "'['", "']'", "','", "'('", "')'", "$accept",
  "Program", "DeclarationList", "Declaration", "VarDeclaration", "VarList",
  "TypeSpecifier", "FunDeclaration", "$@1", "$@2", "FunctionBody",
  "Params", "ParamList", "Param", "CompoundStmt", "$@3",
  "LocalDeclarations", "StatementList", "Statement", "ExpressionStmt",
  "SelectionStmt", "IterationStmt", "ReturnStmt", "ReadStmt", "WriteStmt",
  "AssignmentStmt", "ContinueStmt", "BreakStmt", "Expression", "Var",
  "SimpleExpression", "Relop", "AdditiveExpression", "Addop", "Term",
  "Multop", "Factor", "Call", "Args", "ArgList", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,    59,    91,
      93,    44,    40,    41
};
# endif

#define YYPACT_NINF -111

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-111)))

#define YYTABLE_NINF -21

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      47,  -111,  -111,  -111,    16,  -111,    47,  -111,    24,  -111,
    -111,  -111,   -28,   -23,    28,  -111,  -111,    31,    12,    71,
      20,  -111,    44,    82,    48,  -111,    49,    86,    60,  -111,
      47,    61,    62,     4,  -111,  -111,  -111,  -111,  -111,  -111,
    -111,    47,    47,    96,    18,  -111,    64,  -111,   -22,     3,
      46,    46,   107,    43,  -111,  -111,    46,    74,    75,  -111,
      46,  -111,   104,    18,  -111,  -111,  -111,  -111,  -111,  -111,
    -111,  -111,  -111,    77,    79,    78,    38,    34,  -111,  -111,
      46,    46,  -111,    80,  -111,   106,   100,    81,    83,    84,
      85,  -111,  -111,  -111,    87,  -111,  -111,  -111,    46,  -111,
    -111,  -111,  -111,  -111,  -111,    46,  -111,  -111,    46,  -111,
    -111,  -111,  -111,    46,    88,    90,    89,  -111,  -111,    18,
      18,  -111,  -111,  -111,  -111,    66,    38,    34,  -111,  -111,
      46,  -111,    69,  -111,  -111,  -111,    18,  -111,   109,  -111
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    12,    14,    13,     0,     2,     3,     5,     0,     6,
       1,     4,     8,     0,     0,    15,     7,     0,     0,     0,
      10,     9,    13,     0,     0,    21,    22,     0,    24,    16,
       0,     0,     0,     0,    23,    11,    25,    26,    18,    17,
      19,    28,    28,     0,    30,    29,     8,    77,    56,     0,
       0,     0,     0,     0,    80,    81,     0,     0,     0,    43,
       0,    33,     0,    30,    32,    34,    35,    37,    38,    39,
      36,    40,    41,     0,    78,    55,    58,    66,    70,    79,
       0,    84,    47,     0,    78,     0,     0,    56,     0,     0,
       0,    82,    53,    54,     0,    27,    31,    42,     0,    60,
      61,    62,    63,    64,    65,     0,    68,    69,     0,    74,
      75,    72,    73,     0,     0,    86,     0,    85,    48,     0,
       0,    49,    51,    50,    76,     0,    59,    67,    71,    57,
       0,    83,     0,    46,    52,    87,     0,    44,     0,    45
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -111,  -111,   119,  -111,    39,  -111,    32,  -111,  -111,  -111,
    -111,  -111,    97,  -111,    93,  -111,    91,    72,  -110,  -111,
    -111,  -111,  -111,  -111,  -111,  -111,  -111,  -111,   -48,   -44,
      36,  -111,    33,  -111,    21,  -111,   -52,  -111,  -111,     6
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    13,     8,     9,    19,    33,
      39,    24,    25,    26,    61,    41,    44,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    84,
      75,   105,    76,   108,    77,   113,    78,    79,   116,   117
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,    83,    85,    86,    91,    90,    47,    48,    88,   132,
     133,    14,    94,    37,    15,    16,    10,    80,    17,    74,
      81,    47,    48,    54,    55,    56,   138,    37,    12,    49,
      50,    18,   114,   115,    51,    20,    52,    53,    54,    55,
      56,    82,    38,    57,    58,    60,    47,    48,    89,    47,
      48,    23,    21,     1,     2,     3,    59,   109,   110,    27,
      60,   128,    23,    54,    55,    56,    54,    55,    56,   111,
     112,   106,   107,    43,    43,    74,    74,     1,     2,    22,
      42,    42,   115,   136,   137,    60,    28,   -20,    60,    31,
      30,    29,    74,    99,   100,   101,   102,   103,   104,    32,
      46,    35,    36,    14,   134,    99,   100,   101,   102,   103,
     104,    87,    92,    93,    95,    97,    98,   120,   118,   119,
      80,   121,   122,   123,   139,    11,    40,    34,   129,   127,
     124,   130,   131,    45,   125,    96,   135,     0,   126
};

static const yytype_int16 yycheck[] =
{
      44,    49,    50,    51,    56,    53,     3,     4,    52,   119,
     120,    39,    60,     9,    42,    38,     0,    39,    41,    63,
      42,     3,     4,    20,    21,    22,   136,     9,     4,    11,
      12,     3,    80,    81,    16,     4,    18,    19,    20,    21,
      22,    38,    38,    25,    26,    42,     3,     4,     5,     3,
       4,    19,    40,     6,     7,     8,    38,    23,    24,    39,
      42,   113,    30,    20,    21,    22,    20,    21,    22,    35,
      36,    33,    34,    41,    42,   119,   120,     6,     7,     8,
      41,    42,   130,    14,    15,    42,     4,    43,    42,     3,
      41,    43,   136,    27,    28,    29,    30,    31,    32,    39,
       4,    40,    40,    39,    38,    27,    28,    29,    30,    31,
      32,     4,    38,    38,    10,    38,    37,    17,    38,    13,
      39,    38,    38,    38,    15,     6,    33,    30,    40,   108,
      43,    41,    43,    42,    98,    63,   130,    -1,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,     8,    45,    46,    47,    48,    50,    51,
       0,    46,     4,    49,    39,    42,    38,    41,     3,    52,
       4,    40,     8,    50,    55,    56,    57,    39,     4,    43,
      41,     3,    39,    53,    56,    40,    40,     9,    38,    54,
      58,    59,    48,    50,    60,    60,     4,     3,     4,    11,
      12,    16,    18,    19,    20,    21,    22,    25,    26,    38,
      42,    58,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    76,    78,    80,    81,
      39,    42,    38,    72,    73,    72,    72,     4,    73,     5,
      72,    80,    38,    38,    72,    10,    61,    38,    37,    27,
      28,    29,    30,    31,    32,    75,    33,    34,    77,    23,
      24,    35,    36,    79,    72,    72,    82,    83,    38,    13,
      17,    38,    38,    38,    43,    74,    76,    78,    80,    40,
      41,    43,    62,    62,    38,    83,    14,    15,    62,    15
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    48,    49,    49,
      49,    49,    50,    50,    50,    52,    53,    51,    54,    54,
      55,    55,    56,    56,    57,    57,    59,    58,    60,    60,
      61,    61,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    63,    63,    64,    64,    65,    66,    66,    67,
      68,    68,    69,    70,    71,    72,    73,    73,    74,    74,
      75,    75,    75,    75,    75,    75,    76,    76,    77,    77,
      78,    78,    79,    79,    79,    79,    80,    80,    80,    80,
      80,    80,    80,    81,    82,    82,    83,    83
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     3,     1,     4,
       3,     6,     1,     1,     1,     0,     0,     8,     1,     1,
       1,     1,     1,     3,     2,     4,     0,     5,     0,     2,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     5,     7,     4,     2,     3,     3,
       3,     3,     4,     2,     2,     1,     1,     4,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     2,     4,     0,     1,     1,     3
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
#line 169 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, "Program Production Rule\n");
							if (Has_Proto() == 1){
								yyerror("There is an undefined function.");
								exit(1);
							}
							program = (yyvsp[0].node);
						}
#line 1505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 183 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, "DeclarationList Production Rule\n");
							(yyval.node) = ASTCreateNode(A_DEC_LIST);
							(yyval.node) -> s1 = (yyvsp[0].node);
						}
#line 1514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 188 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, "Declaration DeclarationList Production Rule \n");
							(yyval.node) = ASTCreateNode(A_DEC_LIST);
							(yyval.node) -> s1 = (yyvsp[-1].node);
							(yyval.node) -> s2 = (yyvsp[0].node);
						}
#line 1524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 198 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Declaration VarDeclaration Production Rule\n");
							(yyval.node) = (yyvsp[0].node);
						}
#line 1532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 202 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Declaration FunDeclaration Production Rule\n");
							(yyval.node) = (yyvsp[0].node);
						}
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 212 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " VarDeclaration Production Rule\n");
							(yyval.node) = (yyvsp[-1].node);
							(yyval.node) -> datatype = (yyvsp[-2].datatype);

							/*set datatype*/
							ASTnode *p;
							p = (yyvsp[-1].node);
							while (p != NULL){
								p->symbol->Declared_Type = (yyvsp[-2].datatype);
								p = p->s1;
							} //end while
						}
#line 1557 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 238 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " VarList T_ID Production Rule\n");
							/*fprintf(stderr, "VarList T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);*/

							if (Search((yyvsp[0].string), level, 0) == NULL){
								//symbol not there, put it in the symbol table
								(yyval.node) = ASTCreateNode(A_VARDEC);
								(yyval.node) -> name = (yyvsp[0].string);
								(yyval.node) -> value = 0;
								(yyval.node) -> symbol = Insert((yyvsp[0].string), A_UNKNOWN, SYM_SCALAR, level, 1, offset);
								offset = offset + 1;
							}
							else {
								yyerror((yyvsp[0].string));
								yyerror("Already defined");
								exit(1);
							}
						}
#line 1580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 258 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " VarList T_ID [T_NUM] Production Rule\n");
							/*fprintf(stderr, "VarList T_ID [T_NUM] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);*/
							if (Search((yyvsp[-3].string), level, 0) == NULL){
								//symbol not there, put it in the symbol table
								(yyval.node) = ASTCreateNode(A_VARDEC);
								(yyval.node) -> name = (yyvsp[-3].string);
								(yyval.node) -> value = (yyvsp[-1].value);
								(yyval.node) -> symbol = Insert((yyvsp[-3].string), A_UNKNOWN, SYM_ARRAY, level, (yyvsp[-1].value), offset);
								offset = offset + (yyvsp[-1].value);
							}
							else {
								yyerror((yyvsp[-3].string));
								yyerror("Already defined");
								exit(1);
							}
						}
#line 1602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 277 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
								fprintf(stderr, " VarList T_ID , VarList Production Rule\n");
								fprintf(stderr, "VarList T_ID , VarList Production Rule\n");
								fprintf(stderr, "	T_ID: %s\n", (yyvsp[-2].node));
							}
							if (Search((yyvsp[0].string), level, 0) == NULL){
								//symbol not there, put it in the symbol table
								(yyval.node) = ASTCreateNode(A_VARDEC);
								(yyval.node) -> name = (yyvsp[0].string);
								(yyval.node) -> s1 = (yyvsp[-2].node);
								(yyval.node) -> value = 0;
								(yyval.node) -> symbol = Insert((yyvsp[0].string), A_UNKNOWN, SYM_SCALAR, level, 1, offset);
								offset = offset + 1;
							}
							else {
								yyerror((yyvsp[-2].node));
								yyerror("Already defined");
								exit(1);
							}
						}
#line 1627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 299 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
								fprintf(stderr, " VarList T_ID [T_NUM], Varlist Production Rule\n");
								fprintf(stderr, "VarList T_ID [T_NUM], Varlist Production Rule\n");
								fprintf(stderr, "	T_ID: %s\n", (yyvsp[-5].node));
							}

							if (Search((yyvsp[-3].string), level, 0) == NULL){
								//symbol not there, put it in the symbol table
								(yyval.node) = ASTCreateNode(A_VARDEC);
								(yyval.node) -> name = (yyvsp[-3].string);
								(yyval.node) -> value = (yyvsp[-1].value);
								(yyval.node) -> s1 = (yyvsp[-5].node);
								(yyval.node) -> symbol = Insert((yyvsp[-3].string), A_UNKNOWN, SYM_ARRAY, level, (yyvsp[-1].value), offset);
								offset = offset + (yyvsp[-1].value);
							}
							else {
								yyerror((yyvsp[-5].node));
								yyerror("Already defined");
								exit(1);
							}
						}
#line 1653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 326 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " TypeSpecifier T_INT Production Rule\n");
							(yyval.datatype) = A_INTTYPE;
						}
#line 1661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 330 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " TypeSpecifier T_VOID Production Rule\n");
							(yyval.datatype) = A_VOIDTYPE;
						}
#line 1669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 334 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " TypeSpecifier T_BOOLEAN Production Rule\n");
							(yyval.datatype) = A_BOOLEANTYPE;
						}
#line 1677 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 352 "lab9.y" /* yacc.c:1646  */
    {	//check to see if function name is known
						// insert into symbol table unless it already exists
						//manage offset values

						struct SymbTab *p;
						p = Search((yyvsp[-1].string), level, 0);

						if (p == NULL){
							Insert((yyvsp[-1].string), (yyvsp[-2].datatype), SYM_FUNCTION, level, 0, 0);
						}

						else{
							if (p -> SubType == SYM_FUNCTION_PRE){
								p -> SubType = SYM_FUNCTION_PROTO;
							}

							else {
							printf("FUNCTION SUBTYPE:: %d\n", p -> SubType);
							yyerror((yyvsp[-1].string));
							yyerror("Can't create function. Name in use \n");
							exit(1);}

						}
						goffset = offset;
						offset = 2; 								//2 for SP and RA
						maxoffset = offset;							//maxoffset = largest amount of data needed
					}
#line 1709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 381 "lab9.y" /* yacc.c:1646  */
    {
						struct SymbTab *p;
						p = Search((yyvsp[-4].string), level, 0);

						if (p -> SubType == SYM_FUNCTION_PROTO){
							//check params
							if (proto_func_check(p->fparms, (yyvsp[-1].node)) == 0){
								yyerror("Function cannot be declared. Parameters do not match.");
								exit(1);
							}
							//p -> SubType = SYM_FUNCTION;
						}

						else {
							//update symbtable with parameter
							// allows us to have recursive functions
							Search((yyvsp[-4].string), level, 0)->fparms = (yyvsp[-1].node);
						}
					}
#line 1733 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 402 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " FunDeclaration Production Rule\n");
							fprintf(stderr, "FunDeclaration Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[-6].string));}

						struct SymbTab *p;
						p = Search((yyvsp[-6].string), level, 0);

						(yyval.node) = (yyvsp[0].node);
						(yyval.node) -> name = (yyvsp[-6].string);
						(yyval.node) -> datatype = (yyvsp[-7].datatype);
						(yyval.node) -> s1 = (yyvsp[-3].node);
						(yyval.node) -> symbol = Search((yyvsp[-6].string), level, 0);
						(yyval.node) -> symbol -> offset = maxoffset;
						offset = goffset;
						maxoffset = offset;

						if(p->SubType == SYM_FUNCTION_PROTO){
							if ((yyval.node)->s2 == NULL){
								yyerror("Failed to add function body.");
								exit(1);
							}
							p->SubType = SYM_FUNCTION;
						}

						if ((yyval.node) -> s2 == NULL){
							(yyval.node) -> symbol -> SubType = SYM_FUNCTION_PRE;
							(yyval.node) -> symbol -> offset = 0;
							//deleting the symbols from the symbol table will allow us to create the function and
							Delete(level+1);
						}
					}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 442 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " PrototypeDeclaration Production Rule\n");
							fprintf(stderr, "PrototypeDeclaration Production Rule\n");
							}
							(yyval.node) = ASTCreateNode(A_PROTODEC);
					}
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 450 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " PrototypeDeclaration Production Rule\n");
							fprintf(stderr, "PrototypeDeclaration Production Rule\n");
							}
							(yyval.node) = ASTCreateNode(A_FUNDEC);
							(yyval.node) -> s2 = (yyvsp[0].node);
					}
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 467 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Params T_VOID Production Rule\n");
						(yyval.node) = ASTCreateNode(A_PARAMS);
						(yyval.node) -> datatype = A_VOIDTYPE;
					}
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 472 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Params ParamList Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1810 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 484 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ParamList Param Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1818 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 489 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ParamList Param , ParamList Production Rule\n");
						(yyval.node) = (yyvsp[-2].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
					}
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 503 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " Param TypeSpecifier T_ID Production Rule\n");
							fprintf(stderr, "Param TypeSpecifier T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[0].string));
						}
						(yyval.node) = ASTCreateNode(A_PARAM);
						(yyval.node) -> name = (yyvsp[0].string);
						(yyval.node) -> datatype = (yyvsp[-1].datatype);
						(yyval.node) -> value = 0;

						if (Search((yyvsp[0].string), 1, 0) == NULL){
							(yyval.node) -> symbol = Insert((yyvsp[0].string), (yyvsp[-1].datatype), SYM_SCALAR, 1, 1, offset);
							offset = offset + 1;
							}
						else {
								yyerror((yyvsp[0].string));
								yyerror("Already defined parameter");
								exit(1);
							}
					}
#line 1852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 525 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " Param TypeSpecifier T_ID [] Production Rule\n");
							fprintf(stderr, "Param TypeSpecifier T_ID [] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[-2].string));
						}

						(yyval.node) = ASTCreateNode(A_PARAM);
						(yyval.node) -> name = (yyvsp[-2].string);
						(yyval.node) -> datatype = (yyvsp[-3].datatype);
						(yyval.node) -> value = -1;

						if (Search((yyvsp[-2].string), 1, 0) == NULL){
							(yyval.node) -> symbol = Insert((yyvsp[-2].string), (yyvsp[-3].datatype), SYM_ARRAY, 1, 1, offset);
							(yyval.node) -> symbol -> param = 1;
							offset = offset + 1;
							}
						else {
								yyerror((yyvsp[-2].string));
								yyerror("Already defined parameter");
								exit(1);
							}
					}
#line 1879 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 559 "lab9.y" /* yacc.c:1646  */
    {
						level = level + 1;
					}
#line 1887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 564 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " CompoundStmt Production Rule\n");
						(yyval.node) = ASTCreateNode(A_COMPOUND);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[-1].node);
						if (mydebug) Display();
						if (offset > maxoffset)
							maxoffset = offset;
						offset = offset - Delete(level);
						level = level - 1;
					}
#line 1902 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 582 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " LocalDeclarations Production Rule\n");
						(yyval.node) = NULL;
					}
#line 1910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 587 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " LocalDeclarations VarDeclaration LocalDeclarations Production Rule\n");
						(yyval.node) = ASTCreateNode(A_LOCALDEC);
						(yyval.node)->s1 = (yyvsp[-1].node);
						(yyval.node) -> s2 = (yyvsp[0].node);}
#line 1919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 598 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " StatementList Production Rule\n");
						(yyval.node) = NULL;
					}
#line 1927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 603 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " StatementList Statement StatementList Production Rule\n");
						(yyval.node) = ASTCreateNode(A_STMTLIST);
						(yyval.node) -> s1 = (yyvsp[-1].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
					}
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 612 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement ExpressionStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1945 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 617 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement CompoundStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 622 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement SelectionStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 627 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement IterationStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1969 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 632 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement AssignmentStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 637 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement ReturnStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 642 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement ReadStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 647 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement WriteStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 652 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement ContinueStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2009 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 657 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Statement BreakStmt Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2017 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 667 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ExpressionStmt Expression Production Rule\n");
						(yyval.node) = ASTCreateNode(A_EXPRSTMT);
						(yyval.node) -> s1 = (yyvsp[-1].node);
						if (var_check((yyvsp[-1].node)) == 0){
							yyerror("Variable use invalid");
							exit(1);
						}
					}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 677 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " \n");
						(yyval.node) = ASTCreateNode(A_EXPRSTMT);
					}
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 690 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " SelectionStmt T_IF Production Rule\n");
						(yyval.node) = ASTCreateNode(A_SELECTSTMT);
						ASTnode* ifBody = ASTCreateNode(A_IFBODY);
						ifBody-> s1 = (yyvsp[-1].node);
						(yyval.node) -> s2 = ifBody;
						(yyval.node) -> s1 = (yyvsp[-3].node);
					}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 699 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " SelectionStmt T_IF T_ELSE Production Rule\n");
						(yyval.node) = ASTCreateNode(A_SELECTSTMT);
						ASTnode* ifBody = ASTCreateNode(A_IFBODY);
						ifBody -> s1 = (yyvsp[-3].node);
						ifBody -> s2 = (yyvsp[-1].node);
						(yyval.node) ->s1 = (yyvsp[-5].node);
						(yyval.node) -> s2 = ifBody;
						}
#line 2063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 714 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " IterationStmt Production Rule\n");
						(yyval.node) = ASTCreateNode(A_ITERSTMT);
						(yyval.node) -> s1 = (yyvsp[-2].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
					}
#line 2073 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 726 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ReturnStmt T_RETURN Production Rule\n");
						(yyval.node) = ASTCreateNode(A_RETURN);
					}
#line 2081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 730 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ReturnStmt T_RETURN Expression Production Rule\n");
						(yyval.node) = ASTCreateNode(A_RETURN);
						(yyval.node) -> s1 = (yyvsp[-1].node);
					}
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 741 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ReadStmt Production Rule\n");
						(yyval.node) = ASTCreateNode(A_READ);
						(yyval.node) -> s1 = (yyvsp[-1].node);
					}
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 752 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " WriteStmt T_WRITE Expression Production Rule\n");

						if (var_check((yyvsp[-1].node)) == 0){
							yyerror("Variable use invalid");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_WRITE);
						(yyval.node)->s1 = (yyvsp[-1].node);
					}
#line 2114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 764 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
						fprintf(stderr, " WriteStmt T_WRITE T_STRING Production Rule\n");
						fprintf(stderr, "WriteStmt T_WRITE T_STRING Production Rule\n");
						fprintf(stderr, "	T_STRING: %s\n", (yyvsp[-1].string));
						}

						(yyval.node) = ASTCreateNode(A_WRITE);
						(yyval.node)->name = (yyvsp[-1].string);
						(yyval.node)->nodeLabel = CreateLabel();
					}
#line 2129 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 784 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " AssignmentStmt Production Rule\n");

						if ((yyvsp[-3].node) -> datatype != (yyvsp[-1].node) -> datatype){
							yyerror("Type mismatch in assignment statement.");
							exit(1);
						}

						if ((yyvsp[-3].node)->value == -1 || (yyvsp[-1].node)->value == -1){
							yyerror("Misuse of subtypes in either the left or right hand side of the equation.");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_ASSIGN);
						(yyval.node) -> s1 = (yyvsp[-3].node);
						(yyval.node) -> s2 = (yyvsp[-1].node);
						(yyval.node) -> datatype = (yyvsp[-3].node) -> datatype;
						(yyval.node) -> symbol = Insert(CreateTemp(), (yyval.node) -> datatype, SYM_SCALAR, level, 1, offset++);
					}
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 808 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ContinueStmt Production Rule\n");
						(yyval.node) = ASTCreateNode(A_CONT);
					}
#line 2160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 817 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " BreakStmt Production Rule\n");
						(yyval.node) = ASTCreateNode(A_BREAK);
					}
#line 2168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 824 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Expression Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 837 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug){
							fprintf(stderr, " Var T_ID Production Rule\n");
							fprintf(stderr, "Var T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[0].string));
						}

						//if not in symbol table, barf because in order to use, it must be defined
						//if it's the wrong subt-type, barf
						struct SymbTab * p;
						p = Search((yyvsp[0].string), level, 1);

						if (p == NULL){
							yyerror((yyvsp[0].string));
							yyerror("Variable used but not defined.");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_VAR);
						(yyval.node) -> name = (yyvsp[0].string);
						(yyval.node) -> symbol = p;
						(yyval.node) -> value = 0;
						(yyval.node) -> datatype = p -> Declared_Type;

						if (p->SubType == SYM_ARRAY){
							(yyval.node) -> value = -1;
						}
					}
#line 2208 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 866 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " Var T_ID [Expression] Production Rule\n");
					 		fprintf(stderr, "Var T_ID [Expression] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[-3].string));
						}

						struct SymbTab * p;
						p = Search((yyvsp[-3].string), level, 1);

						if (p == NULL){
							yyerror((yyvsp[-3].string));
							yyerror("Variable used but not defined.");
							exit(1);
						}

						if ((yyvsp[-1].node) -> datatype != A_INTTYPE){
							yyerror("Type missmatch used for array length ");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_VAR);
						(yyval.node) -> name = (yyvsp[-3].string);
						(yyval.node) -> s1 = (yyvsp[-1].node);
						(yyval.node) -> symbol = p;
						(yyval.node) -> datatype = p -> Declared_Type;

					}
#line 2240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 909 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " SimpleExpression AdditiveExpression Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 914 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " SimpleExpression Relop AdditiveExpression Production Rule\n");

						if ((yyvsp[-2].node)->datatype == A_VOIDTYPE){
							yyerror("Type missmatch in expression. Cannot assign to void type variable.");
							exit(1);
						}//endif

						if( (yyvsp[-2].node)->datatype != (yyvsp[0].node)->datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check((yyvsp[-2].node)) == 0 || var_check((yyvsp[0].node)) == 0){
							yyerror("Input variable is invalid.");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node) -> s1 = (yyvsp[-2].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
						(yyval.node) -> operator = (yyvsp[-1].operator);
						(yyval.node) -> datatype = A_BOOLEANTYPE;
						(yyval.node) -> symbol = Insert(CreateTemp(), (yyval.node) -> datatype, SYM_SCALAR, level, 1, offset++);
					}
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 945 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_LE Production Rule\n");
						(yyval.operator) = A_LE;
					}
#line 2285 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 950 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_GR Production Rule\n");
						(yyval.operator) = A_GR;
					}
#line 2293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 955 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_LEEQU Production Rule\n");
						(yyval.operator) = A_LEEQU;
					}
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 960 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_GREQU Production Rule\n");
						(yyval.operator) =A_GREQU;
					}
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 965 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_NEQU Production Rule\n");
						(yyval.operator) = A_NEQU;
					}
#line 2317 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 970 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Relop T_EQUAL Production Rule\n");
						(yyval.operator) = A_EQUAL;
					}
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 988 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " AdditiveExpression Term Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 993 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " AdditiveExpression Addop Term Production Rule\n");
						if( (yyvsp[-2].node)->datatype != (yyvsp[0].node)->datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check((yyvsp[-2].node)) == 0 || var_check((yyvsp[0].node)) == 0){
							yyerror("Input variable is invalid in additive expression.");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node) -> s1 = (yyvsp[-2].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
						(yyval.node) -> operator = (yyvsp[-1].operator);
						(yyval.node) -> datatype = (yyvsp[-2].node) -> datatype;
						(yyval.node) -> symbol = Insert(CreateTemp(), (yyval.node) -> datatype, SYM_SCALAR, level, 1, offset++);
					}
#line 2356 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1018 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Addop T_PLUS Production Rule\n");
						(yyval.operator) = A_PLUS;
					}
#line 2364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1023 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Addop T_MINUS Production Rule\n");
						(yyval.operator) = A_MINUS;
					}
#line 2372 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1041 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Term Factor Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2380 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1046 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Term Multop Factor Production Rule\n");

						if ((yyvsp[-2].node) -> datatype != (yyvsp[0].node) -> datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check((yyvsp[-2].node)) == 0 || var_check((yyvsp[0].node)) == 0){
							yyerror((yyvsp[-2].node));
							yyerror("Input variable is invalid in term.");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node) -> s1 = (yyvsp[-2].node);
						(yyval.node) -> s2 = (yyvsp[0].node);
						(yyval.node) -> operator = (yyvsp[-1].operator);
						(yyval.node) -> datatype = (yyvsp[-2].node) -> datatype;
						(yyval.node) -> symbol = Insert(CreateTemp(), (yyval.node) -> datatype, SYM_SCALAR, level, 1, offset++);
					}
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1073 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Multop T_MUL Production Rule\n");
						(yyval.operator) = A_MUL;
					}
#line 2413 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1078 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Multop T_DIV Production Rule\n");
						(yyval.operator) = A_DIV;
					}
#line 2421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1083 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Multop T_OR Production Rule\n");
						(yyval.operator) = A_OR;
					}
#line 2429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1087 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Multop T_AND Production Rule\n");
						(yyval.operator) = A_AND;
					}
#line 2437 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1102 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor (Expression) Production Rule\n");
						(yyval.node) = (yyvsp[-1].node);
					}
#line 2445 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1107 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor T_NUM Production Rule\n");
						(yyval.node) = ASTCreateNode(A_NUM);
						(yyval.node) -> value = (yyvsp[0].value);
						(yyval.node) -> datatype = A_INTTYPE;
					}
#line 2455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1114 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor Var Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1119 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor Call Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1124 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor T_TRUE Production Rule\n");
						(yyval.node) = ASTCreateNode(A_FACTOR);
						(yyval.node) -> name = "TRUE";
						(yyval.node) -> value = 1;
						(yyval.node) -> datatype = A_BOOLEANTYPE;
					}
#line 2482 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1132 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor T_FALSE Production Rule\n");
						(yyval.node) = ASTCreateNode(A_FACTOR);
						(yyval.node) -> name = "FALSE";
						(yyval.node) -> value = 0;
						(yyval.node) -> datatype = A_BOOLEANTYPE;
					}
#line 2493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1140 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Factor T_NOT Factor Production Rule\n");
						if ((yyvsp[0].node) -> datatype != A_BOOLEANTYPE){
							yyerror("NOT operator expects boolean.");
							exit(1);
						}

						// if (var_check($2) == 0){
						// 	yyerror($1);
						// 	yyerror("Input variable is invalid in NOT operator.");
						// 	exit(1);
						// }

						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node) -> operator = A_NOT;
						(yyval.node) -> s1 = (yyvsp[0].node);
						(yyval.node) -> datatype = (yyvsp[0].node) -> datatype;

					}
#line 2516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1173 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) {
							fprintf(stderr, " Call Production Rule\n");
							fprintf(stderr, "Call Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", (yyvsp[-3].string));
						}

						//check if in symbtable
						struct SymbTab *p;

						p = Search((yyvsp[-3].string), 0, 0);
						if (p == NULL){
							yyerror((yyvsp[-3].string));
							yyerror("Function not declared in symbol table. ");
							exit(1);
						}

						//if symbol is not a function
						if (p-> SubType == SYM_FUNCTION || p-> SubType == SYM_FUNCTION_PROTO){}
						else{
							yyerror((yyvsp[-3].string));
							yyerror("Function name is not defined as a function. ");
							exit(1);
						}

						//if no parameters were passed and $3 is NULL
						//Cannot pass $3 into check_params
						if (((yyvsp[-1].node) == NULL) && (p->fparms->s2 != NULL)){
							yyerror((yyvsp[-3].string));
							yyerror("Incorrect amount of parameters used.");
							exit(1);
						}

						//check to see if actual and formal parameters match in length and type
						if (check_params(p->fparms, (yyvsp[-1].node)) == 0){
							yyerror((yyvsp[-3].string));
							yyerror("Parameter usage incorrect. ");
							exit(1);
						}

						(yyval.node) = ASTCreateNode(A_CALL);
						(yyval.node) -> name = (yyvsp[-3].string);
						(yyval.node) -> s1 = (yyvsp[-1].node);
						(yyval.node) -> symbol = p;
						(yyval.node) -> datatype = (yyval.node) -> symbol -> Declared_Type;

					}
#line 2567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1228 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Args Production Rule\n");
						(yyval.node) = NULL;
					}
#line 2575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1233 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " Args ArgList Production Rule\n");
						(yyval.node) = (yyvsp[0].node);
					}
#line 2583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1245 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ArgList Expression Production Rule\n");
							(yyval.node) = ASTCreateNode(A_ARGLIST);
							(yyval.node) -> s1 = (yyvsp[0].node);
							(yyval.node) -> name = (yyvsp[0].node) -> name;
							(yyval.node) -> value = (yyvsp[0].node) -> value;
							(yyval.node) -> datatype = (yyvsp[0].node) -> datatype;
							(yyval.node)->symbol = Insert(CreateTemp(), (yyval.node)->datatype, SYM_SCALAR, level, 1, offset++);

					}
#line 2597 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1256 "lab9.y" /* yacc.c:1646  */
    {	if(mydebug) fprintf(stderr, " ArgList Expression, ArgList Production Rule\n");

							(yyval.node) = ASTCreateNode(A_ARGLIST);
							(yyval.node) -> s1 = (yyvsp[-2].node);
							(yyval.node) -> s2 = (yyvsp[0].node);
							(yyval.node) -> name = (yyvsp[-2].node) -> name;
							(yyval.node) -> value = (yyvsp[-2].node) -> value;
							(yyval.node) -> datatype = (yyvsp[-2].node) -> datatype;
							(yyval.node)->symbol = Insert(CreateTemp(), (yyval.node)->datatype, SYM_SCALAR, level, 1, offset++);
					}
#line 2612 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2616 "y.tab.c" /* yacc.c:1646  */
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
#line 1268 "lab9.y" /* yacc.c:1906  */
	/* end of rules, start of program */

/*  * @precondition: None
    * @postcondition: None
    * @param: argc - number of arguments given to program
	* @param: argv - name of arguments given to program
	* @return: integer
    * @description: This runs the program*/
int main(int argc, char* argv[])
{ //start main function

	FILE *fp = NULL;
	char s[100];

	for(int i = 1; i < argc; i++){
		if (strcmp("-d", argv[i]) == 0){
			mydebug = 1;
		}//endif

		if (strcmp("-o", argv[i]) == 0){
			//assume that argv[i+1] is on the line & we will not check
			strcpy(s, argv[i+1]);
			strcat(s, ".asm");

			//now you have the file name
			if (mydebug) printf("Opening file %s\n", s);

			fp = fopen(s, "w");
			if (fp == NULL){
				printf("Unablt to open %s\n", s);
				exit(1);
			}//endif
		}//endif stcmp -o
	}//end for loop

	if (fp == NULL){
		printf("No file name given, must use -o option.\n");
		exit(1);
	}//endif

	yyparse();

	if (mydebug) Display();
	if (mydebug) ASTprint(0, program);

	EMIT(program, fp);

} //end main function
