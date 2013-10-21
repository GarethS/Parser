
/* Copyright (c) 2011, 2012, 2013 Gareth Scott */

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

#include "compiler.h"
#include "valve.tab.h"	// To see the %union in valve.y

void debugPrintf(const char* pc);

YYSTYPE yylval;
%}

line			[A-Ha-h][0-7]
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
outputs		{debugPrintf("OUTPUTS"); return OUTPUTS;}
inputs	 	{debugPrintf("INPUTS"); return INPUTS;}
"{"     	{debugPrintf("LBRACE"); return LBRACE;}
"}"     	{debugPrintf("RBRACE"); return RBRACE;}
"["         {debugPrintf("LBRACKET"); return LBRACKET;}
"]"         {debugPrintf("RBRACKET"); return RBRACKET;}
"(" 	    {debugPrintf("LPAREN"); return LPAREN;}
")"     	{debugPrintf("RPAREN"); return RPAREN;}
"," 		{debugPrintf("COMMA"); return COMMA;}
";" 		{debugPrintf("SEMI"); return SEMI;}
"~" 		{debugPrintf("TILDE"); yylval.string = strdup(yytext); return TILDE;}
"!" 		{debugPrintf("BANG"); yylval.string = strdup(yytext); return BANG;}
"=" 		{debugPrintf("EQUAL"); return EQUAL;}
"+" 		{debugPrintf("PLUS"); return PLUS;}
"-" 		{debugPrintf("MINUS"); return MINUS;}
"*" 		{debugPrintf("MULT"); return MULT;}
"/" 		{debugPrintf("DIV"); return DIV;}
"^" 		{debugPrintf("XOR"); return XOR;}
">="		{debugPrintf("GEQ"); return GEQ;}
"<="		{debugPrintf("LEQ"); return LEQ;}
"!="		{debugPrintf("NEQ"); return NEQ;}
">" 		{debugPrintf("GTR"); return GTR;}
"<" 		{debugPrintf("LSS"); return LSS;}
"<<" 		{debugPrintf("SHL"); return SHL;}
">>" 		{debugPrintf("SHR"); return SHR;}
"||"		{debugPrintf("OR"); return OR;}
"&&"  		{debugPrintf("AND"); return AND;}
"|"		    {debugPrintf("BITWISEOR"); return BITWISEOR;}
"&"  		{debugPrintf("BITWISEAND"); return BITWISEAND;}
"=="    	{debugPrintf("TEST_FOR_EQUAL"); return TEST_FOR_EQUAL;}
"ARRAY"     {debugPrintf("ARRAYDEFINE"); return ARRAYDEFINE;}
"if"	    {debugPrintf("IF"); return IF;}
"else"	    {debugPrintf("ELSE"); return ELSE;}
"while"	    {debugPrintf("WHILE"); return WHILE;}
"main"	    {debugPrintf("MAIN"); return MAIN;}
"moveAbsolute"	        {debugPrintf("MOVEABSOLUTE"); return MOVEABSOLUTE;}
"moveRelative"	        {debugPrintf("MOVERELATIVE"); return MOVERELATIVE;}
"sleep"	                {debugPrintf("SLEEP"); return SLEEP;}
"sleepUntil"            {debugPrintf("SLEEPUNTIL"); return SLEEPUNTIL;}
"led"                   {debugPrintf("LED"); return LED;}
"RPM"                   {debugPrintf("RPM"); return RPM;}
"RPMx10k"               {debugPrintf("RPMx10k"); return RPMx10k;}
"accelMicroSec"         {debugPrintf("ACCELMICROSEC"); return ACCELMICROSEC;}
"degreex10kAbsolute"    {debugPrintf("DEGREEx10kABSOLUTE"); return DEGREEx10kABSOLUTE;}
"degreex10kRelative"    {debugPrintf("DEGREEx10kRELATIVE"); return DEGREEx10kRELATIVE;}
-?{digit}+		{debugPrintf("CONST"); yylval.string = strdup(yytext); return CONST;}	/* constant integer */
-?{digit}+\.{digit}+		{debugPrintf("CONST_FLOAT"); yylval.floatingPoint = atof(yytext); return CONST_FLOAT;}	/* constant floating-point */
{alpha}{alphanum}*	{debugPrintf("VAR"); yylval.string = strdup(yytext); return VAR;}	/* variable */
{alpha}{alphanum}*{period}{alpha}{alphanum}*	{debugPrintf("VAR_METHOD"); yylval.string = strdup(yytext); return VAR_METHOD;}	/* method */
{eol}       {}  /* Need this to get yylineno incrementing. */

"/*"				BEGIN(comment);

<comment>[^*\n]*	/* eat anything that's not a '*' */

<comment>"*/"		BEGIN(INITIAL);

"//"				BEGIN(commentNew);

<commentNew>.		/* eat everything until '\n' */

<commentNew>[\n]	BEGIN(INITIAL);


%%
/* Start of user defined code section. */
/* If you're just trying to debug the lexical analyzer then uncomment
	the code containing 'main()' below and in the 'rules' section above
	replace the code in braces {} with something like: 'printf("line token");'.
	You won't have access to the macros LINE, OUTPUTS, etc. unless you link
	with 'port_tab.c' file outputted by bison.
*/


extern int yylex();

#if BISON_PHASE
#define ECHO_DEBUG	0
#else /* not BISON_PHASE */
#define ECHO_DEBUG	1
#endif /* BISON_PHASE */

#if !ECHO_DEBUG
// Turn off ECHO
#ifdef ECHO
#undef ECHO
#endif /* ECHO */
#define ECHO
#endif /* not ECHO_DEBUG */

void debugPrintf(const char* pc) {
// FLEX_DEBUG defined with -d command line option
#if ECHO_DEBUG
	printf("[%s]", pc);
#endif /* ECHO_DEBUG */
}

#if !BISON_PHASE
int main(int argc, char* argv[])
{
	printf("Start scanner\n"); fflush(stdout);
    yyin = fopen( "valve3.def", "r" );
	int r = yylex();
	while (r != 0) {
		ECHO;
		//printf("%d %s\n", r, yylval.string);
		r = yylex();
	}
	printf("\n\nEnd scanner"); fflush(stdout);
	return r;
	//return yylex();
}
#endif /* BISON_PHASE */

/* End of user defined code section. */
