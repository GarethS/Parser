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
#define INTRINSIC_FCN_DEFN_MOVE_ABSOLUTE        (-2)
#define INTRINSIC_FCN_DEFN_MOVE_RELATIVE        (-3)
#define INTRINSIC_FCN_DEFN_SLEEP                (-4)
#define INTRINSIC_FCN_DEFN_SLEEP_UNTIL          (-5)
#define INTRINSIC_FCN_DEFN_LED                  (-6)
#define INTRINSIC_FCN_DEFN_RPM                  (-7)    // 3 parameters
#define INTRINSIC_FCN_DEFN_RPM_x10k             (-8)    // 3
#define INTRINSIC_FCN_DEFN_ACCEL_MICROSEC       (-9)
#define INTRINSIC_FCN_DEFN_DEGREE_x10k_ABSOLUTE (-10)
#define INTRINSIC_FCN_DEFN_DEGREE_x10k_RELATIVE (-11)
#define INTRINSIC_FCN_DEFN_WAIT_FOR_IDLE        (-12)
#define INTRINSIC_FCN_DEFN_PRINT_NUMBER         (-13)
#define INTRINSIC_FCN_DEFN_GET_INPUT            (-14)
#define INTRINSIC_FCN_DEFN_SET_OUTPUT           (-15)
#define INTRINSIC_FCN_DEFN_GET_ADC              (-16)
#define INTRINSIC_FCN_DEFN_GET_TEMP             (-17)
#define INTRINSIC_FCN_DEFN_RESET                (-18)

#define RETURN_OK                               (0)
#define RETURN_ERR_TIMEOUT                      (1)
#define RETURN_NOT_IDLE                         (2)

#define NUM_PARAMETERS_ONE                      (1)
#define NUM_PARAMETERS_TWO                      (2)   // The above intrinsic functions mostly take 2 parameters.
#define NUM_PARAMETERS_THREE                    (3)

typedef enum {nodeVariable,             // 0
                nodeConst,
                nodeOperator,           // Typically a binary operator but can be used for unary operation (e.g. ~) by having the left-hand side NULL
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