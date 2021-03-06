
/* Copyright (c) 2011, 2012, 2013 Gareth Scott */

/*
       Sample of motor control language:
	   counter == 4 && (m1.position >= 3 || m1.velocity < 0) {
			counter = counter + 1;
			m1.position = 2;
			
	   }

*/

%{
/* #define YYDEBUG 1 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
//#include <assert.h>
#include "compiler.h"

// Variable, constant and function name symbol table
symbolNode symbolTable[VAR_ITEMS];
// When the function name gets parsed (the last thing to be done after parsing
//  the statements), this is the index used to hold the function name. When
//  this happens, a space will be reserved at the current 'symbolTableFreeIndex'
//  to hold the next function name. 
//  In addition, all the variables in the scope of the current function will be
//  inspected, and if they are the same name as a function parameter, they will
//  be marked as stack based variables. Their entry in the symbol table will,
//  likewise, be noted.
unsigned int symbolTableLastFunctionIndex = 0;
unsigned int symbolTableFreeIndex = 1;
unsigned int functionParameterIndex = 0; // Index of argument on execution stack so interpreter knows how to access this variable. 

// Abstract syntax tree table
astNode syntaxTable[SYNTAX_ITEMS];
unsigned int syntaxTableFreeIndex = 0;

astNode statementTable[STATEMENT_ITEMS];
unsigned int statementTableFreeIndex = 0;

unsigned int statementOutputIndex = 0;

#define QUOTES_MAIN                     "main"
#define QUOTES_MOVEABSOLUTE             "moveAbsolute"
#define QUOTES_MOVERELATIVE             "moveRelative"
#define QUOTES_SLEEP                    "sleep"
#define QUOTES_SLEEP_UNTIL              "sleepUntil"
#define QUOTES_LED                      "LED"
#define QUOTES_RPM                      "RPM"
#define QUOTES_RPM_x10k                 "RPMx10k"
#define QUOTES_ACCEL_MICROSEC           "accelMicroSec"
#define QUOTES_DEGREE_x10k_ABSOLUTE     "degreex10kAbsolute"
#define QUOTES_DEGREE_x10k_RELATIVE     "degreex10kRelative"
#define QUOTES_WAIT_FOR_IDLE            "waitForIdle"
#define QUOTES_PRINT_NUMBER             "printNumber"
#define QUOTES_GET_INPUT                "getInput"
#define QUOTES_SET_OUTPUT               "setOutput"
#define QUOTES_GET_ADC                  "getADC"
#define QUOTES_GET_TEMP                 "getTemp"
#define QUOTES_GET_POSITION             "getPosition"
#define QUOTES_RESET                    "reset"


%}

/* See compiler.h for structure definitions */
%union {
	int integer;
    float floatingPoint;
	char* string;
	astNode* pSyntaxNode;
}

/* TERMINALS */
%token INPUTS OUTPUTS COMMA
%token EQUAL LBRACE RBRACE ARRAYDEFINE IF ELSE WHILE
// 4x. Add intrinsic function name from flex here
%token <string>	VAR VAR_METHOD CONST CONST_FLOAT MAIN MOVEABSOLUTE MOVERELATIVE SLEEP SLEEPUNTIL LED RPM RPMx10k ACCELMICROSEC DEGREEx10kABSOLUTE DEGREEx10kRELATIVE WAITFORIDLE PRINTNUMBER GETINPUT SETOUTPUT GETADC GETTEMP GETPOSITION RESET

// %left or %right takes the place of %token
%left AND OR BITWISEAND BITWISEOR
%left TEST_FOR_EQUAL GEQ LEQ NEQ GTR LSS SHL SHR
%left LPAREN 
%left PLUS MINUS
%left XOR
%left MULT DIV	
%left LBRACKET
%right RPAREN RBRACKET
%right SEMI
%nonassoc TILDE BANG     // Using these in an associative manner is a syntax error
/* last entry gets highest precedence. Section 3.7.3 bison.pdf */
/* Also note that the above entries affect their order in valve.tab.h with
    the highest precedence operators getting numerically higher numbers. */

/* non-terminals */
%type <integer> arrayDefine
//%type <string>
%type <pSyntaxNode>  statement statementAssign statementIf statementWhile expr statementList functionDefnList argList commaArgList defnArgList defnCommaArgList

%defines	/* generate valve.tab.h for use with lex.yy.c */
%require "2.4.2"


%start program

%% /* Grammar rules and actions */
program:    functionDefnMain functionDefnList   {postProcess(); dumpSymbolTable("symbolTable.txt");}  // Keep it simple. Require main() to be first function at top of file.

functionDefnList:   /* empty */     {}
        | functionDefnList functionDefn {}

functionDefnMain: 	MAIN LPAREN defnArgList RPAREN LBRACE statementList RBRACE	{addFunction(QUOTES_MAIN, $3, $6);}

functionDefn: 	VAR LPAREN defnArgList RPAREN LBRACE statementList RBRACE	{addFunction($1, $3, $6);}
                // 3x. Finally, make sure there is no function definition for this intrinsic. After all, it is built-in
                | MOVEABSOLUTE LPAREN defnArgList RPAREN LBRACE statementList RBRACE {yyerror(QUOTES_MOVEABSOLUTE);}

statementList: /* empty */	{$$ = NULL;}    // Make sure 'statementList' starts out as NULL for each new 'statementList'
		| statementList statement	{/*printf("\nstatementList:%d, statement:%d", (int)$1, (int)$2);*/ $$ = addStatement($1, $2); /*printf(" newStatementList:%d", (int)$$);*/}

statement:	        statementAssign	                    {/*printf("\nstatementAssign:%d", (int)$1); walkSyntaxTree($1, "start", 0);*/ $$ = $1;}
                    | statementIf                       {/*printf("\nstatementIf:%d", (int)$1);*/ $$ = $1;}
                    | statementWhile                    {$$ = $1;}
                    | VAR LPAREN argList RPAREN SEMI    {$$ = addNodeFunctionCall($1, $3);}   // user defined function call
                    
                    // 1x. To add an intrinsic (built-in) function, add its exact prototype here. Note, 'x.' is an aid to searching for things
                    //      that need done to add intrinsic functions.
                    | MOVEABSOLUTE LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_MOVEABSOLUTE, $4, $6);}
                    // 2x. Make sure any other combination of parameters is caught here.
                    | MOVEABSOLUTE LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_MOVEABSOLUTE);} // catch incorrect parameters
                    
                    | MOVERELATIVE LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_MOVERELATIVE, $4, $6);}
                    | MOVERELATIVE LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_MOVERELATIVE);}
                    
                    | SLEEP LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_SLEEP, $4, $6);}
                    | SLEEP LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_SLEEP);}
                    
                    | SLEEPUNTIL LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_SLEEP_UNTIL, $4, $6);}
                    | SLEEPUNTIL LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_SLEEP_UNTIL);}
                    
                    | LED LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_LED, $4, $6);}
                    | LED LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_LED);}
                    
                    | RPM LPAREN BITWISEAND VAR COMMA expr COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction2(QUOTES_RPM, $4, $6, $8);}
                    | RPM LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_RPM);}
                    
                    | RPMx10k LPAREN BITWISEAND VAR COMMA expr COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction2(QUOTES_RPM_x10k, $4, $6, $8);}
                    | RPMx10k LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_RPM_x10k);}
                    
                    | ACCELMICROSEC LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_ACCEL_MICROSEC, $4, $6);}
                    | ACCELMICROSEC LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_ACCEL_MICROSEC);}
                    
                    | DEGREEx10kABSOLUTE LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_DEGREE_x10k_ABSOLUTE, $4, $6);}
                    | DEGREEx10kABSOLUTE LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_DEGREE_x10k_ABSOLUTE);}
                    
                    | DEGREEx10kRELATIVE LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_DEGREE_x10k_RELATIVE, $4, $6);}
                    | DEGREEx10kRELATIVE LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_DEGREE_x10k_RELATIVE);}
                    
                    | WAITFORIDLE LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_WAIT_FOR_IDLE, $4, $6);}
                    | WAITFORIDLE LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_WAIT_FOR_IDLE);}
                    
                    | PRINTNUMBER LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_PRINT_NUMBER, $4, $6);}
                    | PRINTNUMBER LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_PRINT_NUMBER);}
                    
                    | GETINPUT LPAREN BITWISEAND VAR RPAREN SEMI {$$ = addNodeInstrinsicFunction0(QUOTES_GET_INPUT, $4);}
                    | GETINPUT LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_GET_INPUT);}
                    
                    | SETOUTPUT LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_SET_OUTPUT, $4, $6);}
                    | SETOUTPUT LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_SET_OUTPUT);}
                    
                    | GETADC LPAREN BITWISEAND VAR COMMA expr RPAREN SEMI {$$ = addNodeInstrinsicFunction1(QUOTES_GET_ADC, $4, $6);}
                    | GETADC LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_GET_ADC);}
                    
                    | GETTEMP LPAREN BITWISEAND VAR RPAREN SEMI {$$ = addNodeInstrinsicFunction0(QUOTES_GET_TEMP, $4);}
                    | GETTEMP LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_GET_TEMP);}
                    
                    | GETPOSITION LPAREN BITWISEAND VAR RPAREN SEMI {$$ = addNodeInstrinsicFunction0(QUOTES_GET_POSITION, $4);}
                    | GETPOSITION LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_GET_POSITION);}
                    
                    | RESET LPAREN BITWISEAND VAR RPAREN SEMI {$$ = addNodeInstrinsicFunction0(QUOTES_RESET, $4);}
                    | RESET LPAREN argList RPAREN SEMI {$$ = NULL; yyerror(QUOTES_RESET);}
                    
                    | arrayDefine                       {$$ = NULL;}

statementWhile: WHILE LPAREN expr RPAREN LBRACE statementList RBRACE {$$ = addNodeIfOrWhile($3, $6, NULL, nodeWhile);}

statementIf:  IF LPAREN expr RPAREN LBRACE statementList RBRACE	{/*printf("statementIf:statementList:%d", (int)$6);*/ $$ = addNodeIfOrWhile($3, $6, NULL, nodeIf);}
//statementIf:  IF LPAREN expr RPAREN LBRACE statementList RBRACE	{fp = fopen("patternTree.txt", "wb"); walkSyntaxTree($3, "ROOT", 0); fclose(fp);
//                                             printf("\n\n"); fp = fopen("actionTree.txt", "wb");
//                                             fwrite("0 0 Action 0\n", 1, 13/* strlen("0 0 Action 0\n") */, fp); walkList($6); fclose(fp);}
              | IF LPAREN expr RPAREN LBRACE statementList RBRACE ELSE LBRACE statementList RBRACE	{/*printf("statementIf:statementList:%d", (int)$6);*/ $$ = addNodeIfOrWhile($3, $6, $10, nodeIf);}

statementAssign:    VAR EQUAL expr SEMI	                            {/*printf("\nstatementAssign");*/ $$ = addNodeVariableOperator(EQUAL, addVarToSymbolTable($1), $3);}
//                    | array EQUAL expr SEMI                       {$$ = addNodeBinaryOperator(EQUAL, $1, $3);}
                      | VAR LBRACKET expr RBRACKET EQUAL expr SEMI  {$$ = addNodeBinaryOperator(EQUAL, addNodeArray($1, $3), $6);} // array    

                                        /* First statement makes sure '-1' is in symbol table. Second statement multiplies expression by that symbol (i.e. '-1') */
expr:	MINUS expr	                    {astNode* pMinusOneNode = addNodeSymbolIndex(addVarToSymbolTable("-1")); $$ = addNodeBinaryOperator(MULT, $2, pMinusOneNode);}
        | BANG expr                     {$$ = addNodeBinaryOperator(BANG, $2, NULL);}
        | TILDE expr                    {$$ = addNodeBinaryOperator(TILDE, $2, NULL);}
        | LPAREN expr RPAREN	        {$$ = $2;}
		| expr PLUS  expr	            {$$ = addNodeBinaryOperator(PLUS, $1, $3);}
		| expr MINUS expr	            {$$ = addNodeBinaryOperator(MINUS, $1, $3);}
		| expr MULT  expr	            {$$ = addNodeBinaryOperator(MULT, $1, $3);}
		| expr DIV   expr	            {$$ = addNodeBinaryOperator(DIV, $1, $3);}
		| expr XOR   expr               {$$ = addNodeBinaryOperator(XOR, $1, $3);}
		| expr AND   expr	            {$$ = addNodeBinaryOperator(AND, $1, $3);}
		| expr OR    expr	            {$$ = addNodeBinaryOperator(OR, $1, $3);}
		| expr BITWISEAND  expr         {$$ = addNodeBinaryOperator(BITWISEAND, $1, $3);}
		| expr BITWISEOR   expr         {$$ = addNodeBinaryOperator(BITWISEOR, $1, $3);}
		| expr TEST_FOR_EQUAL expr      {$$ = addNodeBinaryOperator(TEST_FOR_EQUAL, $1, $3);}
		| expr NEQ   expr	            {$$ = addNodeBinaryOperator(NEQ, $1, $3);}
		| expr GEQ   expr	            {$$ = addNodeBinaryOperator(GEQ, $1, $3);}
		| expr LEQ   expr	            {$$ = addNodeBinaryOperator(LEQ, $1, $3);}
		| expr GTR   expr	            {$$ = addNodeBinaryOperator(GTR, $1, $3);}
		| expr LSS   expr	            {$$ = addNodeBinaryOperator(LSS, $1, $3);}
		| VAR				            {$$ = addNodeSymbolIndex(addVarToSymbolTable($1));}
		| CONST				            {$$ = addNodeSymbolIndex(addVarToSymbolTable($1));}
        | VAR LPAREN argList RPAREN     {$$ = addNodeFunctionCall($1, $3);}  // function call
        | VAR LBRACKET expr RBRACKET    {$$ = addNodeArray($1, $3);}    // array						

argList: /* empty */            {$$ = NULL;}
        | commaArgList          {$$ = $1;}

commaArgList: expr              {$$ = addFcnCallArgument(NULL, $1);}
        | expr COMMA commaArgList {$$ = addFcnCallArgument($3, $1);}    // right-recursive
            
defnArgList: /* empty */        {$$ = NULL;}
        | defnCommaArgList      {$$ = $1;}

defnCommaArgList: VAR  {$$ = addFcnDefnArgument(NULL, $1, VAR_PASS_BY_VALUE, VAR_FIRST_PARAMETER);}
        | BITWISEAND VAR {$$ = addFcnDefnArgument(NULL, $2, VAR_PASS_BY_REFERENCE, VAR_FIRST_PARAMETER);}
        | defnCommaArgList COMMA VAR {$$ = addFcnDefnArgument($1, $3, VAR_PASS_BY_VALUE, VAR_SUBSEQUENT_PARAMETER);}
        | defnCommaArgList COMMA BITWISEAND VAR  {$$ = addFcnDefnArgument($1, $4, VAR_PASS_BY_REFERENCE, VAR_SUBSEQUENT_PARAMETER);}

arrayDefine:    ARRAYDEFINE VAR LBRACKET CONST RBRACKET SEMI    {$$ = addArrayToSymbolTable($2, atoi($4));}

%% /* Additional C code */


#include "lex.yy.c"

void yyerror (char* s)
{
	printf ("Line: %d: %s before '%s'\n", --yylineno, s, yytext);
}

int main ()
{
#if 0
	/* Initialize IO boilerplate code. */
	FILE* stream = fopen("\\dev\\parser\\port1\\boilerplate.c", "r" );
	if (stream == NULL) {
		printf("Error opening boilerplate code.\n");
	} else {
		#define BUF_LEN (256)
		char buffer[BUF_LEN];
		printf("/* This code automatically generated by ZPORT 1.0. */\n", buffer);
		while (fgets(buffer, BUF_LEN, stream) != NULL) {
			printf("%s", buffer);
		}
		fclose(stream);
	}
#endif

	//initVarTable(); // Doesn't seem necessary since we use buildSymbol()
	// To turn on debugging, make sure the next line is uncommented and
	//  turn on the -t (also use -v -l) options in bison.exe.
	yydebug = 1; 
    yyin = fopen("valve5.def", "r" );
	yyparse ();
    return 0;
}

void postProcess(void) {
    checkFcnSanity();
    fixupFcnCalls();
}

// Replace function call fcnDefnLink with the index of the function definition
void fixupFcnCalls(void) {
    int i;
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        //printf("\nindex:%d, type:%d", i, symbolTable[i].type);
        if (symbolTable[i].type == nodeFunctionCall) {
            // Find the definition for this call and grab its statement link index.
            int fcnDefnIndex;
            int count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count != 1) {
                // We should have only one definition and this should have been detected in checkFcnSanity()
                debugAssert(ERR:fixupFcnCalls():incorrect number of function definitions);
                return;
            }
            //printf("\nfixupFcnCalls():%d, %d", i, symbolTableFreeIndex);
            symbolTable[i].fcnDefnLink = fcnDefnIndex;
            //break;
        }
    }
}

void checkFcnSanity(void) {
    //printf("checkFcnSanity");
    // 0. Check for a function called main()
	symbolNode tmp;
    int fcnDefnIndex;
    buildSymbol(QUOTES_MAIN, 0, &tmp);
    int count = findSymbolFcnDefinition(&tmp, &fcnDefnIndex);
    if (count == 0) {
        debugAssert(ERR:checkFcnSanity():no 'main()' found);
    }
    
    // 1. Check for duplicate definitions including duplicates of 'main()'
    int i;
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        if (symbolTable[i].type == nodeFunctionDefinition) {
            count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count > 1) {
                debugAssert(ERR:checkFcnSanity():multiple definitions found:);
                printf("%s", symbolTable[i].name);
                break;
            }
        }
    }
    
    // 2. Check number of parameters match function call and definition
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        if (symbolTable[i].type == nodeFunctionCall) {
            // Make sure there's a matching definition.
            count = findSymbolFcnDefinition(symbolTable + i, &fcnDefnIndex);
            if (count == 0) {
                debugAssert(ERR:checkFcnSanity():no definition found:check parameter count:);
                printf("%s", symbolTable[i].name);
                break;
            }
        }
    }
}

#if 0
void initVarTable(void) {
	int i;
	for (i = VAR_ITEMS - 1; i >= 0; i--) {
		symbolTable[i].name[0] = EOS;
		symbolTable[i].val = DEFAULT_VAR_VALUE;	// Initialize variable to 0
        symbolTable[i].fcnDefnLink = VAR_FCN_LINK_UNDEFINED;
	}
}
#endif

// Return index of variable/constant in symbol table
int insertSymbol(symbolNode* pVar) {
	if (symbolTableFreeIndex < VAR_ITEMS) {
		symbolTable[symbolTableFreeIndex] = *pVar;
		return symbolTableFreeIndex++;
	}
	return VAR_TABLE_LIMIT;
}

// A little danger here. Forces symbol to be placed at a specific index.
// Used to insert function name at start of symbol list for that function.
void insertSymbolAtIndex(symbolNode* pVar, const unsigned int index) {
    if (index >= VAR_ITEMS) {
        debugAssert(ERR:insertSymbolAtIndex():index >= VAR_ITEMS);
        return;
    }
    symbolTable[index] = *pVar;
}

// Return index of variable (includes function names), or VAR_NOT_FOUND if not found.
int findSymbolByName(const char* pVarName) {
    symbolNode thisNode;
    buildSymbol(pVarName, 0, &thisNode);
    return findSymbol(&thisNode);
}

// Return index where variable is located in symbolTable, or VAR_NOT_FOUND if not found.
// TODO: nodeConst can be anywhere in symbol table since its value never varies. May want to reuse these constants.
int findSymbol(symbolNode* pVar) {
	int i;
    // Only search within the scope of the function currently being parsed.
	for (i = symbolTableLastFunctionIndex; i < symbolTableFreeIndex; ++i) {
		if (strncmp(symbolTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
			return i;
		}
	}
	return VAR_NOT_FOUND;
}

// Returns the number of function definitions that match the symbolNode. Also return
//  the index where it can be found. That symbol contains the index into the statement list
//  for the actual location of the definition.
int findSymbolFcnDefinition(symbolNode* pVar, int* pFcnDefnIndex) {
    if (pFcnDefnIndex == NULL) {
        debugAssert(ERR:findSymbolFcnDefinition():pFcnDefnIndex == NULL);
        return 0;
    }
    unsigned int count = 0;
    int i;
    //printf("symbolTableFreeIndex=%d", symbolTableFreeIndex);
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
        //printf("\ntype=%d, val=%d, name=%s", symbolTable[i].type, symbolTable[i].val, symbolTable[i].name);
        //printf("\ntype=%d, val=%d, name=%s", pVar->type, pVar->val, pVar->name);
		if (symbolTable[i].type == nodeFunctionDefinition &&
            symbolTable[i].val == pVar->val /* contains number of arguments to this function */ &&
            strncmp(symbolTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
            *pFcnDefnIndex = symbolTable[i].fcnDefnLink;
            ++count;
        }
	}
    // 5x. Add test for intrinsic functions here. TODO: Find a better way to do this!
    if (strncmp(QUOTES_MOVEABSOLUTE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_MOVE_ABSOLUTE;
        //printf(QUOTES_MOVEABSOLUTE);
        ++count;
    } else if (strncmp(QUOTES_MOVERELATIVE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_MOVE_RELATIVE;
        ++count;
    } else if (strncmp(QUOTES_SLEEP, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_SLEEP;
        ++count;
    } else if (strncmp(QUOTES_SLEEP_UNTIL, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_SLEEP_UNTIL;
        ++count;
    } else if (strncmp(QUOTES_LED, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_LED;
        ++count;
    } else if (strncmp(QUOTES_RPM, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_THREE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_RPM;
        ++count;
    } else if (strncmp(QUOTES_RPM_x10k, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_THREE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_RPM_x10k;
        ++count;
    } else if (strncmp(QUOTES_ACCEL_MICROSEC, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_ACCEL_MICROSEC;
        ++count;
    } else if (strncmp(QUOTES_DEGREE_x10k_ABSOLUTE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_DEGREE_x10k_ABSOLUTE;
        ++count;
    } else if (strncmp(QUOTES_DEGREE_x10k_RELATIVE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_DEGREE_x10k_RELATIVE;
        ++count;
    } else if (strncmp(QUOTES_WAIT_FOR_IDLE, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_WAIT_FOR_IDLE;
        ++count;
    } else if (strncmp(QUOTES_PRINT_NUMBER, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_PRINT_NUMBER;
        ++count;
    } else if (strncmp(QUOTES_GET_INPUT, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_ONE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_GET_INPUT;
        ++count;
    } else if (strncmp(QUOTES_SET_OUTPUT, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_SET_OUTPUT;
        ++count;
    } else if (strncmp(QUOTES_GET_ADC, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_TWO) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_GET_ADC;
        ++count;
    } else if (strncmp(QUOTES_GET_TEMP, pVar->name, VAR_NAME_LENGTH-1) == 0 &&
        pVar->val == NUM_PARAMETERS_ONE) {
        *pFcnDefnIndex = INTRINSIC_FCN_DEFN_GET_TEMP;
        ++count;
    }
    return count;
}

// e.g. if (x==2) {x = 1;} else {x = 4;}
// Note that both pIfStatementList and pElseStatementList can both be NULL. This can happen if they have empty statement lists.
astNode* addNodeIfOrWhile(astNode* pExpr, astNode* pIfOrWhileStatementList, astNode* pElseStatementList, nodeType type) {
    static int uniqueID = 0;

	astNode* p = getNextASTNode();
    if (pExpr == NULL) {
        debugAssert(ERR:addNodeIfOrWhile():pExpr == NULL);
		return NULL;
    }
    p->type     = type;
    p->value    = uniqueID++;
    p->pLeft    = pExpr;
    p->pCentre  = pIfOrWhileStatementList;
    p->pRight   = pElseStatementList;
    return p;
}

// e.g. '4 * c1'
astNode* addNodeBinaryOperator(int operator, astNode* pLeft, astNode* pRight) {
	astNode* p  = getNextASTNode();
	p->type     = nodeOperator;
	p->value    = operator;
	p->pLeft    = pLeft;
	p->pRight   = pRight;
	return p;	
}

// e.g. '!variableName'
astNode* addNodeUnaryOperator(int operator, astNode* pRight) {
    return addNodeBinaryOperator(operator, NULL, pRight);
}

// e.g. 'c3 = 4 * c1;' where 'operator' is '='
astNode* addNodeVariableOperator(int operator, int varIndex, astNode* pRight) {
	astNode* pLeft  = getNextASTNode();
	pLeft->type     = nodeVariable;
	pLeft->value    = varIndex;
    return addNodeBinaryOperator(operator, pLeft, pRight);
}

astNode* addNodeInstrinsicFunction0(char* functionName, char* returnValue) {
    astNode* p1 = addNodeSymbolIndex(addVarToSymbolTable(returnValue));
    //astNode* p2 = addFcnCallArgument(NULL, parameter1);
    astNode* p3 = addFcnCallArgument(NULL, p1);
    return addNodeFunctionCall(functionName, p3);
}

// The 1 stands for 1 parameter with this call
astNode* addNodeInstrinsicFunction1(char* functionName, char* returnValue, astNode* parameter1) {
    astNode* p1 = addNodeSymbolIndex(addVarToSymbolTable(returnValue));
    astNode* p2 = addFcnCallArgument(NULL, parameter1);
    astNode* p3 = addFcnCallArgument(p2, p1);
    return addNodeFunctionCall(functionName, p3);
}

// The 2 stands for 2 parameters with this call. Note how return parameter, p1, is added last and it's the first parameter in the function call.
//  Parameters are added in the reverse order they appear in the function call.
astNode* addNodeInstrinsicFunction2(char* functionName, char* returnValue, astNode* parameter1, astNode* parameter2) {
    astNode* p1 = addNodeSymbolIndex(addVarToSymbolTable(returnValue));
    astNode* p2 = addFcnCallArgument(NULL, parameter2);
    astNode* p3 = addFcnCallArgument(p2, parameter1);
    astNode* p4 = addFcnCallArgument(p3, p1);
    return addNodeFunctionCall(functionName, p4);
}

astNode* addNodeFunctionCall(char* pFuncName, astNode* pArgList) {
    //printf("\naddNodeFunctionCall()");
    return addFunction(pFuncName, pArgList, NULL);
}

// Add either a function call (pStatmentList = NULL) or a function definition (pStatmentList != NULL)
astNode* addFunction(const char* pFuncName, astNode* pArgList, astNode* pStatementList) {
    // Essentially the same as addNodeFunctionCall() except this is a definition, not a call.
    // 0. Count number of arguments and assign that value to its syntax node
    //int argCount = countArguments(pArgList);
    //printf("\naddFunction():argCount=%d", argCount);
    // 1. See if pFuncName is already in the symbol list. It could be there from
    //     another function call or the function definition.
    // Make symbol table entry and add to symbol table
#if 0    
    int symbolIndex = findSymbolByName(pFuncName);
    if (symbolIndex == VAR_NOT_FOUND) {
        symbolNode funcNode;
        buildSymbol(pFuncName, argCount, &funcNode);
        funcNode.type = nodeFunctionCall;
        int symbolIndex = insertSymbol(&funcNode);
        if (symbolIndex == VAR_TABLE_LIMIT) {
            return NULL;
        }
    } else {
        // Make sure value = argCount and type = nodeFunctionCall
        if (symbolTable[symbolIndex].val != argCount) {
            debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].val != argCount);
            return NULL;
        }
        if (symbolTable[symbolIndex].type != nodeFunctionCall) {
            debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].type != nodeFunctionCall);
            return NULL;
        }
    }
#endif    
    
    // 3. Walk statement list to see if there's already a call to this function.
    //     If so, check that the argument count matches
    // No need to do this. We checked the symbol table already and every function call or definition
    //  will do the same.
    // walkList(statementTable);   // Still needs modified to search for functions by this name
    
    if (pStatementList == NULL) {
        // This is a function call
        // Add function name to this function's symbol table
        int argCount = countArguments(pArgList);
        int symbolIndex = findSymbolByName(pFuncName);
        if (symbolIndex == VAR_NOT_FOUND) {
            symbolNode funcNode;
            buildSymbol(pFuncName, argCount, &funcNode);
            funcNode.type = nodeFunctionCall;
            symbolIndex = insertSymbol(&funcNode);
            if (symbolIndex == VAR_TABLE_LIMIT) {
                return NULL;
            }
        } else {
            if (symbolTable[symbolIndex].type != nodeFunctionCall) {
                debugAssert(ERR:addNodeFunctionCall():symbolTable[symbolIndex].type != nodeFunctionCall);
                return NULL;
            }
        }
        
        astNode* p = getNextASTNode();
        p->type = nodeFunctionCall;
        p->value = symbolIndex;
        p->pNext = pArgList;
        return p;
    } else {
        // This is a function definition not a function call
        if (pFuncName == NULL) {
            debugAssert(ERR:addNodeFunction():pFuncName == NULL);
        } else {
            printf("\nFunctionDefn: %s", pFuncName);
        }
        // TODO: Go through pStatementList, fixing up references to arguments so they get
        //  picked off the evaluation stack, rather than the symbol table.
        // Add function name to reserved space in symbol table.
        symbolNode functionNode;
        buildSymbol(pFuncName, functionParameterIndex /* equals num arguments */, &functionNode);
        // This is set to 1 because of the way function parameters are parsed. The first parameter doesn't actually get parsed until last
        //  but should have index 0. We correct for this by noting in the addFcnDefnArgument() call if it's the first parameter.
        functionParameterIndex = 0; // Reset for every new function definition. Don't need it any more for this function.
        functionNode.type = nodeFunctionDefinition;
        // statementOutputIndex contains the index into execution list. Put that value into fcnDefnLink
        //  of the function definition symbol.
        functionNode.fcnDefnLink = statementOutputIndex;
        insertSymbolAtIndex(&functionNode, symbolTableLastFunctionIndex);
        symbolTableLastFunctionIndex = symbolTableFreeIndex++;
        
        FILE* fp = NULL;
        int inMain = FALSE;
        if (strcmp(pFuncName, QUOTES_MAIN) == 0) {
            // Clears the file so it doesn't keep getting bigger each time this program is run.
            fp = fopen("tree.txt", "wb");
            statementOutputIndex = 0;
            inMain = TRUE;
        } else {
            fp = fopen("tree.txt", "ab");
        }
        writeStatement("0 0 Start 0", fp);
        /*printf("\nstatementList=%d", (int)$1);*/
        //walkList(pArgList, fp);   // Doesn't seem to be necessary since function parameters have alreay been added to the symbol table
        walkList(pStatementList, fp);
        if (inMain) {
            writeStatement("0 0 ProgramEnd 0", fp);
            printf("\nProgramEnd");
        }
        fclose(fp);
    }
    return NULL;
}

astNode* addNodeArray(char* pVarName, astNode* pASTNode) {
    // 1. Make new astNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pASTNode->value, pASTNode->pLeft->value, pASTNode->pRight->value);
    //printf("xxx root=%d\n", pASTNode->value);
	astNode* pArrayVar = getNextASTNode();
	pArrayVar->type = nodeArray;
    symbolNode pArrayNode;
    buildSymbol(pVarName, 0, &pArrayNode);
	pArrayVar->value = findSymbol(&pArrayNode);
    if (pArrayVar->value == VAR_NOT_FOUND) {
        debugAssert(ERR: addNodeArray():VAR_NOT_FOUND);
    }
#if 1
    // This fails
    return addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
#else    
    // This passes, but looks like it's using the wrong index in the array
    //return addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
    astNode* pan = addNodeBinaryOperator(LBRACKET, pArrayVar, pASTNode);
    //printf("yyyroot=%d, left=%d, right=%d", pan->value, pan->pLeft->value, pan->pRight->value);
    return pan;
#endif    
}

astNode* addNodeSymbolIndex(int varIndex) {
	astNode* p  = getNextASTNode();
	p->type     = nodeVariable;
	p->value    = varIndex;
	return p;	
}

void initNode(astNode* pSyntaxNode) {
    if (pSyntaxNode == NULL) {
        debugAssert(ERR:initNode():pSyntaxNode == NULL);
        return;
    }
    pSyntaxNode->type           = nodeInvalid;
    pSyntaxNode->value          = -1;
    pSyntaxNode->pLeft          = NULL;
    pSyntaxNode->pRight         = NULL;
    pSyntaxNode->pCentre        = NULL;
    pSyntaxNode->pNext          = NULL;
}

astNode* getNextASTNode(void) {
	if (syntaxTableFreeIndex < SYNTAX_ITEMS) {
        // Initialize all members before making this node available
        initNode(syntaxTable + syntaxTableFreeIndex);
		return syntaxTable + syntaxTableFreeIndex++;
	}
    // Cannot survive this condition.
    debugAssert(FATAL:ERR:getNextASTNode():syntaxTableFreeIndex >= SYNTAX_ITEMS);
	return NULL;
}

astNode* getNextStatementNode(void) {
	if (statementTableFreeIndex < STATEMENT_ITEMS) {
#if REGRESS_1    
        printf("getNextStatementNode() %d\n", statementTableFreeIndex);
        fflush(stdout);
#endif /* REGRESS_1 */
        initNode(statementTable + statementTableFreeIndex);
		return statementTable + statementTableFreeIndex++;
	}
    debugAssert(ERR:FATAL:getNextStatementNode():);
	return NULL;
}

// A variable may not start with a number. One that does is considered a constant.
int isConstant(symbolNode* pVar) {
	if (isdigit((int)pVar->name[0]) || (pVar->name[0] == '-' && isdigit((int)pVar->name[1]))) {
		return TRUE;
	}
	return FALSE;
}

void buildSymbol(const char* name, int value, symbolNode* varNode) {
    if (name == NULL || varNode == NULL) {
        debugAssert(ERR:buildSymbol());
        return;
    }
	strncpy(varNode->name, name, VAR_NAME_LENGTH-1);
	varNode->name[VAR_NAME_LENGTH-1] = EOS;
	varNode->val = value;
    varNode->type = nodeVariable;
    varNode->fcnDefnLink = VAR_FCN_LINK_UNDEFINED;
	if (isConstant(varNode)) {
		// Assume it's a constant, but can just treat it like a variable, making sure that
		//  any variable that starts with a number (i.e. a constant) is never altered.
		varNode->val = atoi(name);
        varNode->type = nodeConst;
	}
}

// The first entry in the array definition contains the max range. The next entry
//  is the contents of a[0], then a[1] ...
// N.B. If maxRange is 2, then maxIndex is 1.
int addArrayToSymbolTable(char* var, const unsigned int maxRange) {
    //printf("addArrayToSymbolTable: %s, %d", var, maxRange);
    symbolNode tmp;
    buildSymbol(var, maxRange, &tmp);
    int found = findSymbol(&tmp);
	if (found == VAR_NOT_FOUND) {
        if (insertSymbol(&tmp) == VAR_TABLE_LIMIT) {
            return VAR_TABLE_LIMIT;
        }
        if (symbolTableFreeIndex + maxRange >= VAR_ITEMS) {
            symbolTableFreeIndex--;    // Remove variable just inserted with insertSymbol().
            return VAR_TABLE_LIMIT;
        }
        symbolTableFreeIndex += maxRange;
    }
    return found;
}

// Return index in symbol table
int addVarToSymbolTable(char* var) {
	symbolNode tmp;
    buildSymbol(var, DEFAULT_VAR_VALUE, &tmp);
	int found = findSymbol(&tmp);
    if (var == NULL) {
        debugAssert(ERR:addVarToSymbolTable():var == NULL);
        return VAR_NOT_FOUND;
    }
	if (found == VAR_NOT_FOUND) {
		return insertSymbol(&tmp);
	}
	return found;
}

// TODO: Return value of this function can be void since we're inserting arguments right in the symbol table. 
//        The only AST is used in a function call, not the definition.
astNode* addFcnDefnArgument(astNode* pArgumentListNode, const char* pArgumentName, const int passByValueOrReference, const int firstOrSebsequentParameter) {
    if (pArgumentName == NULL) {
        debugAssert(ERR:addFcnDefnArgument():pArgumentName == NULL);
        return NULL;
    }
    //printf("\naddFcnDefnArgument():pArgumentName=%s, pArgumentListNode=%x", pArgumentName, (unsigned int)pArgumentListNode);
    // Add symbols to symbol table, marking them as function parameters.
    // Note that the variable value refers to its index on the execution stack.
    symbolNode argumentNode;
    buildSymbol(pArgumentName, functionParameterIndex++, &argumentNode);
    argumentNode.type = nodeArgumentValue;
    if (passByValueOrReference == VAR_PASS_BY_REFERENCE) {
        argumentNode.type = nodeArgumentReference;
    }
    insertSymbol(&argumentNode);
    return NULL;
#if 0    
    // Old code
	astNode* p = getNextASTNode();
    p->type = passByValueOrReference;
    // Create variable in symbol table
    symbolNode tmp;
    buildSymbol(pArgumentName, DEFAULT_VAR_VALUE, &tmp);
    tmp.type = passByValueOrReference;
    //p->Left = 
    p->pNext = pArgumentListNode;
    return NULL;
#endif    
}

astNode* addFcnCallArgument(astNode* pArgumentListNode, astNode* pArgumentNode) {
    if (pArgumentNode == NULL) {
        // There are no arguments! Just return current argument list.
        return pArgumentListNode;
    }
	astNode* pNewArgumentNode = getNextASTNode();
    if (pArgumentNode->type == nodeVariable) {
        // Duplicate names should be handled already. No need to do anything here I think.
    }
    pNewArgumentNode->type = nodeArgumentCall;
	pNewArgumentNode->pLeft = pArgumentNode;
    pNewArgumentNode->pNext = pArgumentListNode;
    //printf("\naddFcnCallArgument()");
	return pNewArgumentNode;
}

astNode* addStatement(astNode* pStatementListNode, astNode* pStatementNode) {
#if REGRESS_1    
    printf("!!addStatement(astNode* pStatementListNode=%d, astNode* pStatementNode=%d)\n", pStatementListNode, pStatementNode);
    printf("pStatementListNode->Next=%d\n", pStatementListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    if (pStatementNode == NULL) {
        // Special case for array definition
        return pStatementListNode;
    }
	astNode* p          = getNextStatementNode();
    p->type             = nodeStatement;
	p->pLeft            = pStatementNode;
    p->pNext            = pStatementListNode;
	return p;
}

// Walk tree in postfix order; left, right, root.
void walkSyntaxTree(astNode* pSyntaxNode, char* position, int indent, FILE* fp) {
	if (pSyntaxNode == NULL) {
		return;
	}
    if (indent == 0) {
        printf("\nStatement:");
    }
	//printf("Start pattern walk");
    if (pSyntaxNode->type != nodeIf && pSyntaxNode->type != nodeWhile) {
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        walkSyntaxTree(pSyntaxNode->pCentre, "CENTRE", indent + 1, fp);  // TODO: Only used by if-then-else and while. Not necessary
        walkSyntaxTree(pSyntaxNode->pRight, "RIGHT", indent + 1, fp);
    }
    printIndent(indent);
    printf("%s", position);
    char tmp[64];
	switch (pSyntaxNode->type) {
	case (nodeOperator):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Operator %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Operator: ");
        printOperator(pSyntaxNode->value);
		break;
	case (nodeVariable):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Var: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
		break;
    case (nodeArray):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		printf(" Array: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        break;
    case (nodeIf):
        if (fp != NULL) {
            sprintf(tmp, "%d %s If %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EVAL0 %d", indent, position, pSyntaxNode->value);
            //sprintf(tmp, "%d %s Then %d\n", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If EVAL == 0 JMP Else %d", pSyntaxNode->value);
		//printf("THEN %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s JmpEndIf %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
            sprintf(tmp, "%d %s Else %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("JMP EndIf %d", pSyntaxNode->value);
		printf("\nElse %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pRight, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndIf %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf("\nEndIf %d", pSyntaxNode->value);
        
        break;
    case (nodeWhile):
        if (fp != NULL) {
            sprintf(tmp, "%d %s While %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("While %d", pSyntaxNode->value);
        walkSyntaxTree(pSyntaxNode->pLeft, "LEFT", indent + 1, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EVALWHILE0 %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printIndent(indent);
		printf("If EVALWHILE == 0 JMP EndWhile %d", pSyntaxNode->value);
		//printf("Do %d", pSyntaxNode->value);
        walkList(pSyntaxNode->pCentre, fp);
        
        if (fp != NULL) {
            sprintf(tmp, "%d %s EndWhile %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf("\nEndWhile %d", pSyntaxNode->value);
        
        break;
    case (nodeStatement):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Statement %d\n", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }

		//printf(" Statement: index,%d", pSyntaxNode->value);
        break;
    case (nodeArgumentCall):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Argument call%d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        //printf("\n Function argument:");
        break;
    case (nodeFunctionCall):
        if (fp != NULL) {
            sprintf(tmp, "%d %s FunctionCall %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf(" FunctionCall: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        //walkList(pSyntaxNode, fp);
        walkListBackwards(pSyntaxNode, fp);
        if (fp != NULL) {
            sprintf(tmp, "%d %s FunctionCallEnd %d", indent, position, pSyntaxNode->value);
            writeStatement(tmp, fp);
        }
        printf(" \nFunctionCallEnd: index,%d name,%s", pSyntaxNode->value, symbolTable[pSyntaxNode->value].name);
        break;
	default:
		printf(" walkSyntaxTree: Unknown type:%d", pSyntaxNode->type);
		break;
	}
	//walkSyntaxTree(pSyntaxNode->pRight, "RIGHT", indent + 1);
	//printf("End pattern walk");
}

void writeStatement(const char* pTmp, FILE* fp) {
    fwrite(STATEMENT_PREFIX_PLUS_SPACE, 1, strlen(STATEMENT_PREFIX_PLUS_SPACE), fp);
    fwrite(pTmp, 1, strlen(pTmp), fp);
    fwrite(STATEMENT_SUFFIX_PLUS_SPACE_AND_EOL, 1, strlen(STATEMENT_SUFFIX_PLUS_SPACE_AND_EOL), fp);
    ++statementOutputIndex;
}

void walkList(astNode* pListNode, FILE* fp) {
#if REGRESS_1
    printf("\nwalkList(astNode* pListNode=%d)\n", (int)pListNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pListNode == NULL) {
		return;
	}
#if REGRESS_1    
    printf("\nwalkList(pListNode->pNext)=%d\n", (int)pListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkList(pListNode->pNext, fp);
    
#if REGRESS_1    
    printf("walkSyntaxTree(pListNode->pLeft=%d, ROOT, 0)\n", (int)pListNode->pLeft);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkSyntaxTree(pListNode->pLeft, "ROOT", 0, fp);
}

void walkListBackwards(astNode* pListNode, FILE* fp) {
#if REGRESS_1
    printf("\nwalkList(astNode* pListNode=%d)\n", (int)pListNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pListNode == NULL) {
		return;
	}

#if REGRESS_1    
    printf("walkSyntaxTree(pListNode->pLeft=%d, ROOT, 0)\n", (int)pListNode->pLeft);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkSyntaxTree(pListNode->pLeft, "ROOT", 0, fp);

    #if REGRESS_1    
    printf("\nwalkList(pListNode->pNext)=%d\n", (int)pListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkListBackwards(pListNode->pNext, fp);
}

unsigned int countArguments(astNode* pArgNode) {
    unsigned int argCount = 0;
    while (pArgNode != NULL) {
        // This node should either be for a function call or function definition
        //printf("\ncountArguments()");
        if (pArgNode->type != nodeArgumentCall && pArgNode->type != nodeArgumentValue && pArgNode->type != nodeArgumentReference) {
            debugAssert(ERR:countArguments());
            // Shouldn't happen
            return 0;
        }
        ++argCount;
        pArgNode = pArgNode->pNext;
    }
    return argCount;
}

void printIndent(const unsigned int indent) {
    int i;
    printf("\n");
    for (i = indent; i > 0; --i) {
        printf("*");
    }
}

void dumpSymbolTable(const char* fileName) {
	int i;
    FILE* fpSymbol = NULL;
    if (fileName != NULL) {
        fpSymbol = fopen(fileName, "wb");
        if (fpSymbol == NULL) {
            debugAssert(ERR:dumpSymbolTable() fpSymbol == NULL);
        }
    }
    printf("\n\nSymbol table start:");
    // Subtract 1 from symbolTableFreeIndex because the last index is a place
    //  holder for the function name.
	for (i = 0; i < symbolTableFreeIndex - 1; ++i) {
		dumpSymbol(i, fpSymbol);
	}
    printf("\nSymbol table end:\n");
    if (fpSymbol != NULL) {
        fclose(fpSymbol);
    }
}

void dumpSymbol(int i, FILE* fpSymbol) {
    //nodeType symbolType = isConstant(&symbolTable[i]) ? nodeConst : nodeVariable;
    nodeType symbolType = symbolTable[i].type;
	printf("\nindex:%d, name:%s, type:%d, val:%d", i, symbolTable[i].name, symbolType, symbolTable[i].val);
    if (symbolTable[i].fcnDefnLink != VAR_FCN_LINK_UNDEFINED) {
        // Function definition link is defined
        printf(", fcnLink:%d", symbolTable[i].fcnDefnLink);
    }
    char tmp[64];
    sprintf(tmp, "%d %d %d", /*symbolTable[i].name,*/ symbolType, symbolTable[i].val, symbolTable[i].fcnDefnLink);
    if (fpSymbol != NULL) {
        fwrite(SYMBOL_PREFIX_PLUS_SPACE, 1, strlen(SYMBOL_PREFIX_PLUS_SPACE), fpSymbol);
        fwrite(tmp, 1, strlen(tmp), fpSymbol);
        fwrite(SYMBOL_SUFFIX_PLUS_SPACE_AND_EOL, 1, strlen(SYMBOL_SUFFIX_PLUS_SPACE_AND_EOL), fpSymbol);
    }
}

#if 0
// Calculate the symbol table index.
int getSymbolTableIndex(symbolNode* pSymbol) {
    if (pSymbol == NULL) {
        debugAssert(ERR:getSymbolTableIndex():pSymbol == NULL);
    }
    return (pSymbol - symbolTable) / sizeof(symbolTable[0]);
}
#endif

void printOperator(const int value) {
    switch (value) {
    case TILDE:
        printf("~");
        break;
    case BANG:
        printf("!");
        break;
    case EQUAL:
        printf("=");
        break;
    case PLUS:
        printf("+");
        break;
    case MINUS:
        printf("-");
        break;
    case MULT:
        printf("*");
        break;
    case DIV:
        printf("/");
        break;
    case XOR:
        printf("^");
        break;
    case GEQ:
        printf(">=");
        break;
    case LEQ:
        printf("<=");
        break;
    case NEQ:
        printf("!=");
        break;
    case GTR:
        printf(">");
        break;
    case LSS:
        printf("<");
        break;
    case AND:
        printf("&&");
        break;
    case OR:
        printf("||");
        break;
    case BITWISEAND:
        printf("&");
        break;
    case BITWISEOR:
        printf("|");
        break;
    case TEST_FOR_EQUAL:
        printf("==");
        break;
    case LBRACKET:
        printf("array");
        break;
    default:
        printf("Unknown operator");
        break;
    }
    printf(" %d", value);
}
