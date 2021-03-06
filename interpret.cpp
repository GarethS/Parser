/*
	Copyright (c) Gareth Scott 2012, 2013, 2014

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
#include "profiler.h"
#else /* not CYGWIN */

#include "stdio.h"
#include "string.h"
#include "../motor/LED.h"
#include "FreeRTOS.h"

bool led::_enable = FALSE;

extern "C" {

void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void interpretState(void);

//FreeRTOS specific below
static portTickType xTaskWakeTime;
static portTickType xTaskIdleWaitStartTime;
static portTickType xTaskIdleWaitCurrentTime;
static portTickType tickCount;

void vTaskDelay( portTickType xTicksToDelay ) PRIVILEGED_FUNCTION;                                                  // task.h
void vTaskDelayUntil( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement ) PRIVILEGED_FUNCTION;   // task.h
portTickType xTaskGetTickCount( void ) PRIVILEGED_FUNCTION;                                                         // task.h
unsigned portBASE_TYPE uxTaskGetStackHighWaterMark( void* xTask ) PRIVILEGED_FUNCTION;
}   // extern "C"

interpret interpreter;
static interpreterState globalInterpretState = STATE_POWER_UP;
//static interpreterState globalInterpretState = STATE_HALT;
extern stepper* pStepper;

void interpretState(void) {
#if 0    
    //flashio f;
#define FLASH_TEST_SIZE (4)    
    //uint32_t fArray[FLASH_TEST_SIZE] = {0x22, 0x55, 0x73, 0x2};
    //f.saveData(fArray, FLASH_TEST_SIZE);
#endif    
    for (;;) {
        static int programIndex = 0;
        switch (globalInterpretState) {
        case STATE_RUN:
            pStepper = interpreter.getStepper();
            
            pStepper->microstepSet(MICROSTEPS_8);
            pStepper->accelerationTimeMicrosecs(200000);
            pStepper->RPM(12, 220);
            
            led::enable(1);    
            programIndex = interpreter.run();
            globalInterpretState = STATE_HALT;
            break;
        case STATE_POWER_UP:
            interpreter.restoreFromFlash(FLASH_PROGRAM);
            interpreter.restoreFromFlash(FLASH_SYMBOL_TABLE);
            // Just going to try running the program. Halts if no program actually stored in flash.
            globalInterpretState = STATE_RUN;
            break;
        case STATE_HALT:
            vTaskDelay(100 / portTICK_RATE_MS);
            break;
        case STATE_CONTINUE:
            interpreter.run(programIndex);
            globalInterpretState = STATE_HALT;
            break;
        case STATE_FLASH:
            interpreter.saveToFlash(FLASH_PROGRAM);
            interpreter.saveToFlash(FLASH_SYMBOL_TABLE);
            globalInterpretState = STATE_HALT;
            break;
        default:
            assert(false);
        }
    }
}

void successMsg(void) {
    static const char* msg = "<OK>";
    UARTSend((unsigned char *)msg, strlen(msg));
}

void errMsg(void) {
    static const char* msg = "<ER>";
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

//__ramfunc nodeType intToNodeType(const int nt) {
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
    static tinyQueue<unsigned char> serialInput(0);

    static const char statementBegin[]  = "<ST>";  // StatemenT
    static const char statementEnd[]    = "<st>";

    static const char symbolBegin[]     = "<SL>";  // SymboL
    static const char symbolEnd[]       = "<sl>";
    
    static const char programRun[]      = "<RN>";  // RuN
    static const char programStop[]     = "<SP>";  // StoP
    static const char programContinue[] = "<CE>";  // ContinuE
    static const char version[]         = "<VN>";  // VersioN
    static const char flash[]           = "<FH>";  // save to FlasH
    //static const char bootloader[]      = "<BR>";  // BootloadeR

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
    // Statement
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
    // Symbol
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
    // Run 
    if (strstr((const char*)serialInput.getBuffer(), (const char*)programRun) != NULL) {
        interpreter.setLowWaterMarkForTemporarySymbolSearch();
        serialInput.clear();
        postMsg(TRUE);
        globalInterpretState = STATE_RUN;
        return;
    }
    // Stop 
    if (strstr((const char*)serialInput.getBuffer(), (const char*)programStop) != NULL) {
        serialInput.clear();
        postMsg(TRUE);
        globalInterpretState = STATE_HALT;
        return;
    }
    // Continue 
    if (strstr((const char*)serialInput.getBuffer(), (const char*)programContinue) != NULL) {
        serialInput.clear();
        postMsg(TRUE);
        globalInterpretState = STATE_CONTINUE;
        return;
    }
    // Flash 
    if (strstr((const char*)serialInput.getBuffer(), (const char*)flash) != NULL) {
        serialInput.clear();
        postMsg(TRUE);
        globalInterpretState = STATE_FLASH;
        return;
    }
    // Version 
    if (strstr((const char*)serialInput.getBuffer(), (const char*)version) != NULL) {
        serialInput.clear();
        UARTSend((unsigned char *)VERSION_STRING, strlen(VERSION_STRING));
        return;
    }
}
#endif /* CYGWIN */

static char tempBuf[32];

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRETER")),
#else /* not CYGWIN */
                    _firstIntrinsicFcnDefnSleepUntil(true),
#endif /* CYGWIN */		
                    _programIndex(PROGRAM_INDEX_START), _symbolTableIndex(0), _resetIndicesOnNextProgramLoad(false), _bp(PROGRAM_INDEX_START) {
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
    //_stepper.RPM(1, 24);    
}

void interpret::load(void) {
#if CYGWIN
    _loadTree(string("tree.txt"));
    _loadSymbolTable("symbolTable.txt");
#endif // CYGWIN    
}

bool interpret::appendToProgram(const parseTreeEntry& pte) {
    if (_resetIndicesOnNextProgramLoad) {
        _programIndex = _symbolTableIndex = 0;
        _resetIndicesOnNextProgramLoad = false;
    }
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

int interpret::run(int initialProgramIndex /*= 0*/) {
#if !CYGWIN    
#ifndef NDEBUG    
    static unsigned portBASE_TYPE uxHighWaterMark;
#endif // not NDEBUG 
#endif // not CYGWIN
    _resetIndicesOnNextProgramLoad = true;
    if (initialProgramIndex == 0) {
        _evaluationStack.clear();
    }
    //assert(_evaluationStack.empty());
    int localSymbolTableIndex;
    _io.init();
#if CYGWIN
#define CYGWIN_MAX_INFINITE_LOOP    (1000)
    static int infiniteLoopCounter = 0;
#endif /* CYGWIN */    
    for (_programIndex = initialProgramIndex; _program[_programIndex].type() != nodeInvalid; ++_programIndex) {
#if CYGWIN
        if (++infiniteLoopCounter > CYGWIN_MAX_INFINITE_LOOP) {
            break;
        }
        oss() << endl << "Run programIndex=" << _programIndex << " programNodeType:" << _currentProgramNodeType() << " programNodeValue:" << _currentProgramNodeValue();
        dump();
        //dumpEvaluationStack();
#else // not CYGWIN        
        if (globalInterpretState == STATE_HALT) {
            // Got an external command over the USB cable to halt the program. Return current _programIndex in case
            //  we want to restart
            return _programIndex;
        }
#ifndef NDEBUG    
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < 120) {
            // Set breakpoint here to catch this
            uxHighWaterMark = uxHighWaterMark;
        }
#endif // not NDEBUG    
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
                    int symbolIndexParameter1, symbolIndexParameter2;
                    int symbolIndexReturnValue;
                    unsigned int yieldMilliseconds;
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
                        _stepper.moveAbsolute(_symbolTable[symbolIndexParameter1].value());
                        //intrinsicReturnValue = moveAbsolute(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_MOVE_RELATIVE:
                        intrinsicReturnValue = 26;  // a dummy value
                        _stepper.moveRelative(_symbolTable[symbolIndexParameter1].value());
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
                        intrinsicReturnValue = 25;  // dummy value
                        _led.set(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_RPM:
                        intrinsicReturnValue = 25;  // dummy value
                        symbolIndexParameter2 = _evaluationStack.peekAtIndex(_bp - 3);
                        _stepper.RPM(_symbolTable[symbolIndexParameter1].value(), _symbolTable[symbolIndexParameter2].value());
                        break;
                    case INTRINSIC_FCN_DEFN_RPM_x10k:
                        intrinsicReturnValue = 25;  // dummy value
                        symbolIndexParameter2 = _evaluationStack.peekAtIndex(_bp - 3);
                        _stepper.RPMx10k(_symbolTable[symbolIndexParameter1].value(), _symbolTable[symbolIndexParameter2].value());
                        break;
                    case INTRINSIC_FCN_DEFN_ACCEL_MICROSEC:
                        intrinsicReturnValue = 25;  // dummy value
                        _stepper.accelerationTimeMicrosecs(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_DEGREE_x10k_ABSOLUTE:
                        intrinsicReturnValue = 25;  // dummy value
                        _stepper.moveAbsoluteDegreex10k(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_DEGREE_x10k_RELATIVE:
                        intrinsicReturnValue = 25;  // dummy value
                        _stepper.moveRelativeDegreex10k(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_WAIT_FOR_IDLE:
                        intrinsicReturnValue = RETURN_OK;
                        yieldMilliseconds = _symbolTable[symbolIndexParameter1].value();
                        if (yieldMilliseconds == 0) {
                            // Don't wait, just tell whether motor is busy or idle
                            if (_stepper.state() != IDLE) {
                                intrinsicReturnValue = RETURN_NOT_IDLE;
                            }
                            break;
                        }
#if !CYGWIN                        
                        xTaskIdleWaitStartTime = xTaskGetTickCount();
                        while (_stepper.state() != IDLE) {
                            vTaskDelay(0);  // equivalent to taskYIELD()
                            xTaskIdleWaitCurrentTime = xTaskGetTickCount();
                            if (xTaskIdleWaitStartTime > xTaskIdleWaitCurrentTime) {
                                // Timer count wraparound
                                tickCount = portMAX_DELAY - xTaskIdleWaitStartTime + xTaskIdleWaitCurrentTime;
                            } else {
                                tickCount = xTaskIdleWaitCurrentTime - xTaskIdleWaitStartTime;
                            }
                            if (tickCount / portTICK_RATE_MS > yieldMilliseconds) {
                                intrinsicReturnValue = RETURN_ERR_TIMEOUT;
                                break;
                            }
                        }
#endif // not CYGWIN                        
                        break;
                    case INTRINSIC_FCN_DEFN_PRINT_NUMBER:
                        intrinsicReturnValue = sprintf(tempBuf, "<%d>", _symbolTable[symbolIndexParameter1].value());
#if !CYGWIN                        
                        UARTSend((unsigned char *)tempBuf, intrinsicReturnValue);
#endif // not CYGWIN                        
                        break;
                    case INTRINSIC_FCN_DEFN_GET_INPUT:
                        intrinsicReturnValue = _io.getInput();
                        break;
                    case INTRINSIC_FCN_DEFN_SET_OUTPUT:
                        _io.setOutput(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_GET_ADC:
                        intrinsicReturnValue = _io.getADC(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_GET_TEMP:
                        intrinsicReturnValue = _io.getTemperature();
                        break;
                    case INTRINSIC_FCN_DEFN_GET_POSITION:
                        intrinsicReturnValue = _stepper.positionSteps();
                        break;
                    case INTRINSIC_FCN_DEFN_RESET:
                        _io.reset();
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
            return 0;
        default:
#if CYGWIN
            oss() << endl << "ERROR interpret::run, invalid node type:" << _currentProgramNodeType();
            dump();
#endif /* CYGWIN */
            // Will get here if device has never had control program saved in FLASH_PROGRAM. Memory instead gets filled
            //  with 0xffff which is the erased value of flash memory. Exiting here will cause state machine to revert
            //  to STATE_HALT
            return 0;
            //assert(false);
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
    return 0;
}

void interpret::_updateProgramIndex(const nodeType thisNodeType, const int programIndex) {
#if 0
    static float microSecCounter = 0.0;
    profiler p(microSecCounter);
#endif // CYGWIN
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
    //static int count = 0;
    //oss() << "interpret::evaluateOperator() count:" << ++count << " ";
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
            // Exceeded array range
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
    static symbolTableEntry temporarySymbol(nodeTemporary, 0);
    temporarySymbol.value(value);   
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
        if (_symbolTable[index].type() == nodeAvailable) {
            return index;
        }
    }
    return NOT_FOUND;
}

void interpret::saveToFlash(flashRegion fr) {
#if !CYGWIN    
    switch (fr) {
    case FLASH_PROGRAM:
        _flash.saveData((uint32_t*)_program, FLASH_START_ADDRESS_PROGRAM, _getIntSize(fr));
        break;
    case FLASH_SYMBOL_TABLE:
        // _symbolTableIndex needs to be restored. Make sure that you flash just after program download and
        //  not after program has been running which could affect _symbolTableIndex
        // TODO: Find a more robust way to do this rather than just stuffing it in the last symbol table entry.
        _symbolTable[MAX_SYMBOL_TABLE_ENTRY-1].value(_symbolTableIndex);
        _flash.saveData((uint32_t*)_symbolTable, FLASH_START_ADDRESS_SYMBOL_TABLE, _getIntSize(fr));
        break;
    default:
        assert(false);
        break;
    }
#endif // not CYGWIN    
}

void interpret::restoreFromFlash(flashRegion fr) {
#if !CYGWIN    
    switch (fr) {
    case FLASH_PROGRAM:
        _flash.retrieveData((uint32_t*)_program, FLASH_START_ADDRESS_PROGRAM, _getIntSize(fr));
        break;
    case FLASH_SYMBOL_TABLE:
        _flash.retrieveData((uint32_t*)_symbolTable, FLASH_START_ADDRESS_SYMBOL_TABLE, _getIntSize(fr));
        _symbolTableIndex = _symbolTable[MAX_SYMBOL_TABLE_ENTRY-1].value();
        _symbolTableTemporaryBoundaryIndex = _symbolTableIndex - 1;
        break;
    default:
        assert(false);
        break;
    }
#endif // not CYGWIN
}

unsigned int interpret::_getIntSize(flashRegion fr) {
#if CYGWIN
    return 0;
#else // not CYGWIN
//#define MAX_TEST_ENTRY  (3)    
    unsigned int totalByteCount = sizeof(parseTreeEntry) * MAX_PROGRAM_ENTRY;
    if (fr == FLASH_SYMBOL_TABLE) {
        totalByteCount = sizeof(symbolTableEntry) * MAX_SYMBOL_TABLE_ENTRY;
    }
    unsigned int totalIntCount = totalByteCount / sizeof(int);
    if (totalByteCount % sizeof(int) != 0) {
        // Some stray bytes caused totalByteCount to not be an integral multiple of 4
        ++totalIntCount;
    }
    return totalIntCount;
#endif // not CYGWIN    
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