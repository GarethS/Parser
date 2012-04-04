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
#include <string>
#include <istream>
#endif /* CYGWIN */
#include <assert.h>

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRET"))
#endif /* CYGWIN */					
					//_positionCurrent(0), _directionPositive(true), _timerRunning(false), _superState(IDLE) {
                    {
    // TODO: Really need this? Doesn't constructor take care of initializing everything to nodeInvalid?
    for (int i = 0; i < MAX_PROGRAM_ENTRY; ++i) {
        _program[i].type(nodeInvalid);
    }
}

#if CYGWIN
void interpret::load(void) {
    ifstream ifs("parseTree.txt");
    if (!ifs) {
        return;
    }
    // Typical line to read: '3 LEFT Variable 0'
    unsigned int level;
    string leftRight;
    string variableOperator;
    int value;
    while (!ifs.eof()) {
        ifs >> level >> leftRight >> variableOperator >> value;
    }
    ifs.close();
}
#endif /* CYGWIN */

void interpret::run(void) {
    for (_programIndex = 0; _program[_programIndex].type() != nodeInvalid; ++_programIndex) {
        switch (_currentProgramNodeType()) {
        case nodeVar:
        case nodeConst:
            _evaluationStack.push_front(_currentProgramNodeValue());
            _shortCircuitOptimization();
            break;
        case nodeOperator:
            evaluate(_currentProgramNodeValue());
            _shortCircuitOptimization();
            break;
        default:
            assert(false);
            break;
        }
    }
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
    switch (op) {
    case BANG:
        assert(_evaluationStack.size() >= 1);
        _evaluationStack.push_front(!_evalValue());
        break;
    case PLUS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() + _evalValue());
        break;
    case MINUS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() - _evalValue());
        break;
    case MULT:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() * _evalValue());
        break;
    case DIV:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() / _evalValue());
        break;
    case XOR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() ^ _evalValue());
        break;
    case GEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() >= _evalValue()/* ? TRUE : FALSE*/);
        break;
    case LEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() <= _evalValue());
        break;
    case NEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() != _evalValue());
        break;
    case GTR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() > _evalValue());
        break;
    case LSS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() < _evalValue());
        break;
    case AND:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() && _evalValue());
        break;
    case OR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() || _evalValue());
        break;
    case TEST_FOR_EQUAL:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_evalValue() == _evalValue());
        break;
    default:
        assert(false);
        break;
    }
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
    return 0;
}