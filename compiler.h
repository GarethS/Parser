/*************************************************
 *  Copyright (C) 2003 by  Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef compiler_h
#define compiler_h

#include <stdio.h>
//#include <assert.h>
#include "compilerHelper.h"

typedef enum {enumId, enumAnd, enumOr, enumAction} operandType;	// old stuff

typedef enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_XOR, OP_AND, OP_OR, OP_EQUALS, OP_TEST_FOR_EQUAL} operatorType;



#define EOS				'\0'	// End of string
#define VAR_ITEMS		(2000)  // Size of variable table
//#define ARG_ITEMS	    (2000)  // Size of function argument table
#define SYNTAX_ITEMS	(2000)
#define STATEMENT_ITEMS	(2000)
//#define VAR_MAX_INDEX	(VAR_ITEMS-1)
#define VAR_NAME_LENGTH	(12)
#define VAR_NOT_FOUND	(-1)
#define VAR_TABLE_LIMIT	(-2)	// Ran out of room in table

#define DEFAULT_VAR_VALUE   (0)

typedef struct thisVariableNode {
	char name[VAR_NAME_LENGTH];
	int val;        // Value of constant or variable. For functions, equals number of arguments.
    float valFloat; // for future use. Currently only ints allowed
    //unsigned int argumentCount; // Used by functions to ensure number of arguments matches definition
    nodeType type;
} varNode;

// OLD: This node can contain either a line id (e.g. a3) or an operator. Operators
//  will always have 2 operand nodes.
// Generic node structure that the syntax tree is made of.
typedef struct thisSyntaxNode {
#if 0
	// old stuff - meaning left over from first compiler done for Circuit Cellar
	operandType operand;	// &&, ||
	char   idValue[4];		// Only used when operand = enumId
#endif
    // new stuff
	nodeType type;
    // If nodeConst or nodeVar, this is the index into symbol table. If nodeOperator, this is the operator. If nodeIf, the unique IF-THEN-ELSE id.
	int	value;	
	
	struct thisSyntaxNode* pLeft;   // Overloaded by statement list to point to tree.
	struct thisSyntaxNode* pRight;
	struct thisSyntaxNode* pCentre;
	struct thisSyntaxNode* pNext;   // Used for walking each statement
} syntaxNode;

/*
typedef struct thisStatmentNode {
	struct thisSyntaxNode* pArith;
	struct thisStatmentNode* pNext;
} statementNode;
*/

// Function prototypes.
void yyerror (char* s);
int  yylex(void);
void printBoilerplate(void);

// The following 2 functions are only used by 'pattern' in the 
//  'pattern {action} part of the grammar.
syntaxNode* addNodeBinaryOperator(int operator, syntaxNode* pLeft, syntaxNode* pRight);
syntaxNode* addNodeArray(char* pVar, syntaxNode* pArrayIndex);
//syntaxNode* addNodeArrayConstIndex(char* pVar, int symbolTableIndex);
syntaxNode* addNodeVariableOperator(int operator, int varIndex, syntaxNode* pRight);
syntaxNode* addNodeSymbolIndex(int varIndex);
syntaxNode* addNodeIfOrWhile(syntaxNode* pExpr, syntaxNode* pIfOrWhileStatementList, syntaxNode* pElseStatementList, nodeType type);

syntaxNode* addStatement(syntaxNode* pStatementListNode, syntaxNode* pStatementNode);
syntaxNode* addArgument(syntaxNode* pArgumentListNode, syntaxNode* pArgumentNode);
syntaxNode* addDefnArgument(syntaxNode* pArgumentListNode, const char* pArgumentName);
syntaxNode* addFunction(const char* pFuncName, syntaxNode* pArgList, syntaxNode* pStatementList);
syntaxNode* addNodeFunctionCall(char* pFuncName, syntaxNode* pArgList);


// New functions 
int addVarToSymbolTable(char* var);
void buildVariable(const char* name, int value, varNode* varNode);
int addArrayToSymbolTable(char* varName, const unsigned int maxRange);

void initVarTable(void);
void initArgTable(void);
int insertVariable(varNode* pVar);
//int getVariableIndex(varNode* pVar);
//int setVariable(varNode* pVar);
int findVariable(varNode* pVar);
int findVariableByName(const char* pVarName);
int isConstant(varNode* pVar);

void initNode(syntaxNode* pSyntaxNode);
syntaxNode* getNextArithNode(void);
syntaxNode* getNextStatementNode(void);

void printIndent(const unsigned int indent);
void printOperator(const int value);
void walkSyntaxTree(syntaxNode* pSyntaxNode, char* position, int indent, FILE* fp);
void walkList(syntaxNode* pListNode, FILE* fp);   // Walk statements or argument lists
unsigned int countArguments(syntaxNode* pArgNode);
void dumpSymbolTable(const char* fileName);
void dumpSymbol(int i, FILE* fpSymbol);
//void freeNode(node* pNode);

#define debugAssert(x)  printf("\n" #x); fflush(stdout);

#endif /* compiler_h */