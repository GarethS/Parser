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

// Can use either tinyQueue or an official deque
#include "tinyQueue.h"
//#include <deque>
#include <assert.h>

#if CYGWIN
//#include <iostream>
#include "log.h"
#include <sstream>

using namespace std;
#endif /* CYGWIN */

#define MAX_PROGRAM_ENTRY           (200)
#define MAX_SYMBOL_TABLE_ENTRY      (60)
#define PROGRAM_INDEX_START         (0)

class interpret
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    interpret();
    ~interpret() {}
    
#if CYGWIN    
    void load(void);
    void dumpProgram(void);
    void dumpSymbolTable(void);
    void dumpEvaluationStack(void);
#endif /* CYGWIN */    
    void run(void);
    void evaluate(unsigned int op);

private:
    void _loadTree(const string& s);
    void _loadSymbolTable(const string& s);
    nodeType _currentProgramNodeType(void) const {return _program[_programIndex].type();}
    nodeType _programNodeType(unsigned int i) const {return _program[i].type();}
    unsigned int _currentProgramNodeValue(void) const {return _program[_programIndex].value();}
    unsigned int _currentProgramNodeLevel(void) const {return _program[_programIndex].level();}
    
    int _evalValue(void) {assert(_evaluationStack.size() >= 1); int _value = _evaluationStack.front(); _evaluationStack.pop_front(); return _value;}
    int _evalValuePeek(void) const {return _evaluationStack.front();}
    int _findFirstParseTreeEntry(const parseTreeEntry& p, const unsigned int startIndex);
    void _shortCircuitOptimization(void);
    void _resetSymbolTableTemporaryBoundary(void) {_symbolTableIndex = _symbolTableTemporaryBoundaryIndex;}
    void _pushSymbolOnEvaluationStack(unsigned int value);
    //unsigned int _functionArgumentIndexToStackFrameIndex(const unsigned int functionArgumentIndex) {}
    int symbolTableIndexMorph(const int symbolTableIndex);
    int symbolFromIndex(const int symbolTableIndex) {return _symbolTable[symbolTableIndexMorph(symbolTableIndex)].value();} // get symbol

    
    parseTreeEntry _program[MAX_PROGRAM_ENTRY];
    symbolTableEntry _symbolTable[MAX_SYMBOL_TABLE_ENTRY];
    int _programIndex;  // Negative values indicate a built-in function
    unsigned int _symbolTableIndex;
    // Stack frames on the intel 8086 were set up using the idiom:
    //   push bp
    //   move bp, sp
    // With a nod to posterity, the same idiom is being used here
    unsigned int _bp;  // base pointer 
    unsigned int _symbolTableTemporaryBoundaryIndex; // Boundary between temporary symbols (higher index) and regular symbols (index from 0). Use for optimization later on.
    tinyQueue<int> _evaluationStack;    // Contains the index of a symbol, not its value.
    //deque<int> _evaluationStack;    // TODO: Initial version only use int. In future, floats or fixed-point arithmetic
    bool _evaluatingPattern;
    stepper _s;
};

#endif /* _INTERPRET_H_ */