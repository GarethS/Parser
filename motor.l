
/* Copyright (c) 2013 Gareth Scott */

%option noyywrap
%option yylineno
/* Remove warning, yyunput defined but not used */
%option nounput
/* Remove warning, input defined but not used */
%option noinput 

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "compiler.h"
//#include "motor.tab.h"	// To see the %union in valve.y  Generated by bison with -d option

#define ECHO                // Remove any attempt to print out extraneous characters in embedded system
void motorFatalError(const char msg[]);
#define YY_FATAL_ERROR(msg)     motorFatalError(msg)
#define YY_INPUT(a,b,c)         ((void)0)

YYSTYPE yylval;
%}

digit			[0-9]
float           -?{digit}+\.{digit}+
/* Using [A-Za-z0-9] will cause 'a[x]' to be lexed as an alphanum. Don't want the [ character to be interpreted as a character. */
alpha			[A-Za-z]
alphanum		[A-Za-z0-9]
period			[.]
eol             [\n]
%x		comment
%x 		commentNew

%%
"<VERS>"            {return VERSION_BEGIN;}
"<vers>"            {return VERSION_END;}
"<STMT>"            {return STATEMENT_BEGIN;}
"<stmt>"            {return STATEMENT_END;}
"<SYMB>"            {return SYMBOL_BEGIN;}
"<symb>"            {return SYMBOL_END;}
"ROOT"     	        {return ROOT;}
"LEFT"     	        {return LEFT;}
"RIGHT"             {return RIGHT;}
"Variable"          {return VARIABLE;}
"Operator"          {return OPERATOR;}
"If"                {return IF;}
"EVAL0"             {return EVAL0;}
"EVALWHILE0"        {return EVALWHILE0;}
"Else"              {return ELSE;}
"EndIf"             {return ENDIF;}
"While"             {return WHILE;}
"EndWhile"          {return ENDWHILE;}
"FunctionCall"      {return FUNCTIONCALL;}
"FunctionCallEnd"   {return FUNCTIONCALLEND;}
"JmpEndIf"          {return JMPENDIF;}
"Start"             {return START;}
"ProgramEnd"        {return PROGRAMEND;}
-?{digit}+		    {yylval.integer = atoi(yytext); /*printf("yikes!%s,%d", yytext, yylval.integer);*/ return CONST;}	/* constant integer */
{eol}       {}  /* Need this to get yylineno incrementing. */

"/*"				BEGIN(comment);

<comment>[^*\n]*	/* eat anything that's not a '*' */

<comment>"*/"		BEGIN(INITIAL);

"//"				BEGIN(commentNew);

<commentNew>.		/* eat everything until '\n' */

<commentNew>[\n]	BEGIN(INITIAL);


%%
/* Start of user defined code section. */

extern int yylex();

void motorFatalError(const char msg[]) {
    // Do nothing for now. In embedded version set error flag.
}

// Used by motor.yy.c and we're not associated with a terminal
int isatty(int x) {return 0;}

/* End of user defined code section. */