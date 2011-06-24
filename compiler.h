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
#define VAR_ITEMS		(1000)
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
typedef struct thisNode {
	// old stuff
	operandType operand;	// &&, ||
	char   idValue[4];		// Only used when operand = enumId

    // new stuff
	nodeType type;
	int	value;	// If nodeConst, this is the value. If nodeVar, this is the index into symbol table. If nodeOperator, this is the operatorType.
	
	struct thisNode* pLeft;
	struct thisNode* pRight;
} node;


// Function prototypes.
void yyerror (char* s);
int  yylex(void);
void pushInputLine(char* line);
void pushOutputLine(char* line);
void printBoilerplate(void);

// The following 2 functions are only used by 'pattern' in the 
//  'pattern {action} part of the grammar.
node* addNodeOperator(int type, node* pLeft, node* pRight);
node* addNodeId(int varIndex);

// The following 2 functions are only used by 'action' in the 
//  'pattern {action} part of the grammar.
node* addNodeOperatorAction(node* pNode, char* id);
node* addNodeActionId(char* id);

// New functions
node* getAvailNode(void);
int addNodeVar(char* var);
int infixPatternTraversal(node* pn);

void initVarTable(varNode* pvn, unsigned int len);
int insertVariable(varNode* pTable, varNode* pVar);
int findVariable(varNode* pTable, varNode* pVar);


void doPatternAction(node* pPattern, node* pAction);
void walkPatternTree(node* pNode, char complement);
void walkActionTree(node* pNode, char complement);
void freeNode(node* pNode);

#endif /* compiler_h */