/*
	Copyright (c) Gareth Scott 2012

	interpret.cpp 

    interpretIndex = 0;
    while (interpretIndex <= interpretEnd) {
        switch (program[interpretIndex].type) {
        case operand: {
            evalStack.push(program[interpretIndex].value);
            break;
        case operator:
            // A binary operator will pop the 2 operands off the stack,
            //  evaluate and push the result back.
            // For short-circuit optimization, look up the program for the
            //  operator at the next highest level and if it's || and the
            //  current evaluation is TRUE then move the interpretIndex just
            //  past that || operator.
            evaluate(program[interpretIndex].value);
            break;
        default:
            assert(false);
            break;
        }
    }
    
*/

#include "interpret.h"
#if CYGWIN
//#include <iostream>
//#include <fstream>
#include <string>
#include <istream>
#include <sstream>
#endif /* CYGWIN */
#include <assert.h>

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRETER"))
#endif /* CYGWIN */		
                    , _programIndex(0), _symbolTableIndex(0), _evaluatingPattern(true) {
#if 0                    
    for (int i = 0; i < MAX_PROGRAM_ENTRY; ++i) {
        //printf("%d %d\n", i, _program[i].type());
        oss() << i << " " << _program[i].type();
        dump();
        //_program[i].type(nodeInvalid);
    }
#endif    
}

#if CYGWIN
void interpret::load(void) {
    _loadTree(string("patternTree.txt"));
    _loadTree(string("actionTree.txt"));
    _loadSymbolTable("symbolTable.txt");
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
    // Typical line to read: '3 LEFT Variable 0'
    unsigned int level;
    string leftRight;
    string variableOperator;
    int value;
    while (!ifs.eof()) {
        string inputString;
        //ifs.getline(inputString);
        getline(ifs, inputString);
        //cout << "IFS:" << inputString << endl;
        if (inputString == "") {
            //cout << "Got endln" << endl;
            break;
        }
        istringstream iss(inputString);
        iss >> level >> leftRight >> variableOperator >> value;
        parseTreeEntry pte;
        pte.level(level);
        pte.value(value);
        if (variableOperator == "Variable") {
            pte.type(nodeVar);
        } else if (variableOperator == "Operator") {
            pte.type(nodeOperator);
        } else if (variableOperator == "Action") {
            pte.type(nodeStartAction);
        } else if (variableOperator == "If") {
            pte.type(nodeIf);
        }
        _program[_programIndex++] = pte;
        if (_programIndex >= MAX_PROGRAM_ENTRY) {
            oss() << "Out of program memory. Aborting.";
            dump();
            break;
        }
        //oss() << level << " " << leftRight << " " << variableOperator << " " << value;
        //dump();
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
    // Typical line to read: '1 4', where 1 = nodeType, 4 = value. In this case it's a constant with value = 4.
    unsigned int type;
    int value;
    while (!ifs.eof()) {
        string inputString;
        //ifs.getline(inputString);
        getline(ifs, inputString);
        //cout << "IFS:" << inputString << endl;
        if (inputString == "") {
            //cout << "Got endln" << endl;
            break;
        }
        istringstream iss(inputString);
        iss >> type >> value;
        symbolTableEntry ste;
        ste.type((nodeType)type);
        ste.value(value);
        _symbolTable[_symbolTableIndex++] = ste;
        if (_symbolTableIndex >= MAX_SYMBOL_TABLE_ENTRY) {
            oss() << "Exceeded symbol table size. Aborting.";
            dump();
            break;
        }
        //oss() << level << " " << leftRight << " " << variableOperator << " " << value;
        //dump();
    }
    _symbolTableTemporaryBoundaryIndex = _symbolTableIndex;
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
    for (int i = 0; _symbolTable[i].type() != nodeInvalid; ++i) {
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
    // 2. Dump contents of the copy
    while (!_evaluationCopy.empty()) {
        oss() << "Symbol index=" << _evaluationCopy.front() << " value=" << _symbolTable[_evaluationCopy.front()].value();
        dump();
        _evaluationCopy.pop_front();
    }
    oss() << "LEAVE interpret::dumpEvaluationStack";
    dump();
}
#endif /* CYGWIN */

void interpret::run(void) {
    assert(_evaluationStack.empty());
    for (_programIndex = 0; _program[_programIndex].type() != nodeInvalid; ++_programIndex) {
#if CYGWIN
        oss() << endl << "Run programIndex=" << _programIndex;
        dump();
        //dumpEvaluationStack();
#endif /* CYGWIN */    
        switch (_currentProgramNodeType()) {
        case nodeVar:
        case nodeConst:
            //printf("nodeVar or nodeConst\n");
            _evaluationStack.push_front(_currentProgramNodeValue());
            //_shortCircuitOptimization();
            break;
        case nodeOperator:
            //printf("nodeOperator\n");
            evaluate(_currentProgramNodeValue());
            //_shortCircuitOptimization();
            break;
        case nodeStartAction:
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
        default:
            assert(false);
            break;
        }
#if CYGWIN
        dumpEvaluationStack();
#endif /* CYGWIN */    
    }
    _resetSymbolTableTemporaryBoundary();
}

void interpret::_shortCircuitOptimization(void) {
    parseTreeEntry pte(nodeOperator, OR, _currentProgramNodeLevel() - 1);  // Look for the first operator at (level - 1). Going further will detect an incorrect node.
    if (int candidateProgramIndex = _findFirstParseTreeEntry(pte) != NOT_FOUND) {
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
int interpret::_findFirstParseTreeEntry(const parseTreeEntry& p) {
    for (unsigned int i = _programIndex; _programNodeType(i) != nodeInvalid/*_programIndexMax*/; ++i) {
        if (_program[i].type() == p.type() && _program[i].level() == p.level()) {
            if (_program[i].value() == p.value()) {
                return true;
            }
            return NOT_FOUND;
        }
    }
    return NOT_FOUND;
}

#if 0
int interpret::_findParseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level) {
    parseTreeEntry pte(t, p, value, level);
    for (unsigned int i = _programIndex; i < _programIndexMax; ++i) {
        if (_program[i] == pte) {
            return true;
        }
    }
    return NOT_FOUND;
}
#endif

void interpret::evaluate(unsigned int op) {
    assert(_evaluationStack.size() >= 1);
    int rhs = _symbolTable[_evalValue()].value(); // this is usually the right-hand side of the parse node, the exception being BANG below
    int lhs;
    switch (op) {
    case BANG:
#if CYGWIN
        oss() << "!" << rhs;
#endif /* CYGWIN */    
        _pushSymbolOnEvaluationStack(!rhs);
        break;
    case PLUS:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs + rhs);
#if CYGWIN
        oss() << lhs << " + " << rhs;
#endif /* CYGWIN */    
        break;
    case MINUS:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs - rhs);
#if CYGWIN
        oss() << lhs << " - " << rhs;
#endif /* CYGWIN */    
        break;
    case MULT:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs * rhs);
#if CYGWIN
        oss() << lhs << " * " << rhs;
#endif /* CYGWIN */    
        break;
    case DIV:
        assert(_evaluationStack.size() >= 1);
        {
            // tmp = denominator
            int numerator = _symbolTable[_evalValue()].value();
            if (rhs == 0) {
                // If you attempt divide-by-zero we're going to give you back 0.
                assert(rhs != 0);
#if CYGWIN
                oss() << "ERROR" << numerator << " / 0";
#endif /* CYGWIN */    
                _pushSymbolOnEvaluationStack(0);
            } else {
#if CYGWIN
                oss() << numerator << " / " << rhs;
#endif /* CYGWIN */    
                _pushSymbolOnEvaluationStack(numerator / rhs);
            }
        }
        break;
    case XOR:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs ^ rhs);
#if CYGWIN
        oss() << lhs << " ^ " << rhs;
#endif /* CYGWIN */    
        break;
    case GEQ:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs >= rhs);
#if CYGWIN
        oss() << lhs << " >= " << rhs;
#endif /* CYGWIN */    
        break;
    case LEQ:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs <= rhs);
#if CYGWIN
        oss() << lhs << " <= " << rhs;
#endif /* CYGWIN */    
        break;
    case NEQ:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs != rhs);
#if CYGWIN
        oss() << lhs << " != " << rhs;
#endif /* CYGWIN */    
        break;
    case GTR:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs > rhs);
#if CYGWIN
        oss() << lhs << " > " << rhs;
#endif /* CYGWIN */    
        break;
    case LSS:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs < rhs);
#if CYGWIN
        oss() << lhs << " < " << rhs;
#endif /* CYGWIN */    
        break;
    case AND:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs && rhs);
#if CYGWIN
        oss() << lhs << " && " << rhs;
#endif /* CYGWIN */
        break;
    case OR:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs || rhs);
#if CYGWIN
        oss() << lhs << " || " << rhs;
#endif /* CYGWIN */    
        break;
    case TEST_FOR_EQUAL:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs == rhs);
#if CYGWIN
        oss() << lhs << " == " << rhs;
#endif /* CYGWIN */    
        break;
    case EQUAL:
        assert(_evaluationStack.size() >= 1);
        {
            // left-hand side = right-hand side
           int leftHandSymbolTableIndex = _evalValue(); // returns symbol table index
#if CYGWIN
            oss() << leftHandSymbolTableIndex << "(symbol index) = " << rhs;
#endif /* CYGWIN */    
            // Set symbol table entry = rhs;
            if (_symbolTable[leftHandSymbolTableIndex].type() == nodeConst) {
                // It's a constant so don't modify it's value
                assert(_symbolTable[leftHandSymbolTableIndex].type() != nodeConst);
            } else {
                _symbolTable[leftHandSymbolTableIndex].value(rhs);
            }
            //_evaluationStack.push_front(_evalValue() == _evalValue());
        }
        break;
    case LBRACKET:
        // Got something like: a[x], push symbol table index onto evaluation stack
        assert(_evaluationStack.size() >= 1);
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
        assert(false);
#if CYGWIN
        oss() << "ERROR interpret::evaluate";
#endif /* CYGWIN */    
        break;
    }
#if CYGWIN
    dump();
#endif /* CYGWIN */    
}

// Push temporary symbol
void interpret::_pushSymbolOnEvaluationStack(unsigned int value) {
    symbolTableEntry temporarySymbol(nodeTemporary, value); 
    _symbolTable[_symbolTableIndex] = temporarySymbol;
    _evaluationStack.push_front(_symbolTableIndex++);
    assert(_symbolTableIndex < MAX_SYMBOL_TABLE_ENTRY);
}

#if 0
void stepper_init(void) {
    GPIODirModeSet(GPIO_PORTA_BASE, PIN_ALL, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTA_BASE, PIN_ALL, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
}

void stepper::test(void) {
#if CYGWIN
	oss() << "moveAbsolute START" << endl;
	moveAbsolute(2000);
	oss() << "_positionCurrent=" << _positionCurrent <<
			" _positionConstantVelocityStart=" << _positionConstantVelocityStart <<
			" _positionConstantVelocityEnd=" << _positionConstantVelocityEnd <<
			" _positionTarget=" << _positionTarget << endl;
	oss() << "moveAbsolute END";
	dump();
	// Now let's run the isr.
	for (int i = 0; i < 2010 && _superState != IDLE; ++i) {
		isr();
	}
	
	oss() << endl << "moveAbsolute START" << endl;
	moveAbsolute(0);
	oss() << "_positionCurrent=" << _positionCurrent <<
			" _positionConstantVelocityStart=" << _positionConstantVelocityStart <<
			" _positionConstantVelocityEnd=" << _positionConstantVelocityEnd <<
			" _positionTarget=" << _positionTarget << endl;
	oss() << "moveAbsolute END";
	dump();
	for (int i = 0; i < 2010 && _superState != IDLE; ++i) {
		isr();
	}

	// velocity move
	oss() << endl << "velocity START" << endl;
	velocity(800);
	oss() << "_positionCurrent=" << _positionCurrent <<
			" _positionConstantVelocityStart=" << _positionConstantVelocityStart << endl;
	oss() << "velocity END";
	dump();
	for (int i = 0; i < 2000; ++i) {
		isr();
	}
#endif /* CYGWIN */
}

// Set motor to run continuously at a given velocity. To stop, set velocity to 0.
//  This function cannot be used until moveAbsolute() has completed.
int stepper::velocity(const int f) {
	if (_superState == IDLE) {
		// Starting from f = 0.
		if (f == 0) {
			return SUCCESS;	// nothing to do
        } else if (f > 0) {
            directionPositive();
            a.frequency(a.fmin(), f);
        } else {
            // f < 0
            directionPositive(false);
            a.frequency(a.fmin(), -f);
        }
		_updateConstantVelocityStart();
        _superState = VELOCITY_MOVE;
		a.primeTime(1000000);
	} else if (_superState == VELOCITY_MOVE_ACCELERATE || _superState == VELOCITY_MOVE_DECELERATE) {
		// Can't do it right now. Try again when we're done accelerating.
	} else {
        // motor already running at a constant velocity
        assert(_superState == VELOCITY_MOVE);
        if ((f > 0 && !_directionPositive) || (f < 0 && _directionPositive)) {
            // Trying to change direction. Can't do this. Instead, bring motor to a stop and then issue another velocity() command to
            //  reverse direction.
            return ILLEGAL_VELOCITY;
        }
		int fdiff = f - a.freqFromClockTicks(_timerPeriod);
		if (fdiff == 0) {
			// nothing to do
			assert(_subState == VELOCITY_MOVE_CONSTANT_VELOCITY);
			return SUCCESS;
		} else if (fdiff > 0) {
			_subState = VELOCITY_MOVE_ACCELERATE;
			a.frequency(a.freqFromClockTicks(_timerPeriod), f);
		} else {
			// fdiff < 0
			_subState = VELOCITY_MOVE_DECELERATE;
			a.frequency(f, a.freqFromClockTicks(_timerPeriod));
		}
		_updateConstantVelocityStart();
	}
	_timerStart();
    return SUCCESS;
}

void stepper::_updateConstantVelocityStart(void) {
	unsigned int accelStep = a.timeToSteps(a.time());
	if (_directionPositive) {
		_positionConstantVelocityStart = _positionCurrent + accelStep;;
	} else {
		_positionConstantVelocityStart = _positionCurrent - accelStep;;
	}
	_subState = VELOCITY_MOVE_ACCELERATE;
}

// Set motor to run continuously at a given velocity. To stop, set velocity to 0.
//  This function cannot be used until velocity() has brought motor to a halt.
void stepper::moveAbsolute(int positionNew) {
	if (_superState != IDLE) {
		// Can't do it right now. Try again when we're done accelerating.
		return;
	}
	unsigned int positionDelta;
	_positionTarget = positionNew;
	if (_positionTarget > _positionCurrent) {
		directionPositive();
		positionDelta = _positionTarget - _positionCurrent;
	} else {
		directionPositive(false);
		positionDelta = _positionCurrent - _positionTarget;
	}
	// 0. Initialize timer
	// 1. Start timer
	// 2. Ramp up through acceleration curve
	// 3. Maintain max speed until time to decelerate
	// 4. Ramp down acceleration curve (run acceleration curve backwards)
	// 5. Stop timer.
	
	// 1. Set acceleration time (e.g. 0.5s)
	unsigned int accelStep = a.timeToSteps(a.time());
#if REGRESS_1
	oss() << "moveAbsolute() accelStep=" << accelStep << endl;
#endif /* REGRESS_1 */
	if (positionDelta > 2 * accelStep) {
		// Enough room for full acceleration profile
		if (_directionPositive) {
			_positionConstantVelocityStart = _positionCurrent + accelStep;
			_positionConstantVelocityEnd = _positionTarget - accelStep;
		} else {
			_positionConstantVelocityStart = _positionCurrent - accelStep;
			_positionConstantVelocityEnd = _positionTarget + accelStep;
		}
		_superState = MOVE_FULL;
		a.primeTime(1000000);
	} else {
		// Acceleration curve needs to be truncated. Not enough room to reach max speed.
		unsigned int tNew = a.stepsToTime(positionDelta / 2);
		// Set fMAX and then calculate time required for acceleration.
		unsigned int us = a.microSecToCurveIndex(tNew);
		_fminOld = a.fmin();
		_fmaxOld = a.fmax();
		unsigned int fmax = a.freqFromTime(us);
		a.frequency(_fminOld, fmax);	// Rebuilds acceleration tables. Put them back when this move is finished.
		// Set truncated acceleration time.
		a.primeTime(tNew);
		// Remember to rebuild accel tables when move is over. Set trigger to do this in isr().
		if (_directionPositive) {
			_positionConstantVelocityStart = _positionConstantVelocityEnd = _positionCurrent + positionDelta / 2;
		} else {
			_positionConstantVelocityStart = _positionConstantVelocityEnd = _positionCurrent - positionDelta / 2;
		}
		_superState = MOVE_TRUNCATED;
	}
	_subState = MOVE_START;
	_timerStart();
}

// Called every time timer times out.
void stepper::isr(void) {
	step();
	if (_superState == MOVE_FULL || _superState == MOVE_TRUNCATED) {
		if (_directionPositive) {
			if (_positionCurrent < _positionConstantVelocityStart) {
				// accelerating
				_subState = MOVE_ACCELERATE;
				_timer(a.updateClockPeriod());
			} else if (_positionCurrent < _positionConstantVelocityEnd) {
				// constant velocity. Nothing to do.
				_subState = MOVE_CONSTANT_VELOCITY;
			} else if (_positionCurrent == _positionConstantVelocityEnd) {
				// Start of deceleration.
				_subState = MOVE_DECELERATE;
				a.primeTime(a.time());
			} else if (_positionCurrent < _positionTarget) {
				// decelerating
				_subState = MOVE_DECELERATE;
				_timer(a.updateClockPeriodReverse());
			} else /* if (_positionCurrent >= _positionTarget) */ {
				// end of movement
				_timerStart(false);
			}
		} else {
			if (_positionCurrent > _positionConstantVelocityStart) {
				// accelerating
				_subState = MOVE_ACCELERATE;
				_timer(a.updateClockPeriod());
			} else if (_positionCurrent > _positionConstantVelocityEnd) {
				// constant velocity. Nothing to do.
				_subState = MOVE_CONSTANT_VELOCITY;
			} else if (_positionCurrent == _positionConstantVelocityEnd) {
				// Start of deceleration.
				_subState = MOVE_DECELERATE;
				a.primeTime(a.time());
			} else if (_positionCurrent > _positionTarget) {
				// decelerating
				_subState = MOVE_DECELERATE;
				_timer(a.updateClockPeriodReverse());
			} else /* if (_positionCurrent <= _positionTarget) */ {
				// end of movement
				_timerStart(false);
			}
		}
	} else if (_superState == VELOCITY_MOVE) {
		if (_directionPositive) {
			if (_positionCurrent < _positionConstantVelocityStart) {
				// accelerating
				_timer(a.updateClockPeriod());
			} else if (_positionCurrent == _positionConstantVelocityStart) {
				if (a.freqCloseToStop(a.freqFromClockTicks(_timer()))) {
					// end of movement
					_timerStart(false);
				} else {
					_subState = VELOCITY_MOVE_CONSTANT_VELOCITY;
				}
			// } else if (_positionCurrent >= _positionConstantVelocityStart) { // compiler could optimize this line out, but we'll do it just to be explicit
				// constant velocity. Nothing to do.
			}
		} else {
			if (_positionCurrent > _positionConstantVelocityStart) {
				// accelerating
				_timer(a.updateClockPeriod());
			} else if (_positionCurrent == _positionConstantVelocityStart) {
				if (a.freqCloseToStop(a.freqFromClockTicks(_timer()))) {
					// end of movement
					_timerStart(false);
				} else {
					_subState = VELOCITY_MOVE_CONSTANT_VELOCITY;
				}
			// } else if (_positionCurrent <= _positionConstantVelocityStart) {
				// constant velocity. Nothing to do.
			}
		}
	}
#if REGRESS_1
	switch (_superState) {
	case IDLE:
		oss() << "IDLE ";
		break;
	case MOVE_FULL:
		oss() << "MOVE_FULL ";
		break;
	case MOVE_TRUNCATED:
		oss() << "MOVE_TRUNCATED ";
		break;
	case VELOCITY_MOVE:
		oss() << "VELOCITY_MOVE ";
		break;
	default:
		oss() << "UNKNOWN SUPERSTATE ";
		break;
	}

	switch (_subState) {
	case MOVE_START:
		oss() << "MOVE_START ";
		break;
	case MOVE_ACCELERATE:
		oss() << "MOVE_ACCELERATE ";
		break;
	case MOVE_CONSTANT_VELOCITY:
		oss() << "MOVE_CONSTANT_VELOCITY ";
		break;
	case MOVE_DECELERATE:
		oss() << "MOVE_DECELERATE ";
		break;
	case VELOCITY_MOVE_ACCELERATE:
		oss() << "VELOCITY_MOVE_ACCEL ";
		break;
	case VELOCITY_MOVE_DECELERATE:
		oss() << "VELOCITY_MOVE_DECEL ";
		break;
	case VELOCITY_MOVE_CONSTANT_VELOCITY:
		oss() << "VELOCITY_MOVE_CONSTANT_VELOCITY ";
		break;
	default:
		oss() << "UNKNOWN SUBSTATE ";
		break;
	}
	oss() << " position=" << _positionCurrent << " timer=" << _timer();
	dump();
#endif /* REGRESS_1 */				
}

void stepper::_timerStart(bool start /* = true */) {
	if (start) {
		_timerRunning = true;
		TimerEnable(TIMER0_BASE, TIMER_A);
	} else {
		_timerRunning = false;
		TimerDisable(TIMER0_BASE, TIMER_A);
		if (_superState == MOVE_TRUNCATED) {
			// Reset acceleration tables
			a.frequency(_fminOld, _fmaxOld);
		}
		_superState = IDLE;
	}
}
#endif

int main(void) {
    interpret i;
    i.load();
#if CYGWIN
    i.dumpProgram();
    i.dumpSymbolTable();
#endif /* CYGWIN */ 
    i.run();
#if CYGWIN
    i.dumpSymbolTable();
#endif /* CYGWIN */ 
    i.run();
#if CYGWIN
    i.dumpSymbolTable();
#endif /* CYGWIN */ 
    return 0;
}