/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
     MOVEABSOLUTE = 274,
     MOVERELATIVE = 275,
     SLEEP = 276,
     BITWISEOR = 277,
     BITWISEAND = 278,
     OR = 279,
     AND = 280,
     LSS = 281,
     GTR = 282,
     NEQ = 283,
     LEQ = 284,
     GEQ = 285,
     TEST_FOR_EQUAL = 286,
     LPAREN = 287,
     MINUS = 288,
     PLUS = 289,
     XOR = 290,
     DIV = 291,
     MULT = 292,
     LBRACKET = 293,
     RBRACKET = 294,
     RPAREN = 295,
     SEMI = 296
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 53 "valve.y"

	int integer;
    float floatingPoint;
	char* string;
	astNode* pSyntaxNode;



/* Line 1685 of yacc.c  */
#line 101 "valve.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


