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
    parseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level);
    ~parseTreeEntry() {}
    
    bool operator==(parseTreeEntry& p);
    
    const unsigned int level(void) {return _level;}
    void level(unsigned int level) {_level = level;}

    const int value(void) {return _value;}
    void value(int value) {_value = value;}
    
    nodeType type(void) {return _type;}
    void type(nodeType type) {_type = type;}
    
    const nodePosition position(void) {return _position;}
    void position(nodePosition position) {_position = position;}
        
private:
    nodeType _type; // What kind of entry we are
    nodePosition _position; // Left or right of the next highest node?
    unsigned int _level; // Root = 0, children = 1, grandchildren = 2, etc.
    unsigned int _value; // If operator, its value; if variable, its symbol talbe index
};    

#endif /* _PARSETREEENTRY_H_ */