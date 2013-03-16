/*
	Copyright (c) Gareth Scott 2012, 2013

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
//#include <math.h>
//#include <complex>
#endif /* CYGWIN */
#include <assert.h>

static int moveAbsolute(int position) {
    printf("moveAbsolute(%d)\n", position);
    fflush(stdout);
    return 27;
}

interpret::interpret() :
#if CYGWIN 
					logc(std::string("INTERPRETER"))
#endif /* CYGWIN */		
                    , _programIndex(PROGRAM_INDEX_START), _symbolTableIndex(0), _bp(PROGRAM_INDEX_START), _evaluatingPattern(true) {
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
    //_loadTree(string("patternTree.txt"));
    //_loadTree(string("actionTree.txt"));
    _loadTree(string("tree.txt"));
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
    // Typical line reads: '3 LEFT Variable 0'
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
        unsigned int level;
        string leftRight;
        string variableOperator;
        int value;
        iss >> level >> leftRight >> variableOperator >> value;
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
        } else if (variableOperator == "Do") {
            pte.type(nodeDo);
        } else if (variableOperator == "JmpEndIf") {
            pte.type(nodeJmpEndIf);
        } else if (variableOperator == "Start") {
            pte.type(nodeFunctionDefinition);
        } else if (variableOperator == "ProgramEnd") {
            pte.type(nodeProgramEnd);
        } else {
            oss() << endl << "ERROR:unrecognized variableOperator:" << variableOperator;
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
    // Typical line to read: '1 4 -1', where 1 = nodeType, 4 = value and -1 is funcion link. In this case it's a constant with value = 4 and no function link.
    //  Note that function linking is only needed by function calls and function definitions.
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
        unsigned int type;
        int value;
        int fcnLink;
        iss >> type >> value >> fcnLink;
        symbolTableEntry ste((nodeType)type, value, fcnLink);
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
    for (_programIndex = 0; _program[_programIndex].type() != nodeInvalid; ++_programIndex) {
#if CYGWIN
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
            evaluate(_currentProgramNodeValue());
            //_shortCircuitOptimization();
            break;
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
            oss() << endl << "***nodeFunctionDefinition _programIndex:" << _programIndex ;
            dump();
            if (_programIndex == PROGRAM_INDEX_START) {
                // Entering main
                ++_programIndex;
            } else {
                // End of function, time to return from whence we came. In other words, hitting a function definition means we've reached the end of the current function.
                {
                    // TODO: This is the time to return any nodeTemporary symbols on the _evaluationStack to being marked as nodeUnused
                    unsigned int nodeFunctionReturnIndex = _evaluationStack.peekAtIndex(_bp+1); 
                    //oss() << endl << "!!!nodeFunctionReturnIndex:" << nodeFunctionReturnIndex;
                    //dump();
                    _programIndex = _symbolTable[nodeFunctionReturnIndex].value();
                    
                    _evaluationStack.stackFrameIndex(_bp);   // mov sp, bp
                    unsigned int bpIndex = _evaluationStack.front();
                    _bp = _symbolTable[bpIndex].value();
                    _evaluationStack.pop_front();
                    
                    _evaluationStack.pop_front();   // pop the function return index
                }
            }
            break;
        case nodeIfEval0:
            // Result of nodeIf now sits on top of evaluation stack. If it's 0
            //  jump to nodeElse of that same index.
            // Get value from top of _evaluationStack
            
            //_programIndex = _currentProgramNodeValue();
            //int ifEval0Value = _evalValuePeek();
            if (_symbolTable[_evalValue()].value()) {
                ++_programIndex;
            } else {
#if CYGWIN
                //oss() << endl << "!!nodeIfEval0 _currentProgramNodeValue():" << _currentProgramNodeValue() << " _currentProgramNodeLevel()" << _currentProgramNodeLevel() ;
                //dump();
#endif /* CYGWIN */                
                parseTreeEntry pte(nodeElse, _currentProgramNodeValue(), _currentProgramNodeLevel() );
                int newProgramIndex = _findFirstParseTreeEntry(pte);
                if (newProgramIndex == NOT_FOUND) {
                    assert(newProgramIndex != NOT_FOUND);
                } else {
                    _programIndex = newProgramIndex; // Jump one past nodeIfEval0
                }
            }
#if CYGWIN
            //oss() << endl << "!!nodeIfEval0 _programIndex:" << _programIndex;
            //dump();
#endif /* CYGWIN */                
            break;
        case nodeJmpEndIf:
            {
                parseTreeEntry pte(nodeEndif, _currentProgramNodeValue(), _currentProgramNodeLevel() );
                int newProgramIndex = _findFirstParseTreeEntry(pte);
                if (newProgramIndex == NOT_FOUND) {
                    assert(newProgramIndex != NOT_FOUND);
                } else {
                    _programIndex = newProgramIndex;
                }
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
                        intrinsicReturnValue = moveAbsolute(_symbolTable[symbolIndexParameter1].value());
                        break;
                    case INTRINSIC_FCN_DEFN_MOVE_RELATIVE:
                        break;
                    case INTRINSIC_FCN_DEFN_SLEEP:
                        break;
                    default:
                        break;
                    }
                    _symbolTable[symbolIndexReturnValue].value(intrinsicReturnValue);

                    _evaluationStack.stackFrameIndex(_bp);   // mov sp, bp
                    unsigned int bpIndex = _evaluationStack.front();
                    _bp = _symbolTable[bpIndex].value();
                    _evaluationStack.pop_front();
                    
                    _evaluationStack.pop_front();   // pop the function return index
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
#if CYGWIN
        //_program[i].dumpEntry();
#endif /* CYGWIN */    
        if (_program[i].type() == p.type() && _program[i].level() == p.level()) {
            // TODO: Combine this with above 'if' statement.
            if (_program[i].value() == p.value()) {
                return i;
            }
            continue;
            //return NOT_FOUND;
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

// Normally symbolTableIndex will return itself, however, for function arguments the symbolTableIndex
//  is pointed to by the entry in the stack frame. That's where the morphing comes in.
int interpret::symbolTableIndexMorph(const int symbolTableIndex) {
    //int symbolTableIndex = _evalValue();
    int symbolTableIndexReturn = symbolTableIndex;
    unsigned int functionArgumentIndex;
    switch (_symbolTable[symbolTableIndex].type()) {
    case nodeVariable:
    case nodeConst:
        //return symbolTableIndexReturn;
        break;
    case nodeArgumentValue:
    case nodeArgumentReference:
        functionArgumentIndex = _symbolTable[symbolTableIndex].value();
        // Now get index off the stack frame
        symbolTableIndexReturn = _evaluationStack.peekAtIndex(_bp - functionArgumentIndex - 1); // First parameter starts at: _bp - 1; second at _bp -2, etc.
#if CYGWIN
        oss() << " functionArgumentIndex:" << functionArgumentIndex << " symbolTableIndex:" << symbolTableIndex;
#endif /* CYGWIN */
        //return symbolTableIndexReturn;
        break;
    default:
        assert(false);
        break;
    }
    // Almost certainly not what we want to do but not sure what to do at this point.
    return symbolTableIndexReturn;
}

void interpret::evaluate(unsigned int op) {
    assert(_evaluationStack.size() >= 1);
    // Need to determine the type and then decide what to do with it. See: case EQUAL 
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
    case BITWISEAND:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs & rhs);
#if CYGWIN
        oss() << lhs << " & " << rhs;
#endif /* CYGWIN */
        break;
    case BITWISEOR:
        assert(_evaluationStack.size() >= 1);
        lhs = _symbolTable[_evalValue()].value();
        _pushSymbolOnEvaluationStack(lhs | rhs);
#if CYGWIN
        oss() << lhs << " | " << rhs;
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
#if 1
            unsigned int functionArgumentIndex, symbolTableIndex;
            switch (_symbolTable[leftHandSymbolTableIndex].type()) {
            case nodeConst:
                // It's a constant so don't modify it's value
                assert(_symbolTable[leftHandSymbolTableIndex].type() != nodeConst);
                break;
            case nodeVariable:
                _symbolTable[leftHandSymbolTableIndex].value(rhs);
                break;
            case nodeArgumentValue:
                //break;
            case nodeArgumentReference:
                functionArgumentIndex = _symbolTable[leftHandSymbolTableIndex].value();
                // Now get index off the stack frame
                symbolTableIndex = _evaluationStack.peekAtIndex(_bp - functionArgumentIndex - 1); // First parameter starts at: _bp - 1; second at _bp -2, etc.
#if CYGWIN
                oss() << " functionArgumentIndex:" << functionArgumentIndex << " symbolTableIndex:" << symbolTableIndex;
#endif /* CYGWIN */ 
                _symbolTable[symbolTableIndex].value(rhs);
                break;
            default:
                assert(false);
                break;
            }
#else            
            if (_symbolTable[leftHandSymbolTableIndex].type() == nodeConst) {
                // It's a constant so don't modify it's value
                assert(_symbolTable[leftHandSymbolTableIndex].type() != nodeConst);
            } else {
                _symbolTable[leftHandSymbolTableIndex].value(rhs);
            }
#endif            
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
#if CYGWIN
        oss() << "ERROR interpret::evaluate, op=" << op;
#endif /* CYGWIN */    
        //assert(false);
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
#if 0
#define PI  (3.1415926535897932384626433832795)
    cout << "PI * PI:" << PI * PI << endl;
    cout << "PI^2:" << pow(PI,2) << endl;

    std::complex<float> localComplex1(std::polar(1.23, 0.2768 + (PI * 1.0)));
    cout << "localComplex1:" << localComplex1 << endl;
    std::complex<float> localComplex2(std::polar(1.23, 0.2768 + (PI * 2.0)));
    cout << "localComplex2:" << localComplex2 << endl;
    std::complex<float> localComplex3(std::polar(1.23, 0.2768 + (PI * 3.0)));
    cout << "localComplex3:" << localComplex3 << endl;
    std::complex<float> localComplex4(std::polar(1.23, 0.2768 + (PI * 4.0)));
    cout << "localComplex4:" << localComplex4 << endl;
    return 0;
#endif
    
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