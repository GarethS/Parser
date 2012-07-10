
/* Copyright (c) 2011, 2012 Gareth Scott */

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

// Variable symbol table (also contains constants).
varNode varTable[VAR_ITEMS];
unsigned int varTableFreeIndex = 0;

// Arithmetic parse table
arithNode arithTable[ARITH_ITEMS];
unsigned int arithTableFreeIndex = 0;

actionNode actionTable[ACTION_ITEMS];
unsigned int actionTableFreeIndex = 0;
FILE* fp = NULL; // Parse tree file pointer
FILE* fpSymbol = NULL;  // Symbol table file point

%}

/* See compiler.h for definition */
%union {
	int number;
	char* string;
	arithNode* pArithNode;
	actionNode* pActionNode;
}

/* TERMINALS */
%token INPUTS OUTPUTS  COMMA BANG
%token EQUAL GEQ LEQ NEQ GTR LSS TEST_FOR_EQUAL LBRACE RBRACE ARRAYDEFINE IF ELSE
%token <string>	VAR VAR_METHOD CONST

%left LPAREN 
%left PLUS MINUS
%left AND OR XOR
%left MULT DIV	/* last one gets highest precedence */
%left LBRACKET
%right RPAREN RBRACKET
%right SEMI

/* non-terminals */
%type <number>  operandTest var arrayDefine
%type <string> statementIf
%type <pArithNode>  expr statement statementAssign arithmeticExpression array
%type <pActionNode>  statementList

%defines	/* generate valve.tab.h for use with lex.yy.c */

/* %left pattern */

%start program

%% /* Grammar rules and actions */
program: 	patternActionList	{fpSymbol = fopen("symbolTable.txt", "wb");
                                    dumpSymbolTable();
                                    fclose(fpSymbol);}
;

patternActionList: /* empty */	{}
					| patternActionList statementIf	
;

statementList: /* empty */	{}
		| statementList statement	{$$ = addNodeOperatorAction($1, $2);}
;

statement:	        statementAssign	    {$$ = $1;}
                    | arrayDefine {$$ = NULL;}
;			

statementIf:  IF LPAREN expr RPAREN LBRACE statementList RBRACE	{fp = fopen("patternTree.txt", "wb"); walkPatternTree($3, "ROOT", 0); fclose(fp);
                                             printf("\n\n"); fp = fopen("actionTree.txt", "wb");
                                             fwrite("0 0 Action 0\n", 1, 13/* strlen("0 0 Action 0\n") */, fp); walkActionTree($6); fclose(fp);}
              | IF LPAREN expr RPAREN LBRACE statementList RBRACE ELSE LBRACE statementList RBRACE	{fp = fopen("patternTree.txt", "wb"); walkPatternTree($3, "ROOT", 0); fclose(fp);
                                             printf("\n\n"); fp = fopen("actionTree.txt", "wb");
                                             fwrite("0 0 Action 0\n", 1, 13/* strlen("0 0 Action 0\n") */, fp); walkActionTree($6); fclose(fp); fwrite("0 0 Action 0\n", 1, 13/* strlen("0 0 Action 0\n") */, fp); walkActionTree($10); fclose(fp);}
;

statementAssign:    var EQUAL arithmeticExpression SEMI	    {$$ = addNodeVarOperand(EQUAL, $1, $3);}
                    | array EQUAL arithmeticExpression SEMI {$$ = addNodeOperator(EQUAL, $1, $3);}
;    

expr: 	    LPAREN expr RPAREN	{$$ = $2;} 
			| expr AND expr	    {$$ = addNodeOperator(AND, $1, $3);}
			| expr OR expr	    {$$ = addNodeOperator(OR, $1, $3);}
            | var operandTest arithmeticExpression	{$$ = addNodeVarOperand($2, $1, $3);}
            | var               {$$ = addNodeId($1);}
            | CONST             {$$ = addNodeId(addNodeVar($1));}
;

arithmeticExpression:	LPAREN arithmeticExpression RPAREN					{$$ = $2;}
						| arithmeticExpression PLUS  arithmeticExpression	{$$ = addNodeOperator(PLUS, $1, $3);}
						| arithmeticExpression MINUS arithmeticExpression	{$$ = addNodeOperator(MINUS, $1, $3);}
						| arithmeticExpression MULT  arithmeticExpression	{$$ = addNodeOperator(MULT, $1, $3);}
						| arithmeticExpression DIV   arithmeticExpression	{$$ = addNodeOperator(DIV, $1, $3);}
						| arithmeticExpression XOR   arithmeticExpression	{$$ = addNodeOperator(XOR, $1, $3);}
						| var											    {$$ = addNodeId($1);}
						| CONST											    {$$ = addNodeId(addNodeVar($1));}
                        | array                                             {$$ = $1;}
;						

var:		VAR				{$$ = addNodeVar($1);}
			| VAR_METHOD	{$$ = addNodeVar($1);}
;			

// identifier replaced by arithmeticExpression
array:      VAR LBRACKET arithmeticExpression RBRACKET    {$$ = addNodeArray($1, $3);}
/*             | VAR LBRACKET CONST RBRACKET    {$$ = addNodeArrayConstIndex($1, addNodeVar($3));} */
;

arrayDefine:    ARRAYDEFINE VAR LBRACKET CONST RBRACKET SEMI    {$$ = addArrayToSymbolTable($2, atoi($4));}
;

operandTest:	TEST_FOR_EQUAL	{$$ = TEST_FOR_EQUAL;}
				| GEQ			{$$ = GEQ;}
				| LEQ			{$$ = LEQ;}
				| GTR			{$$ = GTR;}
				| LSS			{$$ = LSS;}
;

%% /* Additional C code */


#include "lex.yy.c"

void yyerror (char* s)
{
	printf (" %s\n", s);
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

	initVarTable();
	// To turn on debugging, make sure the next line is uncommented and
	//  turn on the -t (also use -v -l) options in bison.exe.
	yydebug = 1; 
    yyin = fopen("valve4.def", "r" );
	yyparse ();
    return 0;
}

void initVarTable(void) {
	int len;
	for (len = VAR_ITEMS - 1; len >= 0; len--) {
		varTable[len].name[0] = EOS;
		varTable[len].val = DEFAULT_VAR_VALUE;	// Initialize variable to 0
	}
}

// Return index of variable or constant in symbol table
int insertVariable(varNode* pVar) {
	if (varTableFreeIndex < VAR_ITEMS) {
		varTable[varTableFreeIndex] = *pVar;
		return varTableFreeIndex++;
	}
	return VAR_TABLE_LIMIT;
}

#if 0
// Get value of variable or constant.
// Return index where variable is located in varTable, or -1 on failure.
int getVariableIndex(varNode* pVar) {
	int found = findVariable(pVar);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(pVar);
	}
	return found;
}

// Set value of variable but not of constant.
// Return index of variable or constant in symbol table
int setVariable(varNode* pVar) {
	int found = findVariable(pVar);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(pVar);
	} else {
		// found it
		varTable[found].val = pVar->val;
	}
	return found;
}
#endif

// Return index where variable is located in varTable, or -1 on failure.
int findVariable(varNode* pVar) {
	int i;
	for (i = 0; i < varTableFreeIndex; ++i) {
		if (strncmp(varTable[i].name, pVar->name, VAR_NAME_LENGTH-1) == 0) {
			return i;
		}
	}
	return VAR_NOT_FOUND;
}

void pushInputLine(char* line) {
	printf("\nsetLineAsInput(\"%s\");", line);
}

void pushOutputLine(char* line) {
	printf("\nsetLineAsOutput(\"%s\");", line);
}

arithNode* addNodeOperator(int operator, arithNode* pLeft, arithNode* pRight) {
	arithNode* p = getNextArithNode();
	if (p == NULL) {
        printf("ERROR:addNodeOperator()");
		//assert(p != NULL);
		return p;
	}
	p->type = nodeOperator;
	p->value = operator;
	p->pLeft = pLeft;
	p->pRight = pRight;
	return p;	
}

arithNode* addNodeArrayConstIndex(char* pVarName, int symbolTableIndex) {
    arithNode* pArrayNode = addNodeArray(pVarName, addNodeId(symbolTableIndex));
    printf("root=%d, left=%d, right=%d", pArrayNode->value, pArrayNode->pLeft->value, pArrayNode->pRight->value);
    //walkPatternTree(pArrayNode, "rootXXX", 0);
    return pArrayNode;
}

arithNode* addNodeArray(char* pVarName, arithNode* pIndex) {
    // 1. Make new arithNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pIndex->value, pIndex->pLeft->value, pIndex->pRight->value);
    //printf("xxx root=%d\n", pIndex->value);
	arithNode* pArrayVar = getNextArithNode();
	if (pArrayVar == NULL) {
        debugAssert(ERR:addNodeArray():pArrayVar == NULL);
		//assert(pArrayVar != NULL);
		return pArrayVar;
	}
	pArrayVar->type = nodeArray;
    varNode pArrayNode;
    buildNodeVar(pVarName, 0, &pArrayNode);
	pArrayVar->value = findVariable(&pArrayNode);
    if (pArrayVar->value == VAR_NOT_FOUND) {
        debugAssert(ERR: addNodeArray():VAR_NOT_FOUND);
    }
#if 1
    // This fails
    return addNodeOperator(LBRACKET, pArrayVar, pIndex);
#else    
    // This passes, but looks like it's using the wrong index in the array
    //return addNodeOperator(LBRACKET, pArrayVar, pIndex);
    arithNode* pan = addNodeOperator(LBRACKET, pArrayVar, pIndex);
    //printf("yyyroot=%d, left=%d, right=%d", pan->value, pan->pLeft->value, pan->pRight->value);
    return pan;
#endif    
}

// e.g. c3 == 4 * c1;
arithNode* addNodeVarOperand(int operator, int varIndex, arithNode* pRight) {
	arithNode* p = getNextArithNode();
	if (p == NULL) {
		//assert(p != NULL);
		return p;
	}
	p->type = nodeOperator;
	p->value = operator;
	//printf("** operator=%d", operator);

	p->pLeft = getNextArithNode();
	if (p->pLeft == NULL) {
		//assert(p->pLeft != NULL);
		return p->pLeft;
	}
	p->pLeft->type = nodeVar;
	p->pLeft->value = varIndex;

	p->pRight = pRight;
	return p;	
}

arithNode* addNodeId(int varIndex) {
	arithNode* p = getNextArithNode();
	if (p == NULL) {
		//assert(p != NULL);
		return p;
	}
	p->type = nodeVar;
	p->value = varIndex;
	p->pLeft = NULL;
	p->pRight = NULL;
	return p;	
}

arithNode* getNextArithNode(void) {
	if (arithTableFreeIndex < ARITH_ITEMS) {
		return arithTable + arithTableFreeIndex++;
	}
	return NULL;
}

actionNode* getNextActionNode(void) {
	if (actionTableFreeIndex < ACTION_ITEMS) {
#if REGRESS_1    
        printf("getNextActionNode() %d\n", actionTableFreeIndex);
        fflush(stdout);
#endif /* REGRESS_1 */        
		return actionTable + actionTableFreeIndex++;
	}
	return NULL;
}

// A variable may not start with a number. One that does we'll consider a constant.
int isConstant(varNode* pVar) {
	if (isdigit(pVar->name[0])) {
		return TRUE;
	}
	return FALSE;
}

void buildNodeVar(char* name, int value, varNode* varNode) {
    if (name == NULL || varNode == NULL) {
        debugAssert(ERR:buildNodeVar());
        return;
    }
	strncpy(varNode->name, name, VAR_NAME_LENGTH-1);
	varNode->name[VAR_NAME_LENGTH-1] = EOS;
	varNode->val = value;
	if (isConstant(varNode)) {
		// Assume it's a constant, but can just treat it like a variable, making sure that
		//  any variable that starts with a number (i.e. a constant) is never altered.
		varNode->val = atoi(name);
	}
}

// The first entry in an array definition contains the max range. The next entry
//  is the contents of a[0], then a[1] ...
// N.B. If maxRange is 2, then maxIndex is 1.
int addArrayToSymbolTable(char* var, const unsigned int maxRange) {
    //printf("addArrayToSymbolTable: %s, %d", var, maxRange);
    varNode tmp;
    buildNodeVar(var, maxRange, &tmp);
    int found = findVariable(&tmp);
	if (findVariable(&tmp) == VAR_NOT_FOUND) {
        if (insertVariable(&tmp) == VAR_TABLE_LIMIT) {
            return VAR_TABLE_LIMIT;
        }
        if (varTableFreeIndex + maxRange >= VAR_ITEMS) {
            varTableFreeIndex--;    // Remove variable just insterted with insertVariable().
            return VAR_TABLE_LIMIT;
        }
        varTableFreeIndex += maxRange;
    }
    return found;
}

// Return index in symbol table
int addNodeVar(char* var) {
	varNode tmp;
    buildNodeVar(var, DEFAULT_VAR_VALUE, &tmp);
	int found = findVariable(&tmp);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(&tmp);
	}
	return found;
}


// The following 2 functions are only used by 'action' in the 
//  'pattern {action} part of the grammar.
// Where does the first pActionNode get set?????
actionNode* addNodeOperatorAction(actionNode* pActionNode, arithNode* pArithNode) {
    static int first = TRUE;
    
    if (first) {
        first = FALSE;
        // This is tricky. The first call into this function doesn't have pActionNode
        //  set to anything so we null it out here.
        pActionNode = NULL;
    }
#if REGRESS_1    
    printf("!!addNodeOperatorAction(actionNode* pActionNode=%d, arithNode* pArithNode=%d)\n", pActionNode, pArithNode);
    //printf("pActionNode->Next=%d\n", pActionNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    if (pArithNode == NULL) {
        // Special case for array definition
        return pActionNode;
    }
	actionNode* p = getNextActionNode();
	if (p == NULL) {
		//assert(p != NULL);
		return p;
	}
	p->pArith = pArithNode;
    p->pNext = pActionNode;
	return p;
}

#if 0
arithNode* addNodeActionId(char* id) {
	//assert(id != NULL);
	arithNode* p = malloc(sizeof(arithNode));
	if (p == NULL) {
		//assert(p != NULL);
		yyerror("malloc() failed in call to addActionNodeId()");
	}
	p->operand = enumAction;
	// Remember an id can be 'a3' or '!g7'.
	p->idValue[0] = id[0];
	p->idValue[1] = id[1];
	p->idValue[2] = id[2];
	if (id[0] == '!') {
		p->idValue[3] = id[3];
	}
	p->pLeft = NULL;
	p->pRight = NULL;
	return p;	
}
#endif

#if 0
void doPatternAction(arithNode* pPattern, arithNode* pAction)
{
	{
	/* Output statement. */
	printf("\nif ");
	// Output Pattern statements.
	walkPatternTree(pPattern, FALSE);
	printf("  {");

	// Output Action statements.
	walkActionTree(pAction, FALSE);

	printf("\n/* Latch data into LED(0). */");
	printf("\nsetOutput(\"!e7\");");
	printf("\nsetOutput(\"e7\");");
	printf("\n} /* if */\n");
	}

	{
	/* Output complement statement. */
	printf("\nif ");
	// Output Pattern statements.
	walkPatternTree(pPattern, TRUE);
	printf("  {");

	// Output Action statements.
	walkActionTree(pAction, TRUE);

	printf("\n/* Latch data into LED(0). */");
	printf("\nsetOutput(\"!e7\");");
	printf("\nsetOutput(\"e7\");");
	printf("\n} /* if */\n");
	}

	//freeNode(pPattern);
	//freeNode(pAction);
}
#endif

void printOperator(int value) {
    switch (value) {
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

// Walk tree in infix mode; left, right, root.
void walkPatternTree(arithNode* pArithNode, char* position, int indent) {
	if (pArithNode == NULL) {
		return;
	}
    if (indent == 0) {
        printf("Pattern tree:");
    }
	//printf("Start pattern walk");
	walkPatternTree(pArithNode->pLeft, "LEFT", indent + 1);
	walkPatternTree(pArithNode->pRight, "RIGHT", indent + 1);
#if 1
    printIndent(indent);
    printf("%s", position);
    char tmp[64];
	switch (pArithNode->type) {
	case (nodeOperator):
        sprintf(tmp, "%d %s Operator %d\n", indent, position, pArithNode->value);
        fwrite(tmp, 1, strlen(tmp), fp);

		printf(" Operator: ");
        printOperator(pArithNode->value);
		break;
	case (nodeVar):
        sprintf(tmp, "%d %s Variable %d\n", indent, position, pArithNode->value);
        fwrite(tmp, 1, strlen(tmp), fp);

		printf(" Var: index,%d name,%s", pArithNode->value, varTable[pArithNode->value].name);
		break;
    case (nodeArray):
        sprintf(tmp, "%d %s Variable %d\n", indent, position, pArithNode->value);
        fwrite(tmp, 1, strlen(tmp), fp);

		printf(" Array: index,%d name,%s", pArithNode->value, varTable[pArithNode->value].name);
        break;
	default:
		printf(" walkPatternTree: Unknown type");
		break;
	}
#else	
	if (pArithNode->operand == enumId) {
			printf(" getInput(\"%s\") ", pArithNode->idValue);
	} else if (pArithNode->operand == enumAnd) {
			printf(" && ");
	} else if (pArithNode->operand == enumOr) {
			printf(" || ");
	} else {
		//assert(false);
	}
#endif
	//walkPatternTree(pArithNode->pRight, "RIGHT", indent + 1);
	//printf("End pattern walk");
}

void printIndent(unsigned int indent) {
    int i;
    printf("\n");
    for (i = indent; i > 0; --i) {
        printf("*");
    }
}

void walkActionTree(actionNode* pActionNode) {
#if REGRESS_1
    printf("walkActionTree(actionNode* pActionNode=%d)\n", pActionNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pActionNode == NULL) {
		return;
	}
#if REGRESS_1    
    printf("walkActionTree(pActionNode->pNext)=%d\n", pActionNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkActionTree(pActionNode->pNext);
    
#if REGRESS_1    
    printf("walkPatternTree(pActionNode->pArith=%d, ROOT, 0)\n", pActionNode->pArith);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkPatternTree(pActionNode->pArith, "ROOT", 0);
}

int infixPatternTraversal(arithNode* pn) {
	if (pn->type == nodeOperator) {
		int valueLeft = infixPatternTraversal(pn->pLeft);
		// Short-circuit evaluation
		if (pn->value == OP_OR && valueLeft != FALSE) {
			return valueLeft;
		}
		int valueRight = infixPatternTraversal(pn->pRight);
		switch (pn->value) {
		case OP_PLUS:
			return valueLeft + valueRight;
		case OP_MINUS:
			return valueLeft - valueRight;
		case OP_MULT:
			return valueLeft * valueRight;
		case OP_DIV:
			return valueLeft / valueRight;
		default:
			// Got unrecognized opcode!
			break;
		}
	}
	// It's an operand so just return its value.
	return pn->value;
}

void dumpSymbolTable(void) {
	int i;
    printf("\n\nSymbol table:");
	for (i = 0; i < varTableFreeIndex; ++i) {
		dumpSymbol(i);
	}
}

void dumpSymbol(int i) {
    nodeType symbolType = isConstant(&varTable[i]) ? nodeConst : nodeVar;
	printf("\nindex:%d, name:%s, type:%d, val:%d", i, varTable[i].name, symbolType, varTable[i].val);
    char tmp[64];
    sprintf(tmp, "%d %d\n", /*varTable[i].name,*/ symbolType, varTable[i].val);
    if (fpSymbol != NULL) {
        fwrite(tmp, 1, strlen(tmp), fpSymbol);
    }
}