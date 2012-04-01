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
    parseTreeEntry(nodeType t, int value, unsigned int level);
    ~parseTreeEntry() {}
    
    parseTreeEntry& operator=(const parseTreeEntry& p);
    bool operator==(parseTreeEntry& p) const ;
    
    unsigned int level(void) const {return _level;}
    void level(const unsigned int l/* l, not 1*/) {_level = l;}

    int value(void) const {return _value;}
    void value(const int v) {_value = v;}
    
    nodeType type(void) const {return _type;}
    void type(const nodeType t) {_type = t;}
    
    nodePosition position(void) const {return _position;}
    void position(const nodePosition position) {_position = position;}
        
private:
    nodeType _type; // What kind of entry we are
    nodePosition _position; // Left or right of the next highest node? // TODO: Do we really need this?
    unsigned int _level; // Root = 0, children = 1, grandchildren = 2, etc.
    unsigned int _value; // If operator, its value; if variable, its symbol table index
};    

#endif /* _PARSETREEENTRY_H_ */