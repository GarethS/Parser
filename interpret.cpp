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
#include <assert.h>

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRET"))
#endif /* CYGWIN */					
					//_positionCurrent(0), _directionPositive(true), _timerRunning(false), _superState(IDLE) {
                    {
    for (int i = 0; i < MAX_PROGRAM_ENTRY; ++i) {
        _program[i].type(nodeInvalid);
    }
}

void interpret::run(void) {
    _programIndex = 0;
    while (_program[_programIndex].type() != nodeInvalid) {
        switch (_currentNodeType()) {
        case nodeVar:
        case nodeConst:
            // Do short-circuit optimization. If left-hand value TRUE, no need to evaluate right-hand side.
            // To implement this, before pushing the RHS, look to see if the operator one level
            //  above is OR. If it is and LHS is TRUE
            // If left-hand value and true and node operator at level-1 == OR then
            //  _programIndex = index of node operator + 1 (making sure not to run off end of program if that's the end)
            //  Need to look this up for operators too once it's been done for one variable!
            _evaluationStack.push_front(_currentNodeValue());
            break;
        case nodeOperator:
            evaluate(_currentNodeValue());
            break;
        default:
            assert(false);
            break;
        }
    }
}

int interpret::_findParseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level) {
    parseTreeEntry pte(t, p, value, level);
    for (unsigned int i = _programIndex; i < _programIndexMax; ++i) {
        if (_program[i] == pte) {
            return true;
        }
    }
    return NOT_FOUND;
}

void interpret::evaluate(unsigned int op) {
    switch (op) {
    case BANG:
        assert(_evaluationStack.size() >= 1);
        _evaluationStack.push_front(!_value());
        break;
    case PLUS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() + _value());
        break;
    case MINUS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() - _value());
        break;
    case MULT:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() * _value());
        break;
    case DIV:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() / _value());
        break;
    case XOR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() ^ _value());
        break;
    case GEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() >= _value()/* ? TRUE : FALSE*/);
        break;
    case LEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() <= _value());
        break;
    case NEQ:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() != _value());
        break;
    case GTR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() > _value());
        break;
    case LSS:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() < _value());
        break;
    case AND:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() && _value());
        break;
    case OR:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() || _value());
        break;
    case TEST_FOR_EQUAL:
        assert(_evaluationStack.size() >= 2);
        _evaluationStack.push_front(_value() == _value());
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