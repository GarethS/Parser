/*
	Copyright (c) Gareth Scott 2012, 2013

	symbolTableEntry.h 

*/

#ifndef _SYMBOLTABLEENTRY_H_
#define _SYMBOLTABLEENTRY_H_

#include "compilerHelper.h"

#if CYGWIN
#include "log.h"
#include <sstream>

using namespace std;
#endif /* CYGWIN */

//#define SYMBOL_TABLE_NAME_LEN   (32)

class symbolTableEntry
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    symbolTableEntry();
    symbolTableEntry(nodeType t, int value);
    symbolTableEntry(nodeType t, int value, int fcnLink);
    ~symbolTableEntry() {}
    
    void dumpEntry(void);
    symbolTableEntry& operator=(const symbolTableEntry& p);
    bool operator==(symbolTableEntry& p) const ;
    
    int value(void) const {return _value;}
    void value(const int v) {_value = v;}
    
    nodeType type(void) const {return _type;}
    void type(const nodeType t) {_type = t;}
    
    int fcnLink(void) const {return _fcnLink;}
    void fcnLink(const int f) {_fcnLink = f;}
    
private:
    nodeType _type; // Either a variable or a constant
    //char _name[SYMBOL_TABLE_NAME_LEN];
    int _value;
    float _valueFloat;
    int _fcnLink;
};    

#endif /* _SYMBOLTABLEENTRY_H_ */