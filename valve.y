
/* Copyright (c) 2011, Gareth Scott */

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
#include <malloc.h>
#include <string.h>
#include "compiler.h"

// Variable symbol table (also contains constants).
varNode varTable[VAR_ITEMS];
unsigned int varTableFreeIndex = 0;

// Arithmetic parse table
arithNode arithTable[ARITH_ITEMS];
unsigned int arithTableFreeIndex = 0;

actionNode actionTable[ACTION_ITEMS];
unsigned int actionTableFreeIndex = 0;

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
%token <number> 	EQUAL PLUS MINUS MULT DIV XOR GEQ LEQ NEQ GTR LSS AND OR TEST_FOR_EQUAL SEMI LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET
%token <string>	VAR VAR_METHOD CONST

%left PLUS MINUS
%left MULT DIV	/* last one gets highest precedence */

/* non-terminals */
%type <number> identifier operator andOr operandTest var array
%type <string> patternAction
%type <pArithNode>  pattern statementAction arithmeticExpression patternCompare
%type <pActionNode>  action

%defines	/* generate valve.tab.h for use with lex.yy.c */

/* %left pattern */

%start program

%% /* Grammar rules and actions */
program: 	patternActionList	{dumpSymbolTable();}
;

patternActionList: /* empty */	{}
					| patternActionList patternAction	
;

patternAction: pattern LBRACE action RBRACE	{walkPatternTree($1, "ROOT", 0); /*walkActionTree($3);*/}
;

action: /* empty */	{}
		| action statementAction	{$$ = addNodeOperatorAction($1, $2);}
;

statementAction:	var EQUAL arithmeticExpression SEMI	{$$ = addNodeVarOperand($2, $1, $3);}
;			

/* eg: patternCompare && patternCompare */
pattern: 	LPAREN pattern RPAREN	{$$ = $2;}
			| pattern andOr pattern	{$$ = addNodeOperator($2, $1, $3);}
			| patternCompare		{$$ = $1;}
;

patternCompare:	var operandTest arithmeticExpression	{$$ = addNodeVarOperand($2, $1, $3);}
				| identifier 							{$$ = addNodeId($1);}
;

arithmeticExpression:	LPAREN arithmeticExpression RPAREN						{$$ = $2;}
						| arithmeticExpression operator arithmeticExpression	{$$ = addNodeOperator($2, $1, $3);}
						| identifier											{$$ = addNodeId($1);}
;						

identifier:	var		{$$ = $1;}	/* Set top of stack to index of this variable in symbol table. */
			| CONST {$$ = addNodeVar($1);}
;			

var:		VAR				{$$ = addNodeVar($1);}
			| VAR_METHOD	{$$ = addNodeVar($1);}
            | array         {$$ = $1;}
;			

array:      LBRACKET identifier RBRACKET    {$$ = $2;}
;

andOr:	AND		{$$ = AND;}
		| OR	{$$ = OR;}
;			

operandTest:	TEST_FOR_EQUAL	{$$ = TEST_FOR_EQUAL;}
				| GEQ			{$$ = GEQ;}
				| LEQ			{$$ = LEQ;}
				| GTR			{$$ = GTR;}
				| LSS			{$$ = LSS;}
;

operator:	PLUS		{$$ = PLUS;}
			| MINUS		{$$ = MINUS;}
			| MULT		{$$ = MULT;}
			| DIV		{$$ = DIV;}
			| XOR		{$$ = XOR;}
;			


%% /* Additional C code */


#include "lex.yy.c"

void yyerror (char* s)
{
	printf (" %s\n", s);
}

main ()
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
    yyin = fopen("valve3.def", "r" );
	yyparse ();
}

void initVarTable(void) {
	int len = VAR_ITEMS - 1;
	for (; len >= 0; len--) {
		varTable[len].name[0] = EOS;
		varTable[len].val = 0;	// Initialize variable to 0;
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
		//assert(p != NULL);
		return p;
	}
	p->type = nodeOperator;
	p->value = operator;
	p->pLeft = pLeft;
	p->pRight = pRight;
	return p;	
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

int addNodeVar(char* var) {
	varNode tmp;
	strncpy(tmp.name, var, VAR_NAME_LENGTH-1);
	tmp.name[VAR_NAME_LENGTH-1] = EOS;
	tmp.val = 0;
	if (isConstant(&tmp)) {
		// Assume it's a constant, but can just treat it like a variable, making sure that
		//  any variable that starts with a numberic (i.e. a constant) is never altered.
		tmp.val = atoi(var);
	}
	int found = findVariable(&tmp);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(&tmp);
	}
	return found;
}


// The following 2 functions are only used by 'action' in the 
//  'pattern {action} part of the grammar.
actionNode* addNodeOperatorAction(actionNode* pActionNode, arithNode* pArithNode) {
	actionNode* p = getNextActionNode();
	if (p == NULL) {
		//assert(p != NULL);
		return p;
	}
	p->pArith = pArithNode;
	p->pNext = pActionNode;
	return p;
}

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
	switch (pArithNode->type) {
	case (nodeOperator):
		printf(" Operator: %d", pArithNode->value);
		break;
	case (nodeVar):
		printf(" Var: index,%d name,%s", pArithNode->value, varTable[pArithNode->value].name);
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

void walkActionTree(arithNode* pArithNode) {
	if (pArithNode == NULL) {
		return;
	}
	walkActionTree(pArithNode->pRight);
	if (pArithNode->operand == enumAction) {
			printf("\nsetOutput(\"%s\"); ", pArithNode->idValue);
	} else {
		//assert(false);
	}
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
	printf("\nindex:%d, name:%s, val:%d", i, varTable[i].name, varTable[i].val);
}