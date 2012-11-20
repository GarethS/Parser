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
#define SYNTAX_ITEMS	(20000)
#define STATEMENT_ITEMS	(20000)
//#define VAR_MAX_INDEX	(VAR_ITEMS-1)
#define VAR_NAME_LENGTH	(12)
#define VAR_NOT_FOUND	(-1)
#define VAR_TABLE_LIMIT	(-2)	// Ran out of room in table
#define VAR_PASS_BY_VALUE       (1)
#define VAR_PASS_BY_REFERENCE   (2)

#define DEFAULT_VAR_VALUE   (0)

typedef struct thisVariableNode {
	char name[VAR_NAME_LENGTH];
	int val;        // Value of constant or variable. For functions, equals number of arguments.
    float valFloat; // for future use. Currently only ints allowed
    //unsigned int argumentCount; // Used by functions to ensure number of arguments matches definition
    // If this is either nodeArgumentValue or nodeArgumentReference then it should be skipped over when searching for symbols.
    //  The reason for this is that 2 separate functions can have identical parameter names.
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
	nodeType type;
    // If nodeConst or nodeVar, this is the index into symbol table. If nodeOperator, this is the operator. If nodeIf, the unique IF-THEN-ELSE id.
	int	value;	
	
	struct thisSyntaxNode* pLeft;   // Overloaded by statement list to point to tree.
	struct thisSyntaxNode* pRight;
	struct thisSyntaxNode* pCentre;
	struct thisSyntaxNode* pNext;   // Used for walking each statement. When used by symbol table, walks to next table.
    varNode* pVarNode;
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
syntaxNode* addNodeArray(char* pVar, syntaxNode* pASTNode);
//syntaxNode* addNodeArrayConstIndex(char* pVar, int symbolTableIndex);
syntaxNode* addNodeVariableOperator(int operator, int varIndex, syntaxNode* pRight);
syntaxNode* addNodeSymbolIndex(int varIndex);
syntaxNode* addNodeSymbolIndexNew(syntaxNode* pVar);
syntaxNode* addNodeIfOrWhile(syntaxNode* pExpr, syntaxNode* pIfOrWhileStatementList, syntaxNode* pElseStatementList, nodeType type);

syntaxNode* addStatement(syntaxNode* pStatementListNode, syntaxNode* pStatementNode);
syntaxNode* addFcnCallArgument(syntaxNode* pArgumentListNode, syntaxNode* pArgumentNode);
syntaxNode* addFcnDefnArgument(syntaxNode* pArgumentListNode, const char* pArgumentName, const int passByValueOrReference);
syntaxNode* addFunction(const char* pFuncName, syntaxNode* pArgList, syntaxNode* pStatementList);
syntaxNode* addNodeFunctionCall(char* pFuncName, syntaxNode* pArgList);


// New functions 
int addVarToSymbolTable(char* var);
syntaxNode* addVarToSymbolTableNew(syntaxNode* pVarTable, char* var);
void buildVariable(const char* name, int value, varNode* varNode);
int addArrayToSymbolTable(char* varName, const unsigned int maxRange);
syntaxNode* addArrayToSymbolTableNew(syntaxNode* pVarTable, char* varName, const unsigned int maxRange);

void initVarTable(void);
void initArgTable(void);
int insertVariable(varNode* pVar);
//int getVariableIndex(varNode* pVar);
//int setVariable(varNode* pVar);
int findVariable(varNode* pVar);
syntaxNode* findVariableNew(syntaxNode* pVarTable, varNode* pVar);
int findVariableByName(const char* pVarName);
int isConstant(varNode* pVar);

void initNode(syntaxNode* pSyntaxNode);
syntaxNode* getNextASTNode(void);
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