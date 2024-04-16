/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 121 "lab9.y" /* yacc.c:1909  */

	int value;
	char* string;
	ASTnode* node;
	enum DataTypes datatype;
	enum OPERATORS operator;

#line 136 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
