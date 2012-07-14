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
#define VAR_ITEMS		(2000)
#define SYNTAX_ITEMS	(2000)
#define STATEMENT_ITEMS	(2000)
//#define VAR_MAX_INDEX	(VAR_ITEMS-1)
#define VAR_NAME_LENGTH	(12)
#define VAR_NOT_FOUND	(-1)
#define VAR_TABLE_LIMIT	(-2)	// Ran out of room in table

#define DEFAULT_VAR_VALUE   (0)

typedef struct thisVariableNode {
	char name[VAR_NAME_LENGTH];
	int val;
    float valFloat;
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
	int	value;	// If nodeConst or nodeVar, this is the index into symbol table. If nodeOperator, this is the operator.
	
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
syntaxNode* addNodeIf(syntaxNode* pExpr, syntaxNode* pIfStatementList, syntaxNode* pElseStatementList);

syntaxNode* addStatement(syntaxNode* pStatementListNode, syntaxNode* pStatementNode);

// New functions 
int addVarToSymbolTable(char* var);
void buildVariable(char* name, int value, varNode* varNode);
int addArrayToSymbolTable(char* varName, const unsigned int maxRange);

void initVarTable(void);
int insertVariable(varNode* pVar);
//int getVariableIndex(varNode* pVar);
//int setVariable(varNode* pVar);
int findVariable(varNode* pVar);
int isConstant(varNode* pVar);

void initNode(syntaxNode* pArithNode);
syntaxNode* getNextArithNode(void);
syntaxNode* getNextStatementNode(void);

void printIndent(unsigned int indent);
void printOperator(int value);
void walkSyntaxTree(syntaxNode* pArithNode, char* position, int indent);
void walkStatementList(syntaxNode* pStatementListNode);
void dumpSymbolTable(void);
void dumpSymbol(int i);
//void freeNode(node* pNode);

#define debugAssert(x)  printf("\n" #x); fflush(stdout);

#endif /* compiler_h */