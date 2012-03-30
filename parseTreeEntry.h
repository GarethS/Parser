/*
	Copyright (c) Gareth Scott 2012

	parseTreeEntry.h 

*/

#ifndef _PARSETREEENTRY_H_
#define _PARSETREEENTRY_H_

#include "compilerHelper.h"

#if CYGWIN
//#include <iostream>
#include "log.h"
#include <sstream>

using namespace std;
#endif /* CYGWIN */

class parseTreeEntry
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    parseTreeEntry();
    ~parseTreeEntry() {}
    
    unsigned int level(void) {return _level;}
    void level(unsigned int level) {_level = level;}

    unsigned int value(void) {return _value;}
    void value(unsigned int value) {_value = value;}
    
    nodeType type(void) {return _type;}
    void type(nodeType type) {_type = type;}
    
private:
    unsigned int _level; // Root = 0, children = 1, grandchildren = 2, etc.
    unsigned int _value; // If operator, its value; if variable, its symbol talbe index
    nodeType _type; // What kind of entry we are
};    

#endif /* _PARSETREEENTRY_H_ */