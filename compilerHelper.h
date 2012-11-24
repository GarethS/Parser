/*************************************************
 *  Copyright (C) 2012 by Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef _COMPILERHELPER_H_
#define _COMPILERHELPER_H_

#ifndef FALSE
#define FALSE (0)
#endif /* FALSE */
#ifndef TRUE
#define TRUE  (!FALSE)
#endif /* TRUE */

#define NOT_FOUND   (-1)

// nodeStartAction is the start of an action sequence in the program
typedef enum {nodeVariable,
                nodeConst,
                nodeOperator,
                nodeArray,
                nodeInvalid,
                nodeStartAction,
                nodeNOP,    // Skip past this node.
                nodeTemporary,
                nodeIf,
                nodeWhile,
                nodeStatement,
                nodeArgumentCall,       // Used by function call
                nodeArgumentValue,      // Used by function definition
                nodeArgumentReference,  // Used by function definition
                nodeFloat,
                nodeFunctionCall,
                nodeFunctionDefinition,
                nodeMotorMove,
                nodeMotorGetPosition
                } nodeType;
typedef enum {symbolVar, symbolConst, symbolArray, symbolInvalid, symbolTemporary, symbolFloat} symbolType;
typedef enum {nodeLeft, nodeRight, nodeRoot} nodePosition;

#endif /* _COMPILERHELPER_H_ */