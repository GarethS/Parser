/*
	Copyright (c) Gareth Scott 2012, 2013

	interpret.h 

*/

#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#include "parseTreeEntry.h"
#include "symbolTableEntry.h"
#define YYSTYPE_IS_DECLARED // prevent compile errors from valve.tab.h
#define YYSTYPE int         // ditto
#include "valve.tab.h"
#include "stepper.h"

// Can use either tinyQueue or a standard library deque
#include "tinyQueue.h"
#include "led.h"
#include "flashio.h"
//#include <deque>
#include <assert.h>

#include <sstream>
#if CYGWIN
#include "log.h"

using namespace std;
#endif /* CYGWIN */

#define MAX_PROGRAM_ENTRY           (200)
#define MAX_SYMBOL_TABLE_ENTRY      (60)
#define PROGRAM_INDEX_START         (0)
#define VERSION_STRING              "00.00.08"
#define VERSION_NUMBER              000008

extern "C" {
void bufferInput(unsigned char c);
}

class interpret
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    interpret();
    ~interpret() {}
    
    void load(void);
    bool appendToProgram(const parseTreeEntry& pte);
    bool appendToSymbolTable(const symbolTableEntry& ste);
#if CYGWIN    
    void dumpProgram(void);
    void dumpSymbolTable(void);
    void dumpEvaluationStack(void);
#else /* not CYGWIN */
    stepper* getStepper(void) {return &_s;}
    void setLowWaterMarkForTemporarySymbolSearch(void) {_symbolTableTemporaryBoundaryIndex = _symbolTableIndex - 1;}
#endif /* CYGWIN */    
    void run(void);
    void evaluateOperator(unsigned int op);
    void evaluateUnaryOperator(unsigned int op);

    void saveToFlash(flashRegion f);
    void restoreFromFlash(flashRegion f);
    
private:
    unsigned int _getIntSize(flashRegion f);
#if CYGWIN
    void _loadTree(const string& s);
    void _loadSymbolTable(const string& s);
    void setLowWaterMarkForTemporarySymbolSearch(void) {_symbolTableTemporaryBoundaryIndex = _symbolTableIndex - 1;}
    bool _loadParseTreeEntry(const string& inputString);
    bool _loadSymbolTableEntry(const string& inputString);
#endif /* CYGWIN */
    nodeType _currentProgramNodeType(void) const {return _program[_programIndex].type();}
    nodeType _programNodeType(unsigned int i) const {return _program[i].type();}
    unsigned int _currentProgramNodeValue(void) const {return _program[_programIndex].value();}
    unsigned int _currentProgramNodeLevel(void) const {return _program[_programIndex].level();}
    
    int _evalValue(void) {assert(_evaluationStack.size() >= 1); int _value = _evalValuePeek(); _evaluationStack.pop_front(); return _value;}
    int _evalValuePeek(void) const {return _evaluationStack.front();}   // We don't pop the _evaluationStack like _evalValue() does
    int _findFirstParseTreeEntry(const parseTreeEntry& p, const unsigned int startIndex);
    void _shortCircuitOptimization(void);
    void _resetSymbolTableTemporaryBoundary(void) {_symbolTableIndex = _symbolTableTemporaryBoundaryIndex;}
    void _pushTemporarySymbolOnEvaluationStack(unsigned int value);
    int _findFirstAvailableNodeInSymbolTable(void);
    void _cleanUpEvaluationStack(const unsigned int count);
    void _cleanUpSymbolTable(void);
    void _returnSymbolToAvailablePool(const unsigned int symbolTableIndex) {if (_symbolTable[symbolTableIndex].type() == nodeTemporary) {_symbolTable[symbolTableIndex].type(nodeAvailable);}}
    void _updateProgramIndex(const nodeType thisNodeType, const int programIndex);
    unsigned int _getLastSymbolTableIndex(void) {if (_symbolTableIndex == 0) {return _symbolTableIndex;} return _symbolTableIndex - 1;}
 
    //unsigned int _functionArgumentIndexToStackFrameIndex(const unsigned int functionArgumentIndex) {}
    int symbolTableIndexMorph(const int symbolTableIndex);
    int symbolFromIndex(const int symbolTableIndex) {return _symbolTable[symbolTableIndexMorph(symbolTableIndex)].value();} // get symbol
    unsigned int getLowWaterMarkForTemporarySymbolSearch(void) const {return _symbolTableTemporaryBoundaryIndex;}
    
#if !CYGWIN
    bool _firstIntrinsicFcnDefnSleepUntil;
    flashio _flash;
#endif // not CYGWIN    
    parseTreeEntry _program[MAX_PROGRAM_ENTRY];
    symbolTableEntry _symbolTable[MAX_SYMBOL_TABLE_ENTRY];
    int _programIndex;  // Negative values indicate a built-in function
    unsigned int _symbolTableIndex; // Always points to an available symbol location
    // Stack frames on the intel 8086 were set up using the idiom:
    //   push bp
    //   move bp, sp
    // A nod to posterity; the same idiom is used here
    unsigned int _bp;  // base pointer 
    unsigned int _symbolTableTemporaryBoundaryIndex; // Boundary between temporary symbols (higher index) and regular symbols (index from 0). Use for optimization later on.
    tinyQueue<int> _evaluationStack;    // Contains the index of a symbol, not its value.
    bool _evaluatingPattern;
    stepper _s;
    led _led;
};

#endif /* _INTERPRET_H_ */