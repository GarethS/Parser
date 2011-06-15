
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"	// To see the %union in valve.y
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
neql			"!="
gtr				[>]
lss				[<]
or				"||"
and				"&&"
testForEqual	"=="
%x		comment

%%
{line}		{yylval.string = strdup(yytext); return LINE;}
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
{neql}		{return NEQL;}
{gtr}		{return GTR;}
{lss}		{return LSS;}
{or}		{return OR;}
{and}  		{return AND;}
{testForEqual}	{return TEST_FOR_EQUAL;}
{digit}+		{yylval.number = atoi(yytext); return CONST;}	/* constants */
{alpha}{alphanum}*	{yylval.string = strdup(yytext); return VAR;}	/* variable */
{alpha}{alphanum}*{period}{alpha}{alphanum}*	{yylval.string = strdup(yytext); return METHOD;}	/* method */

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

/*
extern int yylex();

int main( argc, argv )
int argc;
char *argv[];
	{
    yyin = fopen( "\\dev\\parser\\port1\\port.def", "r" );
	return yylex();
	}
*/
/* End of user defined code section. */
