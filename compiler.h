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
typedef enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_XOR, OP_AND, OP_OR} operatorType;



// This node can contain either a line id (e.g. a3) or an operator. Operators
//  will always have 2 operand nodes.
typedef struct thisNode {
	// old stuff
	operandType operand;	// &&, ||
	char   idValue[4];		// Only used when operand = enumId

    // new stuff
	nodeType type;
	int	value;	// If nodeConst, this is the value. If nodeVar, this is the index into symbol table. if nodeOperator, this is the operator.
	
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
node* addNodeId(char* id);

// The following 2 functions are only used by 'action' in the 
//  'pattern {action} part of the grammar.
node* addNodeOperatorAction(node* pNode, char* id);
node* addNodeActionId(char* id);

// New functions
node* getAvailNode(void);
node* addNodeVar(char* var);
int infixPatternTraversal(node* pn);


void doPatternAction(node* pPattern, node* pAction);
void walkPatternTree(node* pNode, char complement);
void walkActionTree(node* pNode, char complement);
void freeNode(node* pNode);

#endif /* compiler_h */