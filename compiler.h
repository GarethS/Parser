/*************************************************
 *  Copyright (C) 2003, 2011, 2012 by  Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef compiler_h
#define compiler_h

#include <stdio.h>
//#include <assert.h>
#include "compilerHelper.h"

//typedef enum {enumId, enumAnd, enumOr, enumAction} operandType;	// old stuff
//typedef enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_XOR, OP_AND, OP_OR, OP_EQUALS, OP_TEST_FOR_EQUAL} operatorType;

#define EOS				'\0'	// End of string
#define VAR_ITEMS		(2000)  // Size of variable table
#define SYNTAX_ITEMS	(20000)
#define STATEMENT_ITEMS	(20000)
//#define VAR_MAX_INDEX	(VAR_ITEMS-1)
#define VAR_NAME_LENGTH	(16)
#define VAR_NOT_FOUND	    (-1)
#define VAR_TABLE_LIMIT	    (-2)	// Ran out of room in table
//#define VAR_TABLE_INVALID	(-3)
#define VAR_PASS_BY_VALUE       (1)
#define VAR_PASS_BY_REFERENCE   (2)

#define VAR_FIRST_PARAMETER         (0)
#define VAR_SUBSEQUENT_PARAMETER    (1)

#define DEFAULT_VAR_VALUE   (0)

typedef struct thisSymbolNode {
	char name[VAR_NAME_LENGTH];
	int val;        // Value of constant or variable. For functions, equals number of arguments.
    float valFloat; // for future use. Currently only ints allowed
    //unsigned int argumentCount; // Used by functions to ensure number of arguments matches definition
    // If this is either nodeArgumentValue or nodeArgumentReference then it should be skipped over when searching for symbols.
    //  The reason for this is that 2 separate functions can have identical parameter names.
    nodeType type;
} symbolNode;

// Generic node structure the abstract syntax tree is made of.
typedef struct thisASTNode {
	nodeType type;
    // If nodeConst or nodeVariable, this is the index into symbol table. If nodeOperator, this is the operator. If nodeIf, the unique IF-THEN-ELSE id.
	int	value;	
	
	struct thisASTNode* pLeft;   // Overloaded by statement list to point to tree.
	struct thisASTNode* pRight;
	struct thisASTNode* pCentre;
	struct thisASTNode* pNext;   // Used for walking each statement. When used by symbol table, walks to next table.
} astNode;

// Function prototypes.
void yyerror (char* s);
int  yylex(void);
//void printBoilerplate(void);

astNode* addNodeBinaryOperator(int operator, astNode* pLeft, astNode* pRight);
astNode* addNodeArray(char* pVar, astNode* pASTNode);
astNode* addNodeVariableOperator(int operator, int varIndex, astNode* pRight);
astNode* addNodeSymbolIndex(int varIndex);
astNode* addNodeIfOrWhile(astNode* pExpr, astNode* pIfOrWhileStatementList, astNode* pElseStatementList, nodeType type);

astNode* addStatement(astNode* pStatementListNode, astNode* pStatementNode);
astNode* addFcnCallArgument(astNode* pArgumentListNode, astNode* pArgumentNode);
astNode* addFcnDefnArgument(astNode* pArgumentListNode, const char* pArgumentName, const int passByValueOrReference, const int firstOrSebsequentParameter);
astNode* addFunction(const char* pFuncName, astNode* pArgList, astNode* pStatementList);
astNode* addNodeFunctionCall(char* pFuncName, astNode* pArgList);
astNode* addNodeInstrinsicFunction1(char* functionName, char* returnValue, astNode* parameter1);

// New functions 
int addVarToSymbolTable(char* var);
void buildSymbol(const char* name, int value, symbolNode* varNode);
int addArrayToSymbolTable(char* varName, const unsigned int maxRange);

void initVarTable(void);
void initArgTable(void);
int insertSymbol(symbolNode* pVar);
void insertSymbolAtIndex(symbolNode* pVar, const unsigned int index);
//int getVariableIndex(symbolNode* pVar);
//int setVariable(symbolNode* pVar);
int findSymbol(symbolNode* pVar);
int findSymbolFcnDefinition(symbolNode* pVar);
int findSymbolByName(const char* pVarName);
int isConstant(symbolNode* pVar);

void initNode(astNode* pSyntaxNode);
astNode* getNextASTNode(void);
astNode* getNextStatementNode(void);
//int getSymbolTableIndex(symbolNode* pSymbol);

void printIndent(const unsigned int indent);
void printOperator(const int value);
void walkSyntaxTree(astNode* pSyntaxNode, char* position, int indent, FILE* fp);
void walkList(astNode* pListNode, FILE* fp);   // Walk statements or argument lists
unsigned int countArguments(astNode* pArgNode);
void dumpSymbolTable(const char* fileName);
void dumpSymbol(int i, FILE* fpSymbol);
//void freeNode(node* pNode);
void checkFcnSanity(void);

#define debugAssert(x)  printf("\n" #x); fflush(stdout);

#endif /* compiler_h */