/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    AND = 258,
    ASSIGN = 259,
    STAR = 260,
    COMMA = 261,
    DIV = 262,
    EQ = 263,
    GE = 264,
    GT = 265,
    LBRACE = 266,
    LE = 267,
    LPAR = 268,
    LSQ = 269,
    LT = 270,
    MINUS = 271,
    MOD = 272,
    NE = 273,
    NOT = 274,
    OR = 275,
    PLUS = 276,
    RBRACE = 277,
    RPAR = 278,
    RSQ = 279,
    SEMICOLON = 280,
    ARROW = 281,
    LSHIFT = 282,
    RSHIFT = 283,
    XOR = 284,
    DOT = 285,
    BOOL = 286,
    CLASS = 287,
    DOTLENGTH = 288,
    DOUBLE = 289,
    ELSE = 290,
    IF = 291,
    INT = 292,
    PRINT = 293,
    PARSEINT = 294,
    PUBLIC = 295,
    RETURN = 296,
    STATIC = 297,
    STRING = 298,
    VOID = 299,
    WHILE = 300,
    ASPAS = 301,
    ENTER = 302,
    TAB = 303,
    INVALID_ESCAPE = 304,
    SPACE = 305,
    EXPOENTE = 306,
    INTLIT = 307,
    ID = 308,
    REALLIT = 309,
    RESERVED = 310,
    STRLIT = 311,
    BOOLLIT = 312,
    IFX = 313
  };
#endif
/* Tokens.  */
#define AND 258
#define ASSIGN 259
#define STAR 260
#define COMMA 261
#define DIV 262
#define EQ 263
#define GE 264
#define GT 265
#define LBRACE 266
#define LE 267
#define LPAR 268
#define LSQ 269
#define LT 270
#define MINUS 271
#define MOD 272
#define NE 273
#define NOT 274
#define OR 275
#define PLUS 276
#define RBRACE 277
#define RPAR 278
#define RSQ 279
#define SEMICOLON 280
#define ARROW 281
#define LSHIFT 282
#define RSHIFT 283
#define XOR 284
#define DOT 285
#define BOOL 286
#define CLASS 287
#define DOTLENGTH 288
#define DOUBLE 289
#define ELSE 290
#define IF 291
#define INT 292
#define PRINT 293
#define PARSEINT 294
#define PUBLIC 295
#define RETURN 296
#define STATIC 297
#define STRING 298
#define VOID 299
#define WHILE 300
#define ASPAS 301
#define ENTER 302
#define TAB 303
#define INVALID_ESCAPE 304
#define SPACE 305
#define EXPOENTE 306
#define INTLIT 307
#define ID 308
#define REALLIT 309
#define RESERVED 310
#define STRLIT 311
#define BOOLLIT 312
#define IFX 313

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 31 "jucompiler.y" /* yacc.c:1921  */

	struct n* no;
	struct type_struct* t;	
	struct _args* args;

#line 180 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
