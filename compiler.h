/*************************************************
 *  Copyright (C) 2003 by  Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef compiler_h
#define compiler_h

#include <stdio.h>
//#include <assert.h>

#define FALSE (0)
#define TRUE  (!FALSE)

typedef enum {enumId, enumAnd, enumOr, enumAction} operandType;	// old stuff

typedef enum {nodeVar, nodeConst, nodeOperator} nodeType;
typedef enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_XOR, OP_AND, OP_OR, OP_EQUALS, OP_TEST_FOR_EQUAL} operatorType;



#define EOS				'\0'	// End of string
#define VAR_ITEMS		(200)
#define ARITH_ITEMS		(200)
#define ACTION_ITEMS	(200)
//#define VAR_MAX_INDEX	(VAR_ITEMS-1)
#define VAR_NAME_LENGTH	(12)
#define VAR_NOT_FOUND	(-1)
#define VAR_TABLE_LIMIT	(-2)	// Ran out of room in table

typedef struct thisVariableNode {
	char name[VAR_NAME_LENGTH];
	int val;
} varNode;

// OLD: This node can contain either a line id (e.g. a3) or an operator. Operators
//  will always have 2 operand nodes.
// Generic node structure that the syntax tree is made of.
typedef struct thisArithmeticNode {
	// old stuff
	operandType operand;	// &&, ||
	char   idValue[4];		// Only used when operand = enumId

    // new stuff
	nodeType type;
	int	value;	// If nodeConst, this is the value. If nodeVar, this is the index into symbol table. If nodeOperator, this is the operator.
	
	struct thisArithmeticNode* pLeft;
	struct thisArithmeticNode* pRight;
} arithNode;

typedef struct thisActionNode {
	struct thisArithmeticNode* pArith;
	struct thisActionNode* pNext;
} actionNode;


// Function prototypes.
void yyerror (char* s);
int  yylex(void);
void pushInputLine(char* line);
void pushOutputLine(char* line);
void printBoilerplate(void);

// The following 2 functions are only used by 'pattern' in the 
//  'pattern {action} part of the grammar.
arithNode* addNodeOperator(int operator, arithNode* pLeft, arithNode* pRight);
arithNode* addNodeVarOperand(int operator, int varIndex, arithNode* pRight);
arithNode* addNodeId(int varIndex);

//arithNode* addNodeOperatorAction(arithNode* pNode, char* id);
actionNode* addNodeOperatorAction(actionNode* pActionNode, arithNode* pArithNode);
arithNode* addNodeActionId(char* id);

// New functions 
arithNode* getAvailNode(void);
int addNodeVar(char* var);
int infixPatternTraversal(arithNode* pn);

void initVarTable(void);
int insertVariable(varNode* pVar);
//int getVariableIndex(varNode* pVar);
//int setVariable(varNode* pVar);
int findVariable(varNode* pVar);
int isConstant(varNode* pVar);

arithNode* getNextArithNode(void);
actionNode* getNextActionNode(void);

void doPatternAction(arithNode* pPattern, arithNode* pAction);
void walkPatternTree(arithNode* pNode, char* position);
void walkActionTree(arithNode* pNode);
void dumpSymbolTable(void);
void dumpSymbol(int i);
//void freeNode(node* pNode);

#endif /* compiler_h */