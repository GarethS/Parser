
/* Copyright (c) 2011, Gareth Scott */

%option noyywrap

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
alpha			[A-Za-z]
alphanum		[A-za-z0-9]
lparen			[(]
rparen			[)]
lbrace			[{]
rbrace			[}]
comma			[,]
semi			[;]
bang			[!]
period			[.]
equal			"="
plus			[+]
minus			[-]
mult			[*]
div				[/]
xor				"^"
geq				">="
leq				"<="
neq				"!="
gtr				[>]
lss				"<"
or				"||"
and				"&&"
testForEqual	"=="
%x		comment
%x 		commentNew

%%
outputs		{debugPrintf("OUTPUTS"); return OUTPUTS;}
inputs	 	{debugPrintf("INPUTS"); return INPUTS;}
{lbrace}	{debugPrintf("LBRACE"); return LBRACE;}
{rbrace}	{debugPrintf("RBRACE"); return RBRACE;}
{lparen}	{debugPrintf("LPAREN"); return LPAREN;}
{rparen}	{debugPrintf("RPAREN"); return RPAREN;}
{comma}		{debugPrintf("COMMA"); return COMMA;}
{semi}		{debugPrintf("SEMI"); return SEMI;}
{bang}		{debugPrintf("BANG"); yylval.string = strdup(yytext); return BANG;}
{equal}		{debugPrintf("EQUAL"); return EQUAL;}
{plus}		{debugPrintf("PLUS"); return PLUS;}
{minus}		{debugPrintf("MINUS"); return MINUS;}
{mult}		{debugPrintf("MULT"); return MULT;}
{div}		{debugPrintf("DIV"); return DIV;}
{xor}		{debugPrintf("XOR"); return XOR;}
{geq}		{debugPrintf("GEQ"); return GEQ;}
{leq}		{debugPrintf("LEQ"); return LEQ;}
{neq}		{debugPrintf("NEQ"); return NEQ;}
{gtr}		{debugPrintf("GTR"); return GTR;}
{lss}		{debugPrintf("LSS"); return LSS;}
{or}		{debugPrintf("OR"); return OR;}
{and}  		{debugPrintf("AND"); return AND;}
{testForEqual}	{debugPrintf("TEST_FOR_EQUAL"); return TEST_FOR_EQUAL;}
{digit}+		{debugPrintf("DIGIT"); yylval.string = strdup(yytext); return CONST;}	/* constant */
{alpha}{alphanum}*	{debugPrintf("VAR"); yylval.string = strdup(yytext); return VAR;}	/* variable */
{alpha}{alphanum}*{period}{alpha}{alphanum}*	{debugPrintf("VAR_METHOD"); yylval.string = strdup(yytext); return VAR_METHOD;}	/* method */

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
    yyin = fopen( "valve2.def", "r" );
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
