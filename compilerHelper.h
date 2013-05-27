/*************************************************
 *  Copyright (C) 2011, 2012, 2013 by Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef _COMPILERHELPER_H_
#define _COMPILERHELPER_H_

// Function prototypes.
void yyerror (char* s);
int  yylex(void);

#ifndef FALSE
#define FALSE (0)
#endif /* FALSE */
#ifndef TRUE
#define TRUE  (!FALSE)
#endif /* TRUE */

#define NOT_FOUND       (-1)
#define INVALID_VALUE   (-1)

#define SYSTEM_VERSION_PREFIX                   "<VERS>"    // Used to note start of interpreter download
#define SYSTEM_VERSION_SUFFIX                   "<vers>"
#define SYSTEM_INTERPRET_DONE                   "<DONE>"    // Time to run the interpreter

#define STATEMENT_PREFIX                        "<STMT>"
#define STATEMENT_PREFIX_PLUS_SPACE             STATEMENT_PREFIX" "
#define STATEMENT_SUFFIX                        "<stmt>"
#define STATEMENT_SUFFIX_PLUS_SPACE_AND_EOL     " "STATEMENT_SUFFIX"\n" 
#define STATEMENT_ACKNOWLEDGE                   "<ASTM>"    // After each statement there's an acknowledge before next statement is sent

#define SYMBOL_PREFIX                           "<SYMB>"
#define SYMBOL_PREFIX_PLUS_SPACE                SYMBOL_PREFIX" "
#define SYMBOL_SUFFIX                           "<symb>"
#define SYMBOL_SUFFIX_PLUS_SPACE_AND_EOL        " "SYMBOL_SUFFIX"\n" 
#define SYMBOL_ACKNOWLEDGE                      "<ASYM>"

#define VAR_FCN_LINK_UNDEFINED              (-1)
// Used by interpreter to call intrinsic (built-in) functions. All intrinsics
//  use a negative number to distinguish them from user defined functions.
#define INTRINSIC_FCN_DEFN_MOVE_ABSOLUTE    (-2)
#define INTRINSIC_FCN_DEFN_MOVE_RELATIVE    (-3)
#define INTRINSIC_FCN_DEFN_SLEEP            (-4)
#define INTRINSIC_FCN_DEFN_SLEEP_UNTIL      (-5)
#define INTRINSIC_FCN_DEFN_LED              (-6)
#define NUM_PARAMETERS_TWO                  (2)   // The above intrinsic functions all take 2 parameters.

typedef enum {nodeVariable,             // 0
                nodeConst,
                nodeOperator,
                nodeArray,
                nodeInvalid,
                nodeStartAction,        // 5
                nodeNOP,                // Skip past this node.
                nodeTemporary,
                nodeIf,
                nodeWhile,
                nodeStatement,          // 10
                nodeArgumentCall,       // Used by function call
                nodeArgumentValue,      // Used by function definition
                nodeArgumentReference,  // Used by function definition
                nodeFloat,
                nodeFunctionCall,       // 15   Better name would be nodeFunctionCallStart to match nodeFunctionCallEnd below
                nodeFunctionDefinition,
                nodeMotorMove,
                nodeMotorGetPosition,
                // Nodes below here only used by interpreter
                nodeIfEval0,
                nodeElse,               // 20
                nodeEndif,
                nodeEndWhile,
                nodeDo,
                nodeJmpEndIf,
                nodeBasePointer,        // 25
                nodeFunctionCallEnd,    // Function call parameters are finished, time to actually call the function
                nodeProgramEnd,
                nodeFunctionReturn,
                nodeWhileEval0,
                nodeAvailable           // 30, After being used, this node is now available
                } nodeType;
                
//typedef enum {symbolVar, symbolConst, symbolArray, symbolInvalid, symbolTemporary, symbolFloat} symbolType;
typedef enum {nodeLeft, nodeRight, nodeRoot} nodePosition;

// Below here used by motor/embedded parser
typedef enum {nodeEmbeddedUnknown, nodeEmbeddedVersion, nodeEmbeddedStatement, nodeEmbeddedSymbol, nodeEmbeddedProgramEnd} nodeEmbeddedtype;

#endif /* _COMPILERHELPER_H_ */