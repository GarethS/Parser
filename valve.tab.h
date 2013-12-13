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
     WAITFORIDLE = 283,
     PRINTNUMBER = 284,
     GETINPUT = 285,
     SETOUTPUT = 286,
     GETADC = 287,
     GETTEMP = 288,
     GETPOSITION = 289,
     RESET = 290,
     BITWISEOR = 291,
     BITWISEAND = 292,
     OR = 293,
     AND = 294,
     SHR = 295,
     SHL = 296,
     LSS = 297,
     GTR = 298,
     NEQ = 299,
     LEQ = 300,
     GEQ = 301,
     TEST_FOR_EQUAL = 302,
     LPAREN = 303,
     MINUS = 304,
     PLUS = 305,
     XOR = 306,
     DIV = 307,
     MULT = 308,
     LBRACKET = 309,
     RBRACKET = 310,
     RPAREN = 311,
     SEMI = 312,
     BANG = 313,
     TILDE = 314
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 71 "valve.y"

	int integer;
    float floatingPoint;
	char* string;
	astNode* pSyntaxNode;



/* Line 1685 of yacc.c  */
#line 119 "valve.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


