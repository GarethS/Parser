
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "valve.tab.h"	// To see the %union in valve.y

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
equal			[=]
plus			[+]
minus			[-]
mult			[*]
div				[/]
xor				"^"
geq				">="
leq				"<="
neq				"!="
gtr				[>]
lss				[<]
or				"||"
and				"&&"
testForEqual	"=="
%x		comment

%%
outputs		{return OUTPUTS;}
inputs	 	{return INPUTS;}
{lbrace}	{return LBRACE;}
{rbrace}	{return RBRACE;}
{lparen}	{return LPAREN;}
{rparen}	{return RPAREN;}
{comma}		{return COMMA;}
{semi}		{return SEMI;}
{bang}		{yylval.string = strdup(yytext); return BANG;}
{equal}		{return EQUAL;}
{plus}		{return PLUS;}
{minus}		{return MINUS;}
{mult}		{return MULT;}
{div}		{return DIV;}
{xor}		{return XOR;}
{geq}		{return GEQ;}
{leq}		{return LEQ;}
{neq}		{return NEQ;}
{gtr}		{return GTR;}
{lss}		{return LSS;}
{or}		{return OR;}
{and}  		{return AND;}
{testForEqual}	{return TEST_FOR_EQUAL;}
{digit}+		{yylval.number = atoi(yytext); /*ECHO;*/ return CONST;}	/* constants */
{alpha}{alphanum}*	{yylval.string = strdup(yytext); /*ECHO;*/ return VAR;}	/* variable */
{alpha}{alphanum}*{period}{alpha}{alphanum}*	{yylval.string = strdup(yytext); return VAR_METHOD;}	/* method */

"/*"				BEGIN(comment);

<comment>[^*\n]*	/* eat anything that's not a '*' */

<comment>"*/"		BEGIN(INITIAL);

%%
/* Start of user defined code section. */
/* If you're just trying to debug the lexical analyzer then uncomment
	the code containing 'main()' below and in the 'rules' section above
	replace the code in braces {} with something like: 'printf("line token");'.
	You won't have access to the macros LINE, OUTPUTS, etc. unless you link
	with 'port_tab.c' file outputted by bison.
*/


extern int yylex();

int main( argc, argv )
int argc;
char *argv[];
	{
	printf("start scanner"); fflush(stdout);
    yyin = fopen( "valve.def", "r" );
	int r = yylex();
	while (r != 0) {
		//printf("%d\n", r);
		r = yylex();
	}
	//return yylex();
	}

/* End of user defined code section. */
