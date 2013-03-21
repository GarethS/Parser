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
    //int _findParseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level);
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
    
#if 0    
	accel a;
	
    void step(void) {
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_STEP, PIN_STEP);
        if (_directionPositive) {
            ++_positionCurrent;
        } else {
            --_positionCurrent;
        }
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_STEP, ~PIN_STEP);
    }
      
    void directionPositive(bool positive = true) {
        _directionPositive = positive;
        if (_directionPositive) {
            GPIOPinWrite(GPIO_PORTA_BASE, PIN_DIR, (unsigned char)~PIN_DIR);
        } else {
            GPIOPinWrite(GPIO_PORTA_BASE, PIN_DIR, PIN_DIR);
        }
    }
    //bool directionPositive(void) {return _directionPositive;}
      
	// Only allow one of these, moveAbsolute() or velocity(), to be active at any one time.  
    void moveAbsolute(int positionNew);
	int velocity(const int f);
    void isr(void);
    //int acceleration(void) {/* TODO */}
    //int velocity(void) {/* TODO */}
    //int position(void) {/* TODO */} // Need to return both degrees and scaled position.
	
	void test(void);
    
private:
	// _superState
    enum {IDLE, MOVE_FULL, MOVE_TRUNCATED, VELOCITY_MOVE};
	
	// _subState
	enum {MOVE_START, MOVE_ACCELERATE, MOVE_CONSTANT_VELOCITY, MOVE_DECELERATE,
			VELOCITY_MOVE_ACCELERATE, VELOCITY_MOVE_DECELERATE, VELOCITY_MOVE_CONSTANT_VELOCITY};

    // return values
    enum {SUCCESS, ILLEGAL_MOVE, ILLEGAL_VELOCITY};                            
            
    void _init(void);
    
private:
	void _timerStart(bool start = true);
	void _timer(unsigned long newClockPeriod) {
		_timerPeriod = newClockPeriod;
		TimerLoadSet(TIMER0_BASE, TIMER_A, _timerPeriod);
	}
	unsigned long _timer(void) {return _timerPeriod;}
	void _updateConstantVelocityStart(void);
	
	// These 4 points represent a typical movement profile.
	//  Acceleration is from _positionCurrent to _positionConstantVelocityStart
	//  Constant velocity is from _positionConstantVelocityStart to _positionConstantVelocityEnd
	//  Deceleration is from _positionConstantVelocityEnd to _positionTarget
	// If, instead, a velocity() command is being issued, only 2 of the points are used.
	//  Increasing velocity is from _positionCurrent to _positionConstantVelocityStart
	//  Decreasing velocity is from _positionConstantVelocityEnd to _positionTarget
	// To change direction, 2 velocity, or moveAbsolute, commands should be used. One to bring
	//  the motor to 0 velocity and the next to move in the opposite direction.
    int _positionCurrent;  // in steps
	int _positionConstantVelocityStart;
	int _positionConstantVelocityEnd;
	int _positionTarget;
	
    bool _directionPositive;    // false -> direction is negative
	bool _timerRunning;
	unsigned long _timerPeriod;
    //int _acceleration;          // 0 when constant velocity
    //bool _accelerating; 
    //int _velocity;              // step/s
	unsigned int _fminOld, _fmaxOld;	// save frequency min/max for acceleration. Used when acceleration is trucated.
	unsigned int _superState;
	unsigned int _subState;
#endif    
};

#endif /* _INTERPRET_H_ */