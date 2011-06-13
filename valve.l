
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

line			[A-Ha-h][0-7]
lparen			[(]
rparen			[)]
lbrace			[{]
rbrace			[}]
comma			[,]
semi			[;]
bang			[!]
or				"||"
and				"&&"
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
{or}		{return OR;}
{and}  		{return AND;}

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
