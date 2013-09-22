/*
	Copyright (c) Gareth Scott 2012, 2013

	interpret.cpp 

*/

#include <stdbool.h>
#include <stdint.h>
#include "interpret.h"
#include "compilerHelper.h"
#if CYGWIN
//#include <iostream>
//#include <fstream>
#include <string>
#include <istream>
#include <sstream>
#else /* not CYGWIN */

#include "stdio.h"
#include "string.h"
#include "../motor/LED.h"
#include "FreeRTOS.h"

bool led::_enable = FALSE;

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern "C" {

void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void interpretRun(void);

//FreeRTOS specific below
static portTickType xTaskWakeTime;
void vTaskDelay( portTickType xTicksToDelay ) PRIVILEGED_FUNCTION;  // task.h
void vTaskDelayUntil( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement ) PRIVILEGED_FUNCTION; // task.h
portTickType xTaskGetTickCount( void ) PRIVILEGED_FUNCTION; // task.h
}   // extern "C"

interpret interpreter;
static bool interpreterRunBool = FALSE;
extern stepper* sp;

void interpretRun(void) {
    for (;;) {
        if (interpreterRunBool) {
            sp = interpreter.getStepper();
            led::enable(1);    
            interpreter.run();
            interpreterRunBool = FALSE;  // Send another <PRUN> command to start again.
        } else {
            // interpreter.stop();
            vTaskDelay(100 / portTICK_RATE_MS);
        }
    }
}

void successMsg(void) {
    static const char* msg = "<OK>";
    UARTSend((unsigned char *)msg, strlen(msg));
}

void errMsg(void) {
    static const char* msg = "<ERR>";
    UARTSend((unsigned char *)msg, strlen(msg));
}

void postMsg(bool success) {
    //led blink;
    if (success) {
        successMsg();
    } else {
        errMsg();
        //led blink;  // Second blink to show an error.
    }
    //vTaskDelay(100 / portTICK_RATE_MS); // So we can see the LED
}

nodePosition stringToNodePosition(const char* pc) {
    if (strcmp(pc, "LEFT") == 0) {
        return nodeLeft;
    } else if (strcmp(pc, "RIGHT") == 0) {
        return nodeRight;
    } 
    return nodeRoot;    // TODO - should have a nodeUnknown
}

// Similar to _loadParseTreeEntry()
nodeType stringToNodeType(const char* pc) {
    if (strcmp(pc, "Variable") == 0) {
        return nodeVariable;
    } else if (strcmp(pc, "Operator") == 0) {
        return nodeOperator;
    } else if (strcmp(pc, "Action") == 0) {
        // TODO - Is this still needed?
        return nodeStartAction;
    } else if (strcmp(pc, "If") == 0) {
        return nodeIf;
    } else if (strcmp(pc, "EVAL0") == 0) {
        return nodeIfEval0;
    } else if (strcmp(pc, "EVALWHILE0") == 0) {
        return nodeWhileEval0;
    } else if (strcmp(pc, "Else") == 0) {
        return nodeElse;
    } else if (strcmp(pc, "EndIf") == 0) {
        return nodeEndif;
    } else if (strcmp(pc, "While") == 0) {
        return nodeWhile;
    } else if (strcmp(pc, "EndWhile") == 0) {
        return nodeEndWhile;
    } else if (strcmp(pc, "FunctionCall") == 0) {
        return nodeFunctionCall;
    } else if (strcmp(pc, "FunctionCallEnd") == 0) {
        return nodeFunctionCallEnd;
    } else if (strcmp(pc, "JmpEndIf") == 0) {
        return nodeJmpEndIf;
    } else if (strcmp(pc, "Start") == 0) {
        return nodeFunctionDefinition;
    } else if (strcmp(pc, "ProgramEnd") == 0) {
        return nodeProgramEnd;
    } else {
        return nodeInvalid; // TODO - flag this as an error
    }
}

nodeType intToNodeType(const int nt) {
    switch (nt) {
    case nodeVariable:
        return nodeVariable;
    case nodeConst:
        return nodeConst;
    case nodeOperator:
        return nodeOperator;
    case nodeTemporary:
        return nodeTemporary;
    case nodeFunctionCall:
        return nodeFunctionCall;
    case nodeFunctionDefinition:
        return nodeFunctionDefinition;
    case nodeArgumentValue:
        return nodeArgumentValue;
    case nodeArgumentReference:
        return nodeArgumentReference;
    case nodeBasePointer:
        return nodeBasePointer;
    case nodeFunctionReturn:
        return nodeFunctionReturn;
    case nodeAvailable:
        return nodeAvailable;
    case nodeInvalid:
    default:
        return nodeInvalid;
    }
}

#define EOT                         ('$')   // End-of-transmission
#define EOS                         ('\0')  // End-of-string
#define STMT_STR_LEN                (32)
static int stmtNestingLevel;
static char stmtPosition[STMT_STR_LEN];
static char stmtType[STMT_STR_LEN];
static int stmtValue;

static int symbType;
static int symbValue;
static int symbFcnLink;

void bufferInput(unsigned char c) {
    // TODO - make sure _back really starts at 0 or yy_scan_bytes won't work
    static tinyQueue<unsigned char> serialInput(0);

    static const char statementBegin[] = "<STMT>";
    static const char statementEnd[] = "<stmt>";

    static const char symbolBegin[] = "<SYMB>";
    static const char symbolEnd[] = "<symb>";
    
    static const char programRun[] = "<PRUN>";

    if (c != EOT) {
        // We're called from an interrupt so get out quickly!
        serialInput.push(c);
        return;
    }
    serialInput.push(EOS);
    if (serialInput.spaceAvail() == 0) {
        // TODO - Post error back
        serialInput.clear();
        return;
    }
    const char* pStatementBegin = strstr((const char*)serialInput.getBuffer(), (const char*)statementBegin);
    const char* pStatementEnd = strstr((const char*)serialInput.getBuffer(), (const char*)statementEnd);
    if (pStatementBegin != NULL && pStatementEnd != NULL) {
        // Got a valid statement. Typical statement: <STMT> 0 0 Start 0 <stmt>
        int scannedItems = sscanf(pStatementBegin + strlen(statementBegin), "%d %s %s %d", &stmtNestingLevel, stmtPosition, stmtType, &stmtValue);
        if (scannedItems == 4) {
            parseTreeEntry pte;
            pte.level(stmtNestingLevel);
            pte.position(stringToNodePosition(stmtPosition));
            pte.value(stmtValue);
            pte.type(stringToNodeType(stmtType));
            bool atpSuccess = interpreter.appendToProgram(pte);
            serialInput.clear();
            postMsg(atpSuccess);
            return;
        }
    }
    const char* pSymbolBegin = strstr((const char*)serialInput.getBuffer(), (const char*)symbolBegin);
    const char* pSymbolEnd = strstr((const char*)serialInput.getBuffer(), (const char*)symbolEnd);
    if (pSymbolBegin != NULL && pSymbolEnd != NULL) {
        // Got a valid symbol. Typical symbol: <SYMB> 16 0 0 <symb>
        int scannedItems = sscanf(pSymbolBegin + strlen(symbolBegin), "%d %d %d", &symbType, &symbValue, &symbFcnLink);
        if (scannedItems == 3) {
            symbolTableEntry ste;
            ste.type(intToNodeType(symbType));
            ste.value(symbValue);
            ste.fcnLink(symbFcnLink);
            bool atstSuccess = interpreter.appendToSymbolTable(ste);
            serialInput.clear();
            postMsg(atstSuccess);            
            return;
        }
    }
    const char* pProgramRun = strstr((const char*)serialInput.getBuffer(), (const char*)programRun);
    if (pProgramRun != NULL) {
        interpreter.setLowWaterMarkForTemporarySymbolSearch();
        serialInput.clear();
        postMsg(TRUE);
        interpreterRunBool = TRUE;
        return;
    }
}
#endif /* CYGWIN */

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRETER")),
#else /* not CYGWIN */
                    _firstIntrinsicFcnDefnSleepUntil(true),
#endif /* CYGWIN */		
                    _programIndex(PROGRAM_INDEX_START), _symbolTableIndex(0), _bp(PROGRAM_INDEX_START), _evaluatingPattern(true) {
#if 0                    
    for (int i = 0; i < MAX_PROGRAM_ENTRY; ++i) {
        //printf("%d %d\n", i, _program[i].type());
        oss() << i << " " << _program[i].type();
        dump();
        //_program[i].type(nodeInvalid);
    }
#endif
#if CYGWIN
    printf("Version: %s\n", VERSION_STRING);
#endif /* CYGWIN */
    //_s.RPM(1, 24);    
}

void interpret::load(void) {
#if CYGWIN
    _loadTree(string("tree.txt"));
    _loadSymbolTable("symbolTable.txt");
#endif // CYGWIN    
}

bool interpret::appendToProgram(const parseTreeEntry& pte) {
    _program[_programIndex++] = pte;
    if (_programIndex >= MAX_PROGRAM_ENTRY) {
#if CYGWIN        
        oss() << "Out of program memory. Aborting.";
        dump();
#endif /* CYGWIN */        
        return false;
    }
    //oss() << level << " " << leftRight << " " << variableOperator << " " << value;
    //dump();
    return true;
}

bool interpret::appendToSymbolTable(const symbolTableEntry& ste) {
    _symbolTable[_symbolTableIndex++] = ste;
    if (_symbolTableIndex >= MAX_SYMBOL_TABLE_ENTRY) {
#if CYGWIN        
        oss() << "Exceeded symbol table size. Aborting.";
        dump();
#endif /* CYGWIN */        
        return false;
    }
    //oss() << level << " " << leftRight << " " << variableOperator << " " << value;
    //dump();
    return true;
}

#if CYGWIN
bool interpret::_loadParseTreeEntry(const string& inputString) {
    istringstream iss(inputString);
    string prefix;
    unsigned int level;
    string leftRight;
    string variableOperator;
    int value;
    string suffix;
    iss >> prefix >> level >> leftRight >> variableOperator >> value >> suffix;
    if (prefix != STATEMENT_PREFIX) {
        //assert(prefix == STATEMENT_PREFIX);
#if CYGWIN        
        oss() << endl << "ERROR:STATEMENT_PREFIX:'" << prefix << "'";
#endif /* CYGWIN */
        return false;
    }
    if (suffix != STATEMENT_SUFFIX) {
        //assert(suffix == STATEMENT_SUFFIX);
#if CYGWIN        
        oss() << endl << "ERROR:STATEMENT_SUFFIX:'" << suffix << "'";
#endif /* CYGWIN */
        return false;
    }
    parseTreeEntry pte;
    pte.level(level);
    pte.value(value);
    // Note that these need to be matched in interpret::run()
    if (variableOperator == "Variable") {
        pte.type(nodeVariable);
    } else if (variableOperator == "Operator") {
        pte.type(nodeOperator);
    } else if (variableOperator == "Action") {
        pte.type(nodeStartAction);
    } else if (variableOperator == "If") {
        pte.type(nodeIf);
    } else if (variableOperator == "EVAL0") {
        pte.type(nodeIfEval0);
    } else if (variableOperator == "EVALWHILE0") {
        pte.type(nodeWhileEval0);
    } else if (variableOperator == "Else") {
        pte.type(nodeElse);
    } else if (variableOperator == "EndIf") {
        pte.type(nodeEndif);
    } else if (variableOperator == "While") {
        pte.type(nodeWhile);
    } else if (variableOperator == "EndWhile") {
        pte.type(nodeEndWhile);
    } else if (variableOperator == "FunctionCall") {
        pte.type(nodeFunctionCall);
    } else if (variableOperator == "FunctionCallEnd") {
        pte.type(nodeFunctionCallEnd);
    //} else if (variableOperator == "Do") {
    //    pte.type(nodeDo);
    } else if (variableOperator == "JmpEndIf") {
        pte.type(nodeJmpEndIf);
    } else if (variableOperator == "Start") {
        pte.type(nodeFunctionDefinition);
    } else if (variableOperator == "ProgramEnd") {
        pte.type(nodeProgramEnd);
    } else {
#if CYGWIN        
        oss() << endl << "ERROR:unrecognized variableOperator:" << variableOperator;
#endif /* CYGWIN */        
    }
    return appendToProgram(pte);
}

bool interpret::_loadSymbolTableEntry(const string& inputString) {
    istringstream iss(inputString);
    string prefix;
    unsigned int type;
    int value;
    int fcnLink;
    string suffix;
    iss >> prefix >> type >> value >> fcnLink >> suffix;
    if (prefix != SYMBOL_PREFIX) {
        //assert(prefix == SYMBOL_PREFIX);
#if CYGWIN        
        oss() << endl << "ERROR:SYMBOL_PREFIX:'" << prefix << "'";
#endif /* CYGWIN */
        return false;
    }
    if (suffix != SYMBOL_SUFFIX) {
        //assert(suffix == SYMBOL_SUFFIX);
#if CYGWIN        
        oss() << endl << "ERROR:SYMBOL_SUFFIX:'" << suffix << "'";
#endif /* CYGWIN */
        return false;
    }
    symbolTableEntry ste((nodeType)type, value, fcnLink);
    return appendToSymbolTable(ste);
}

void interpret::_loadTree(const string& s) {
    ifstream ifs(s.c_str());
    if (!ifs) {
        oss() << "error opening: " << s;
        dump();
        return;
    }
    oss() << "interpret::" << s;
    dump();
    // Typical line reads: '3 LEFT Variable 0'
    while (!ifs.eof()) {
        string inputString;
        //ifs.getline(inputString);
        getline(ifs, inputString);
        //cout << "IFS:" << inputString << endl;
        if (inputString == "" || !_loadParseTreeEntry(inputString)) {
            //cout << "Got endln" << endl;
            break;
        }
    }
    ifs.close();
}

// Temporary symbols (e.g. the evaluation of an expression like 'x < 5' are
//  stored just above the symbols loaded here. The value of _symbolTableIndex
//  when this function exits will contain the dividing line between permanent
//  and temporaries.
void interpret::_loadSymbolTable(const string& s) {
    ifstream ifs(s.c_str());
    if (!ifs) {
        oss() << "error opening: " << s;
        dump();
        return;
    }
    oss() << "interpret::" << s;
    dump();
    // Typical line to read: '1 4 -1', where 1 = nodeType, 4 = value and -1 is funcion link. In this case it's a constant with value = 4 and no function link.
    //  Note that function linking is only needed by function calls and function definitions.
    while (!ifs.eof()) {
        string inputString;
        //ifs.getline(inputString);
        getline(ifs, inputString);
        //cout << "IFS:" << inputString << endl;
        if (inputString == "" || !_loadSymbolTableEntry(inputString)) {
            //cout << "Got endln" << endl;
            break;
        }
    }
    setLowWaterMarkForTemporarySymbolSearch();
    ifs.close();
}

void interpret::dumpProgram(void) {
    oss() << "interpret::dumpProgram";
    dump();
    for (int i = 0; _program[i].type() != nodeInvalid; ++i) {
        _program[i].dumpEntry();
    }
}

void interpret::dumpSymbolTable(void) {
    oss() << "interpret::dumpSymbolTable";
    dump();
    for (unsigned int i = 0; i < _symbolTableIndex; ++i) {
        oss() << i << " ";
        _symbolTable[i].dumpEntry();
    }
}

void interpret::dumpEvaluationStack(void) {
    oss() << "ENTER interpret::dumpEvaluationStack";
    dump();
    // 1. Save current eval stack
    tinyQueue<int> _evaluationCopy;
    _evaluationCopy = _evaluationStack;
    // 2. Dump contents of the copy. Using a copy because content gets destroyed when an item is poped off stack.
    while (!_evaluationCopy.empty()) {
        oss() << "Symbol index=" << _evaluationCopy.front() << " value=" << _symbolTable[_evaluationCopy.front()].value() << " stackFrameIndex():" << _evaluationCopy.stackFrameIndex();
        dump();
        _evaluationCopy.pop_front();
    }
    oss() << "LEAVE interpret::dumpEvaluationStack";
    dump();
}
#endif /* CYGWIN */

void interpret::run(void) {
    assert(_evaluationStack.empty());
    int localSymbolTableIndex;
#if CYGWIN
#define CYGWIN_MAX_INFINITE_LOOP    (1000)
    static int infiniteLoopCounter = 0;
#endif /* CYGWIN */    
    for (_programIndex = 0; _program[_programIndex].type() != nodeInvalid; ++_programIndex) {
#if CYGWIN
        if (++infiniteLoopCounter > CYGWIN_MAX_INFINITE_LOOP) {
            break;
        }
        oss() << endl << "Run programIndex=" << _programIndex << " programNodeType:" << _currentProgramNodeType() << " programNodeValue:" << _currentProgramNodeValue();
        dump();
        //dumpEvaluationStack();
#endif /* CYGWIN */
        switch (_currentProgramNodeType()) {
        case nodeVariable:
        case nodeConst:
            //printf("nodeVariable or nodeConst\n");
            _evaluationStack.push_front(_currentProgramNodeValue());
            //_shortCircuitOptimization();
            break;
        case nodeOperator:
            //printf("nodeOperator\n");
            evaluateOperator(_currentProgramNodeValue());
            //_shortCircuitOptimization();
            break;
#if 0            
        case nodeOperatorUnary:
            evaluateUnaryOperator(_currentProgramNodeValue());
            break;
#endif            
#if 0            
        case nodeStartAction:
            // This now has a different meaning. It's both the start of a function and
            //  if you're in a function and hit this, you've reached the end of your function
            //  and need to reset the stack frame.
            // Look at value on top of stack and see if action should be executed.
#if CYGWIN
                oss() << endl << "nodeStartAction _evalValuePeek:" << _evalValuePeek();
                dump();
#endif /* CYGWIN */                
            if (!_symbolTable[_evalValue()].value()) {
                // If value on top of stack == 0 then don't need to perform action statements
                assert(_evaluationStack.empty());
                //_programIndex = -1;
                //_evaluatingPattern = false; // Now evaluating action part
#if CYGWIN
                dumpEvaluationStack();
#endif /* CYGWIN */    
                _resetSymbolTableTemporaryBoundary();
                return;
                //break;
            } else {
                _evaluatingPattern = false; // Now evaluating action part
            }
            break;
#endif            
        case nodeFunctionDefinition:
#if CYGWIN        
            oss() << endl << "***nodeFunctionDefinition _programIndex:" << _programIndex ;
            dump();
#endif /* CYGWIN */            
            if (_programIndex == PROGRAM_INDEX_START) {
                // Entering main
                //++_programIndex;
            } else {
                // End of function, time to return from whence we came. In other words, hitting a function definition means we've reached end of current function.
                {
                    // TODO: This is the time to return any nodeTemporary symbols on the _evaluationStack to being marked as nodeUnused
                    // TODO: Time to return nodeFunctionReturn and nodeBasePointer to nodeAvailable
                    unsigned int nodeFunctionReturnIndex = _evaluationStack.peekAtIndex(_bp+1); 
                    //oss() << endl << "!!!nodeFunctionReturnIndex:" << nodeFunctionReturnIndex;
                    //dump();
                    _programIndex = _symbolTable[nodeFunctionReturnIndex].value();
                    
                    _evaluationStack.stackFrameIndex(_bp);   // mov sp, bp
                    unsigned int bpIndex = _evaluationStack.front();
                    _bp = _symbolTable[bpIndex].value();
                    _cleanUpEvaluationStack(2);   // clean up base pointer index, then function return index
                }
            }
            break;
        case nodeWhileEval0:
            //_programIndex = _currentProgramNodeValue();
            //int ifEval0Value = _evalValuePeek();
            localSymbolTableIndex = _evalValue();
            _returnSymbolToAvailablePool(localSymbolTableIndex);
            if (_symbolTable[localSymbolTableIndex].value()) {
                // TRUE on evaluation stack so just carry on with the next statement
            } else {
                // FALSE on evaluation stack
#if CYGWIN
                //oss() << endl << "!!nodeWhileEval0 _currentProgramNodeValue():" << _currentProgramNodeValue() << " _currentProgramNodeLevel()" << _currentProgramNodeLevel() ;
                //dump();
#endif /* CYGWIN */ 
                _updateProgramIndex(nodeEndWhile, _programIndex);
            }
            break;
        case nodeIfEval0:
            // Result of nodeIf now sits on top of evaluation stack. If it's 0
            //  jump to nodeElse of that same index.
            // Get value from top of _evaluationStack
            
            localSymbolTableIndex = _evalValue();
            _returnSymbolToAvailablePool(localSymbolTableIndex);
            if (_symbolTable[localSymbolTableIndex].value()) {
                //++_programIndex;    // TODO: Is this correct? Don't need to do it for While statement. Looks suspicious!
            } else {
#if CYGWIN
                //oss() << endl << "!!nodeIfEval0 _currentProgramNodeValue():" << _currentProgramNodeValue() << " _currentProgramNodeLevel()" << _currentProgramNodeLevel() ;
                //dump();
#endif /* CYGWIN */  
                _updateProgramIndex(nodeElse, _programIndex);
            }
#if CYGWIN
            //oss() << endl << "!!nodeIfEval0 _programIndex:" << _programIndex;
            //dump();
#endif /* CYGWIN */                
            break;
        case nodeJmpEndIf:
            {
                _updateProgramIndex(nodeEndif, _programIndex);
            }
            break;
        case nodeEndWhile:
            {
                // At end of while loop and want to jump back to the beginning
                _updateProgramIndex(nodeWhile, 0);
            }
            break;
        case nodeFunctionCall:
            // 1. push bp
            {
                symbolTableEntry symbolReturnValue(nodeFunctionReturn, INVALID_VALUE);  // Filled in later at nodeFunctionCallEnd when all parameters are on the stack
                _symbolTable[_symbolTableIndex] = symbolReturnValue;
                _evaluationStack.push_front(_symbolTableIndex++);   // push function return value
                
                symbolTableEntry symbolBasePointer(nodeBasePointer, _bp);
                _symbolTable[_symbolTableIndex] = symbolBasePointer;
                _evaluationStack.push_front(_symbolTableIndex++);   // push bp
                // _bp points to next available index. Note that 1 plus this index is the nodeFunctionReturn which we'll have to fill in when nodeFunctionCallEnd is called.
            }
            // 2. mov bp, sp
            _bp = _evaluationStack.stackFrameIndex();    
            break;
        case nodeFunctionCallEnd:
            // 1. Get the _programIndex from symbolTable
            _symbolTable[_currentProgramNodeValue()].dumpEntry();
            //oss() << endl << "!!!_currentProgramNodeValue()" << _currentProgramNodeValue();
            //dump();
            
            // Time to fill in nodeFunctionReturn
            {
                // _evaluationStack grows down and nodeFunctionReturn was pushed first so add 1 here.
                int nodeFunctionReturnIndex = _evaluationStack.peekAtIndex(_bp + 1);
                //oss() << endl << "!!!nodeFunctionReturnIndex:" << nodeFunctionReturnIndex;
                //dump();
                _symbolTable[nodeFunctionReturnIndex].value(_programIndex/*+1*/);   // Want to return to the next _programIndex, so + 1. Update, _programIndex is incremented so we don't have to do it here.
            }
            
            // 2. mov bp, sp
            //_bp = _programIndex;
            {
                int tentativeProgramIndex = _symbolTable[_currentProgramNodeValue()].fcnLink(); // Jump to new location
                if (tentativeProgramIndex < 0) {
                    // It's an intrinsic function call so make the call here locally.
                    int intrinsicReturnValue = 0;
                    int symbolIndexParameter1;
                    int symbolIndexReturnValue;
                    // 1. Get parameters and make the call to the intrinsic function. 
                    //     First parameter is always by reference since it's the return value, second parameter is by value
                    symbolIndexParameter1 = _evaluationStack.peekAtIndex(_bp - 2); // _bp points to the return symbol index ...
                    symbolIndexReturnValue = _evaluationStack.peekAtIndex(_bp - 1); // ... which is here
                    switch (tentativeProgramIndex) {
                    case INTRINSIC_FCN_DEFN_MOVE_ABSOLUTE:
                        //oss() << endl << "hello_programIndex:" << _programIndex;
                        //dump();
                        //oss() << endl << "_bp:" << _bp << " symbolIndexParameter1:" << symbolIndexParameter1;
                        //dump();
                        // There is no return value from stepper::moveAbsolute() so just fake it for now.
                        intrinsicReturnValue = 27;  // a dummy value
                        _s.moveAbsolute(_symbolTable[symbolIndexParameter1].value());
                        //intrinsicReturnValue = moveAbsolute(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_MOVE_RELATIVE:
                        intrinsicReturnValue = 26;  // a dummy value
                        _s.moveRelative(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_SLEEP:
#if !CYGWIN
                        vTaskDelay(_symbolTable[symbolIndexParameter1].value() / portTICK_RATE_MS); // See FreeRTOS for description
#endif // not CYGWIN                    
                        break;
                    case INTRINSIC_FCN_DEFN_SLEEP_UNTIL:
#if !CYGWIN
                        {
                            if (_firstIntrinsicFcnDefnSleepUntil) {
                                xTaskWakeTime = xTaskGetTickCount();
                                _firstIntrinsicFcnDefnSleepUntil = false;
                            }
                            vTaskDelayUntil(&xTaskWakeTime, _symbolTable[symbolIndexParameter1].value() / portTICK_RATE_MS);    // See FreeRTOS for description
                        }
#endif // not CYGWIN                    
                        break;
                    case INTRINSIC_FCN_DEFN_LED:
                        intrinsicReturnValue = 25;  // a dummy value
                        _led.set(_symbolTable[symbolIndexParameter1].value());
                        break;
                    default:
                        break;
                    }
                    _symbolTable[symbolIndexReturnValue].value(intrinsicReturnValue);

                    _evaluationStack.stackFrameIndex(_bp);   // mov sp, bp
                    unsigned int bpIndex = _evaluationStack.front();
                    _bp = _symbolTable[bpIndex].value();
                    
                    // Need to clean up symbols associated with the bp and function return index. Do it here.
#if CYGWIN
                    oss() << "Front" << _evaluationStack.front() << endl;
                    dump();
#endif /* CYGWIN */ 
                    _cleanUpEvaluationStack(2);   // clean up base pointer index, then function return index
                } else {
                    _programIndex = tentativeProgramIndex;
                }
            }
            break;
        case nodeProgramEnd:
            // done
#if CYGWIN
            oss() << endl << "nodeProgramEnd";
            dump();
#endif /* CYGWIN */    
            return;
        default:
#if CYGWIN
            oss() << endl << "ERROR interpret::run, invalid node type:" << _currentProgramNodeType();
            dump();
#endif /* CYGWIN */    
            assert(false);
        case nodeNOP:
        case nodeWhile:
        case nodeIf:
        case nodeEndif:
            // Nothing to do except increment the program counter for these nodes. Which is done automatically in the for loop.
            //++_programIndex;
            break;
        }
#if CYGWIN
        dumpEvaluationStack();
        // Show _bp and evaluationStack index
        oss() << "_bp:" << _bp << " _evaluationStack.stackFrameIndex():" << _evaluationStack.stackFrameIndex() << " _evaluationStack.size():" << _evaluationStack.size();
        dump();
        dumpSymbolTable();
#endif /* CYGWIN */    
    }
    _resetSymbolTableTemporaryBoundary();
}

void interpret::_updateProgramIndex(const nodeType thisNodeType, const int programIndex) {
    parseTreeEntry pte(thisNodeType, _currentProgramNodeValue(), _currentProgramNodeLevel() );
    int newProgramIndex = _findFirstParseTreeEntry(pte, programIndex);
    if (newProgramIndex == NOT_FOUND) {
        assert(newProgramIndex != NOT_FOUND);
    } else {
        _programIndex = newProgramIndex;
    }
}

void interpret::_cleanUpEvaluationStack(const unsigned int count) {
#if CYGWIN
    //oss() << "_cleanUpEvaluationStack" << endl;
    //dump();
#endif // CYGWIN
    for (unsigned int i = 0; i < count; ++i) {
        _symbolTable[_evalValuePeek()].type(nodeAvailable);
        _evaluationStack.pop_front();
    }
    _cleanUpSymbolTable();
}

void interpret::_cleanUpSymbolTable(void) {
#if CYGWIN
    //oss() << "_cleanUpSymbolTable:_symbolTableIndex:" << _getLastSymbolTableIndex() << " type:" << _symbolTable[_getLastSymbolTableIndex()].type() << endl;
    //dump();
#endif // CYGWIN
    while (_symbolTable[_getLastSymbolTableIndex()].type() == nodeAvailable) {
        //_symbolTable[_getLastSymbolTableIndex()].type(nodeInvalid);
        --_symbolTableIndex;
    }
}

void interpret::_shortCircuitOptimization(void) {
    parseTreeEntry pte(nodeOperator, OR, _currentProgramNodeLevel() - 1);  // Look for the first operator at (level - 1). Going further will detect an incorrect node.
    if (int candidateProgramIndex = _findFirstParseTreeEntry(pte, _programIndex) != NOT_FOUND) {
        // We found the parse tree entry we were looking for so let's see if the left-hand side it true, or, rather, !false.
        //if (_currentProgramNodeValue() != false) {
        if (_evalValuePeek() != false) { 
            // Left-hand side is true and next operator is OR so skip evaluation of right-hand
            //  side by moving _programIndex. But first, pop the value we just looked at.
            _evaluationStack.pop_front();
            _evaluationStack.push_front(true);  // The net result of the OR would be true so put it on the evaluation stack now
            _programIndex = candidateProgramIndex;    // Point to the OR operator we just evaluated
            return;
        }
    }
}

// Return programIndex location of the found entry
int interpret::_findFirstParseTreeEntry(const parseTreeEntry& p, const unsigned int startIndex) {
    // FIXME: This should start from the top, not _programIndex
    for (unsigned int i = startIndex; _programNodeType(i) != nodeInvalid/*_programIndexMax*/; ++i) {
#if CYGWIN
        //_program[i].dumpEntry();
#endif /* CYGWIN */    
        if (_program[i].type() == p.type() && _program[i].level() == p.level()) {
            // TODO: Combine this with above 'if' statement. Perhaps use: if (_program[i] == pte) {}
            if (_program[i].value() == p.value()) {
                return i;
            }
            continue;
            //return NOT_FOUND;
        }
    }
    return NOT_FOUND;
}

// Normally symbolTableIndex will return itself, however, for function arguments the symbolTableIndex
//  is pointed to by the entry in the stack frame. That's where the morphing comes in.
int interpret::symbolTableIndexMorph(const int symbolTableIndex) {
    int symbolTableIndexReturn = symbolTableIndex;
    unsigned int functionArgumentIndex;
    switch (_symbolTable[symbolTableIndex].type()) {
    case nodeTemporary:
        // Return to symbol table to be reused
        _symbolTable[symbolTableIndex].type(nodeAvailable);
    case nodeVariable:
    case nodeConst:
        break;
    case nodeArgumentValue:
    case nodeArgumentReference:
        functionArgumentIndex = _symbolTable[symbolTableIndex].value();
        // Now get index off the stack frame
        symbolTableIndexReturn = _evaluationStack.peekAtIndex(_bp - functionArgumentIndex - 1); // First parameter starts at: _bp - 1; second at _bp -2, etc.
#if CYGWIN
        oss() << " functionArgumentIndex:" << functionArgumentIndex << " symbolTableIndex:" << symbolTableIndexReturn << " ";
#endif /* CYGWIN */
        break;
    default:
#if CYGWIN
        oss() << " _symbolTable[symbolTableIndex].type():" << _symbolTable[symbolTableIndex].type();
        dump();
#endif /* CYGWIN */
        assert(false);
        // Almost certainly not what we want to do but not sure what to do at this point.
        break;
    }
    return symbolTableIndexReturn;
}

void interpret::evaluateOperator(unsigned int op) {
    int rightHandSymbolTableIndex = _evalValue(); // returns symbol table index
#if CYGWIN
    oss() << "rightHandSymbolTableIndex:" << rightHandSymbolTableIndex << " ";
#endif /* CYGWIN */
    // rhs -> right-hand symbol; lhs -> left-hand symbol
    int rhs = symbolFromIndex(rightHandSymbolTableIndex);
    int lhs; // evaluated in individual case statements below
    switch (op) {
    case TILDE:
#if CYGWIN
        oss() << "~" << rhs;
#endif /* CYGWIN */    
        _pushTemporarySymbolOnEvaluationStack(~rhs);
        break;
    case BANG:
#if CYGWIN
        oss() << "!" << rhs;
#endif /* CYGWIN */    
        _pushTemporarySymbolOnEvaluationStack(!rhs);
        break;
    case PLUS:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs + rhs);
#if CYGWIN
        oss() << lhs << " + " << rhs;
#endif /* CYGWIN */    
        break;
    case MINUS:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs - rhs);
#if CYGWIN
        oss() << lhs << " - " << rhs;
#endif /* CYGWIN */    
        break;
    case MULT:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs * rhs);
#if CYGWIN
        oss() << lhs << " * " << rhs;
#endif /* CYGWIN */    
        break;
    case DIV:
        {
            // tmp = denominator
            int numerator = symbolFromIndex(_evalValue());
            if (rhs == 0) {
                // If you attempt divide-by-zero we're going to give you back 0.
                assert(rhs != 0);
#if CYGWIN
                oss() << "ERROR" << numerator << " / 0";
#endif /* CYGWIN */    
                _pushTemporarySymbolOnEvaluationStack(0);
            } else {
#if CYGWIN
                oss() << numerator << " / " << rhs;
#endif /* CYGWIN */    
                _pushTemporarySymbolOnEvaluationStack(numerator / rhs);
            }
        }
        break;
    case XOR:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs ^ rhs);
#if CYGWIN
        oss() << lhs << " ^ " << rhs;
#endif /* CYGWIN */    
        break;
    case GEQ:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs >= rhs);
#if CYGWIN
        oss() << lhs << " >= " << rhs;
#endif /* CYGWIN */    
        break;
    case LEQ:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs <= rhs);
#if CYGWIN
        oss() << lhs << " <= " << rhs;
#endif /* CYGWIN */    
        break;
    case NEQ:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs != rhs);
#if CYGWIN
        oss() << lhs << " != " << rhs;
#endif /* CYGWIN */    
        break;
    case SHR:   // Shift-right
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs >> rhs);
#if CYGWIN
        oss() << lhs << " >> " << rhs;
#endif /* CYGWIN */    
        break;
    case SHL:   // Shift-left
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs << rhs);
#if CYGWIN
        oss() << lhs << " << " << rhs;
#endif /* CYGWIN */    
        break;
    case GTR:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs > rhs);
#if CYGWIN
        oss() << lhs << " > " << rhs;
#endif /* CYGWIN */    
        break;
    case LSS:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs < rhs);
#if CYGWIN
        oss() << lhs << " < " << rhs;
#endif /* CYGWIN */    
        break;
    case AND:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs && rhs);
#if CYGWIN
        oss() << lhs << " && " << rhs;
#endif /* CYGWIN */
        break;
    case OR:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs || rhs);
#if CYGWIN
        oss() << lhs << " || " << rhs;
#endif /* CYGWIN */    
        break;
    case BITWISEAND:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs & rhs);
#if CYGWIN
        oss() << lhs << " & " << rhs;
#endif /* CYGWIN */
        break;
    case BITWISEOR:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs | rhs);
#if CYGWIN
        oss() << lhs << " | " << rhs;
#endif /* CYGWIN */    
        break;
    case TEST_FOR_EQUAL:
        lhs = symbolFromIndex(_evalValue());
        _pushTemporarySymbolOnEvaluationStack(lhs == rhs);
#if CYGWIN
        oss() << lhs << " == " << rhs;
#endif /* CYGWIN */    
        break;
    case EQUAL:
        {
            // left-hand side = right-hand side
           int leftHandSymbolTableIndex = _evalValue(); // returns symbol table index
#if CYGWIN
            oss() << leftHandSymbolTableIndex << "(symbol index) = " << rhs;
#endif /* CYGWIN */
            _symbolTable[symbolTableIndexMorph(leftHandSymbolTableIndex)].value(rhs);
#if 0 //CYGWIN            
            if (_symbolTable[symbolTableIndexMorph(leftHandSymbolTableIndex)].type() == nodeTemporary) {
                oss() << leftHandSymbolTableIndex << endl << "TEMPORARY SYMBOL ";
            }
#endif /* CYGWIN */            
            //_evaluationStack.push_front(_evalValue() == _evalValue());
        }
        break;
    case LBRACKET:
        // Got something like: a[x], push symbol table index onto evaluation stack
        lhs = _evalValue();   // Array index
        {
        int arrayRange = _symbolTable[lhs].value();   // Array range. Remember, max index is 1 less.
#if CYGWIN
        //oss() << "Array range: " << arrayRange;
#endif /* CYGWIN */    
        if (rhs > arrayRange) {
            // Exceeded array range so cancel pattern or action and print error
            if (_evaluatingPattern) {
                // Abandon all evaluation here since it's impossible to tell if the pattern is true or not.
                // Move to start of next pattern and start evaluating. Probably need to reset evaluation stack.
            } else {
                // Can just abandon evaluation of this action statment and move on to the next
                // Move to start of next action and reset evaluation stack.
            }
#if CYGWIN
            oss() << "Array index out-of-bounds: " /*  TODO: Print symbol name. Currently not stored with symbol. */;
#endif /* CYGWIN */    
        } else {
            // The + 1 is because the very first index in the array stores the array range, not an array value.
            _evaluationStack.push_front(lhs + rhs + 1); // push symbol table index of a[x]
#if CYGWIN
            oss() << lhs << "[" << rhs << "]" << ", Range=" << arrayRange;
#endif /* CYGWIN */    
        }
        }
        break;
    default:
#if CYGWIN
        oss() << "ERROR interpret::evaluateBinaryOperator, op=" << op;
#endif /* CYGWIN */    
        //assert(false);
        break;
    }
#if CYGWIN
    dump();
#endif /* CYGWIN */    
}

void interpret::_pushTemporarySymbolOnEvaluationStack(unsigned int value) {
    symbolTableEntry temporarySymbol(nodeTemporary, value); 
#if 1
    int availableSymbolTableIndex = _findFirstAvailableNodeInSymbolTable();
    if (availableSymbolTableIndex != NOT_FOUND) {
        // Found a nodeAvailable
        _symbolTable[availableSymbolTableIndex] = temporarySymbol;
        _evaluationStack.push_front(availableSymbolTableIndex);
        return;
    }
#endif    
    _symbolTable[_symbolTableIndex] = temporarySymbol;
    _evaluationStack.push_front(_symbolTableIndex++);
    assert(_symbolTableIndex < MAX_SYMBOL_TABLE_ENTRY);
}

int interpret::_findFirstAvailableNodeInSymbolTable(void) {
    if (_symbolTableIndex == 0) {
        return NOT_FOUND;
    }
    assert(_getLastSymbolTableIndex() >= getLowWaterMarkForTemporarySymbolSearch());
    for (unsigned int index = _getLastSymbolTableIndex(); index > getLowWaterMarkForTemporarySymbolSearch(); --index) {
#if 0 //CYGWIN
        oss() << endl << "index:" << index;
        dump();
#endif /* CYGWIN */    
        symbolTableEntry ste = _symbolTable[index];
        if (ste.type() == nodeAvailable) {
            return index;
        }
    }
    return NOT_FOUND;
}

#if CYGWIN
int main(void) {
    interpret i;
    i.load();
#if CYGWIN
    i.dumpProgram();
    //i.dumpSymbolTable();
#endif /* CYGWIN */ 
    i.run();
#if CYGWIN
    //i.dumpSymbolTable();
#endif /* CYGWIN */ 
    //i.run();
#if CYGWIN
    //i.dumpSymbolTable();
#endif /* CYGWIN */ 
    return 0;
}
#endif /* CYGWIN */