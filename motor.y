/* Copyright (c) 2013 Gareth Scott */

/*
  The idea is to parse the compiled language (now in a very simple format) which
   gets sent one line (statement or symbol) at a time. The line is parsed and an
   acknowledgement is returned instructing the sender to send the next line until
   each line has been parsed. yy_scan_bytes() will be used by flex to extract each
   line which is then sent here to the parser. 
  This parser is used by the motor (i.e. embedded) portion of the controller.
*/

%{
#define YYDEBUG 0
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
//#include <string.h>
//#include <assert.h>
#include "compilerHelper.h"

nodeEmbeddedtype net = nodeEmbeddedUnknown; // Usually either a statment or symbol

// Statement entries
int statementNestingLevel;
int statementPosition;
int statementType;
int statementValue;

// Symbol entries
int symbolType;
int symbolValue;
int symbolFcnLink;

%}

/* See compiler.h for structure definitions */
%union {
	int integer;
}

/* TERMINALS */
%token <integer> VERSION_BEGIN VERSION_END STATEMENT_BEGIN STATEMENT_END SYMBOL_BEGIN SYMBOL_END ROOT LEFT RIGHT VARIABLE OPERATOR IF EVAL0 EVALWHILE0 ELSE ENDIF WHILE ENDWHILE FUNCTIONCALL FUNCTIONCALLEND JMPENDIF START PROGRAMEND CONST

/* non-terminals */
%type <integer> statement version symbol position action

%defines	/* generate motor.tab.h for use with motor.yy.c */
%require "2.4.2"
%no-lines 


%start program

%% /* Grammar rules and actions */
program:    statement           {/*printf("STMT\n");*/ net = nodeEmbeddedStatement;}
            | symbol            {/*printf("SYMB\n");*/ net = nodeEmbeddedSymbol;}
            | version           {net = nodeEmbeddedVersion;}
            | PROGRAMEND        {net = nodeEmbeddedProgramEnd;}

position:           CONST       {$$ = $1;}
                    | LEFT      {$$ = $1;}
                    | RIGHT     {$$ = $1;}
                    | ROOT      {$$ = $1;}

action:             VARIABLE                {$$ = $1;}
                    | OPERATOR
                    | IF
                    | EVAL0
                    | EVALWHILE0
                    | ELSE
                    | ENDIF
                    | WHILE
                    | ENDWHILE
                    | FUNCTIONCALL
                    | FUNCTIONCALLEND
                    | JMPENDIF
                    | START
                    | PROGRAMEND
                    
version:            VERSION_BEGIN CONST VERSION_END
      
statement:          STATEMENT_BEGIN CONST/*nestingLevel*/ position action CONST/*value*/ STATEMENT_END   {statementNestingLevel = $2; statementPosition = $3; statementType = $4; statementValue = $5;}

symbol:             SYMBOL_BEGIN CONST/*type*/ CONST/*value*/ CONST/*fcnLink*/ SYMBOL_END    {symbolType = $2; symbolValue = $3; symbolFcnLink = $4;}                    
                    
%% /* Additional C code */


#include "motor.yy.c"

#if !BISON_IAR
// Don't include this part if we're embedding in hardware
int main ()
{
	// To turn on debugging, make sure the next line is uncommented and
	//  turn on the -t (also use -v -l) options in bison.exe.
	//yydebug = 1; 
#if 1
    // This code will be done in the embedded system. It's here only for testing.
#define SERIAL_INPUT_LEN    (64)    
    char serialInput[SERIAL_INPUT_LEN] = " ab <STMT> 0 ROOT FunctionCall 14 <stmt> ";
//    char serialInput[SERIAL_INPUT_LEN] = "<STMT> 0 ROOT FunctionCall 14 <stmt>  <STMT> 0 ROOT If 3 <stmt>";
    YY_BUFFER_STATE yySerialBuffer = yy_scan_bytes(serialInput, SERIAL_INPUT_LEN);
    yy_switch_to_buffer(yySerialBuffer);
#else    
    yyin = fopen("treeMotor.txt", "r" );
#endif    
	int yyparseReturn = yyparse();
    printf("parse=%d\n", yyparseReturn);
    //yyin = fopen("symbolTable.txt", "r" );
	//yyparse ();
    return 0;
}

void yyerror (char* s)
{
	printf ("Line: %d: %s before '%s'\n", --yylineno, s, yytext);
}
#else // BISON_IAR
void yyerror (char* s)
{
    // Set flag for interpreter
}
#endif // not BISON_IAR
