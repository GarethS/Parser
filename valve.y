
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

// Abstract syntax tree table
syntaxNode syntaxTable[SYNTAX_ITEMS];
unsigned int syntaxTableFreeIndex = 0;

syntaxNode statementTable[STATEMENT_ITEMS];
unsigned int statementTableFreeIndex = 0;
FILE* fp = NULL; // Parse tree file pointer
FILE* fpSymbol = NULL;  // Symbol table file point

%}

/* See compiler.h for definition */
%union {
	int integer;
    float floatingPoint;
	char* string;
	syntaxNode* pArithNode;
}

/* TERMINALS */
%token INPUTS OUTPUTS  COMMA BANG
%token EQUAL LBRACE RBRACE ARRAYDEFINE IF ELSE WHILE
%token <string>	VAR VAR_METHOD CONST CONST_FLOAT

%left AND OR
%left TEST_FOR_EQUAL GEQ LEQ NEQ GTR LSS
%left LPAREN 
%left PLUS MINUS
%left XOR
%left MULT DIV	
%left LBRACKET
%right RPAREN RBRACKET
%right SEMI
/* last entry gets highest precedence */

/* non-terminals */
%type <integer> arrayDefine
//%type <string>
%type <pArithNode>  statement statementAssign statementIf expr statementList

%defines	/* generate valve.tab.h for use with lex.yy.c */

/* %left pattern */

%start program

%% /* Grammar rules and actions */
program: 	statementList	{fpSymbol = fopen("symbolTable.txt", "wb"); dumpSymbolTable(); fclose(fpSymbol);
//                            fp = fopen("tree.txt", "wb"); fwrite("0 0 Start 0\n", 1, 12, fp); printf("\nstatementTableFreeIndex=%d", statementTableFreeIndex); walkStatementList(statementTable+5); fclose(fp);}
//                            fp = fopen("tree.txt", "wb"); fwrite("0 0 Start 0\n", 1, 12, fp); printf("\nstatementTableFreeIndex=%d", statementTableFreeIndex); walkStatementList(statementTable+statementTableFreeIndex-1); fclose(fp);}
                            fp = fopen("tree.txt", "wb"); fwrite("0 0 Start 0\n", 1, 12, fp); /*printf("\nstatementList=%d", (int)$1);*/ walkStatementList($1); fclose(fp);}
;

statementList: /* empty */	{$$ = NULL;}    // Make sure 'statementList' starts out as NULL for each new 'statementList'
		| statementList statement	{/*printf("\nstatementList:%d, statement:%d", (int)$1, (int)$2);*/ /*walkSyntaxTree($2, "start", 0);*/ $$ = addStatement($1, $2); /*printf(" newStatementList:%d", (int)$$);*/}
;

statement:	        statementAssign	    {/*printf("\nstatementAssign:%d", (int)$1); walkSyntaxTree($1, "start", 0);*/ $$ = $1;}
                    | statementIf       {/*printf("\nstatementIf:%d", (int)$1);*/ $$ = $1;}
                    | arrayDefine       {$$ = NULL;}
;			

statementIf:  IF LPAREN expr RPAREN LBRACE statementList RBRACE	{/*printf("statementIf:statementList:%d", (int)$6);*/ $$ = addNodeIf($3, $6, NULL);}
//statementIf:  IF LPAREN expr RPAREN LBRACE statementList RBRACE	{fp = fopen("patternTree.txt", "wb"); walkSyntaxTree($3, "ROOT", 0); fclose(fp);
//                                             printf("\n\n"); fp = fopen("actionTree.txt", "wb");
//                                             fwrite("0 0 Action 0\n", 1, 13/* strlen("0 0 Action 0\n") */, fp); walkStatementList($6); fclose(fp);}
              | IF LPAREN expr RPAREN LBRACE statementList RBRACE ELSE LBRACE statementList RBRACE	{/*printf("statementIf:statementList:%d", (int)$6);*/ $$ = addNodeIf($3, $6, $10);}
;

statementAssign:    VAR EQUAL expr SEMI	    {$$ = addNodeVariableOperator(EQUAL, addVarToSymbolTable($1), $3);}
//                    | array EQUAL expr SEMI {$$ = addNodeBinaryOperator(EQUAL, $1, $3);}
                      | VAR LBRACKET expr RBRACKET EQUAL expr SEMI {$$ = addNodeBinaryOperator(EQUAL, addNodeArray($1, $3), $6);} // array
;    

expr:	LPAREN expr RPAREN	{$$ = $2;}
		| expr PLUS  expr	{$$ = addNodeBinaryOperator(PLUS, $1, $3);}
		| expr MINUS expr	{$$ = addNodeBinaryOperator(MINUS, $1, $3);}
		| expr MULT  expr	{$$ = addNodeBinaryOperator(MULT, $1, $3);}
		| expr DIV   expr	{$$ = addNodeBinaryOperator(DIV, $1, $3);}
		| expr XOR   expr	{$$ = addNodeBinaryOperator(XOR, $1, $3);}
		| expr AND   expr	{$$ = addNodeBinaryOperator(AND, $1, $3);}
		| expr OR    expr	{$$ = addNodeBinaryOperator(OR, $1, $3);}
		| expr TEST_FOR_EQUAL expr	{$$ = addNodeBinaryOperator(TEST_FOR_EQUAL, $1, $3);}
		| expr NEQ   expr	{$$ = addNodeBinaryOperator(NEQ, $1, $3);}
		| expr GEQ   expr	{$$ = addNodeBinaryOperator(GEQ, $1, $3);}
		| expr LEQ   expr	{$$ = addNodeBinaryOperator(LEQ, $1, $3);}
		| expr GTR   expr	{$$ = addNodeBinaryOperator(GTR, $1, $3);}
		| expr LSS   expr	{$$ = addNodeBinaryOperator(LSS, $1, $3);}
		| VAR				{$$ = addNodeSymbolIndex(addVarToSymbolTable($1));}
		| CONST				{$$ = addNodeSymbolIndex(addVarToSymbolTable($1));}
        | VAR LBRACKET expr RBRACKET        {$$ = addNodeArray($1, $3);}    // array
;						

arrayDefine:    ARRAYDEFINE VAR LBRACKET CONST RBRACKET SEMI    {$$ = addArrayToSymbolTable($2, atoi($4));}
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

// e.g. if (x==2) {x = 1;} else {x = 4;}
syntaxNode* addNodeIf(syntaxNode* pExpr, syntaxNode* pIfStatementList, syntaxNode* pElseStatementList) {
	syntaxNode* p = getNextArithNode();
	if (p == NULL) {
        debugAssert(ERR:addNodeIf():p == NULL);
		return p;
	}
    p->type = nodeIf;
    p->pLeft = pExpr;
    p->pCentre = pIfStatementList;
    p->pRight = pElseStatementList;
    return p;
}

// e.g. '4 * c1'
syntaxNode* addNodeBinaryOperator(int operator, syntaxNode* pLeft, syntaxNode* pRight) {
	syntaxNode* p = getNextArithNode();
	if (p == NULL) {
        debugAssert("ERROR:addNodeBinaryOperator():p == NULL");
		return p;
	}
	p->type = nodeOperator;
	p->value = operator;
	p->pLeft = pLeft;
	p->pRight = pRight;
	return p;	
}

// e.g. 'c3 == 4 * c1;' where 'operator' is '='
syntaxNode* addNodeVariableOperator(int operator, int varIndex, syntaxNode* pRight) {
	syntaxNode* pLeft= getNextArithNode();
	if (pLeft == NULL) {
        debugAssert(ERR:addNodeVariableOperator():pLeft == NULL);
		return pLeft;
	}
	pLeft->type = nodeVar;
	pLeft->value = varIndex;
    return addNodeBinaryOperator(operator, pLeft, pRight);
}

/*
syntaxNode* addNodeArrayConstIndex(char* pVarName, int symbolTableIndex) {
    syntaxNode* pArrayNode = addNodeArray(pVarName, addNodeSymbolIndex(symbolTableIndex));
    printf("root=%d, left=%d, right=%d", pArrayNode->value, pArrayNode->pLeft->value, pArrayNode->pRight->value);
    //walkSyntaxTree(pArrayNode, "rootXXX", 0);
    return pArrayNode;
}
*/

syntaxNode* addNodeArray(char* pVarName, syntaxNode* pArrayIndex) {
    // 1. Make new syntaxNode to contain index of array (starting point). Actual array
    //     index can't be determined until run time.
    //printf("xxx root=%d, left=%d, right=%d", pArrayIndex->value, pArrayIndex->pLeft->value, pArrayIndex->pRight->value);
    //printf("xxx root=%d\n", pArrayIndex->value);
	syntaxNode* pArrayVar = getNextArithNode();
	if (pArrayVar == NULL) {
        debugAssert(ERR:addNodeArray():pArrayVar == NULL);
		return pArrayVar;
	}
	pArrayVar->type = nodeArray;
    varNode pArrayNode;
    buildVariable(pVarName, 0, &pArrayNode);
	pArrayVar->value = findVariable(&pArrayNode);
    if (pArrayVar->value == VAR_NOT_FOUND) {
        debugAssert(ERR: addNodeArray():VAR_NOT_FOUND);
    }
#if 1
    // This fails
    return addNodeBinaryOperator(LBRACKET, pArrayVar, pArrayIndex);
#else    
    // This passes, but looks like it's using the wrong index in the array
    //return addNodeBinaryOperator(LBRACKET, pArrayVar, pArrayIndex);
    syntaxNode* pan = addNodeBinaryOperator(LBRACKET, pArrayVar, pArrayIndex);
    //printf("yyyroot=%d, left=%d, right=%d", pan->value, pan->pLeft->value, pan->pRight->value);
    return pan;
#endif    
}

syntaxNode* addNodeSymbolIndex(int varIndex) {
	syntaxNode* p = getNextArithNode();
	if (p == NULL) {
		debugAssert("ERR:addNodeSymbolIndex():p == NULL");
		return p;
	}
	p->type = nodeVar;
	p->value = varIndex;
	return p;	
}

void initNode(syntaxNode* pArithNode) {
    if (pArithNode == NULL) {
        debugAssert(ERR:initNode());
        return;
    }
    pArithNode->pLeft = NULL;
    pArithNode->pRight = NULL;
    pArithNode->pCentre = NULL;
    pArithNode->pNext = NULL;
}

syntaxNode* getNextArithNode(void) {
	if (syntaxTableFreeIndex < SYNTAX_ITEMS) {
        // Initialize all members before making this node available
        initNode(syntaxTable + syntaxTableFreeIndex);
		return syntaxTable + syntaxTableFreeIndex++;
	}
	return NULL;
}

syntaxNode* getNextStatementNode(void) {
	if (statementTableFreeIndex < STATEMENT_ITEMS) {
#if REGRESS_1    
        printf("getNextStatementNode() %d\n", statementTableFreeIndex);
        fflush(stdout);
#endif /* REGRESS_1 */
        initNode(statementTable + statementTableFreeIndex);
		return statementTable + statementTableFreeIndex++;
	}
	return NULL;
}

// A variable may not start with a number. One that does we'll consider a constant.
int isConstant(varNode* pVar) {
	if (isdigit((int)pVar->name[0])) {
		return TRUE;
	}
	return FALSE;
}

void buildVariable(char* name, int value, varNode* varNode) {
    if (name == NULL || varNode == NULL) {
        debugAssert(ERR:buildVariable());
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
    buildVariable(var, maxRange, &tmp);
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
int addVarToSymbolTable(char* var) {
	varNode tmp;
    buildVariable(var, DEFAULT_VAR_VALUE, &tmp);
	int found = findVariable(&tmp);
	if (found == VAR_NOT_FOUND) {
		return insertVariable(&tmp);
	}
	return found;
}

syntaxNode* addStatement(syntaxNode* pStatementListNode, syntaxNode* pStatementNode) {
#if REGRESS_1    
    printf("!!addStatement(syntaxNode* pStatementListNode=%d, syntaxNode* pStatementNode=%d)\n", pStatementListNode, pStatementNode);
    printf("pStatementListNode->Next=%d\n", pStatementListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    if (pStatementNode == NULL) {
        // Special case for array definition
        return pStatementListNode;
    }
	syntaxNode* p = getNextStatementNode();
	if (p == NULL) {
		//assert(p != NULL);
		return p;
	}
    p->type = nodeStatement;
	p->pLeft = pStatementNode;
    p->pNext = pStatementListNode;
	return p;
}

// Walk tree in infix mode; left, right, root.
void walkSyntaxTree(syntaxNode* pArithNode, char* position, int indent) {
	if (pArithNode == NULL) {
		return;
	}
    if (indent == 0) {
        printf("\nStatement:");
    }
	//printf("Start pattern walk");
    if (pArithNode->type != nodeIf) {
        walkSyntaxTree(pArithNode->pLeft, "LEFT", indent + 1);
        walkSyntaxTree(pArithNode->pCentre, "CENTRE", indent + 1);  // TODO: Only used by if-then-else. Not necessary
        walkSyntaxTree(pArithNode->pRight, "RIGHT", indent + 1);
    }
    printIndent(indent);
    printf("%s", position);
    char tmp[64];
	switch (pArithNode->type) {
	case (nodeOperator):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Operator %d\n", indent, position, pArithNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Operator: ");
        printOperator(pArithNode->value);
		break;
	case (nodeVar):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d\n", indent, position, pArithNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Var: index,%d name,%s", pArithNode->value, varTable[pArithNode->value].name);
		break;
    case (nodeArray):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Variable %d\n", indent, position, pArithNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		printf(" Array: index,%d name,%s", pArithNode->value, varTable[pArithNode->value].name);
        break;
    case (nodeIf):
        if (fp != NULL) {
            sprintf(tmp, "%d %s If %d\n", indent, position, pArithNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

        //printf("\n");
        printIndent(indent);
		printf("IF");
        walkSyntaxTree(pArithNode->pLeft, "LEFT", indent + 1);
        
        //printf("\n");
        printIndent(indent);
		printf("THEN");
        walkStatementList(pArithNode->pCentre);
        
        //printf("\n");
        printIndent(indent);
		printf("ELSE");
        walkStatementList(pArithNode->pRight);
        printf("\nENDIF");
        
        break;
    case (nodeStatement):
        if (fp != NULL) {
            sprintf(tmp, "%d %s Statement %d\n", indent, position, pArithNode->value);
            fwrite(tmp, 1, strlen(tmp), fp);
        }

		//printf(" Statement: index,%d", pArithNode->value);
        break;
	default:
		printf(" walkSyntaxTree: Unknown type:%d", pArithNode->type);
		break;
	}
	//walkSyntaxTree(pArithNode->pRight, "RIGHT", indent + 1);
	//printf("End pattern walk");
}

void walkStatementList(syntaxNode* pStatementListNode) {
#if REGRESS_1
    printf("\nwalkStatementList(syntaxNode* pStatementListNode=%d)\n", (int)pStatementListNode);
    fflush(stdout);
#endif /* REGRESS_1 */    
	if (pStatementListNode == NULL) {
		return;
	}
#if REGRESS_1    
    printf("\nwalkStatementList(pStatementListNode->pNext)=%d\n", (int)pStatementListNode->pNext);
    fflush(stdout);
#endif /* REGRESS_1 */    
    walkStatementList(pStatementListNode->pNext);
    
#if REGRESS_1    
    printf("walkSyntaxTree(pStatementListNode->pLeft=%d, ROOT, 0)\n", (int)pStatementListNode->pLeft);
    fflush(stdout);
#endif /* REGRESS_1 */    
	walkSyntaxTree(pStatementListNode->pLeft, "ROOT", 0);
}

void printIndent(unsigned int indent) {
    int i;
    printf("\n");
    for (i = indent; i > 0; --i) {
        printf("*");
    }
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
