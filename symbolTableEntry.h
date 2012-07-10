/*
	Copyright (c) Gareth Scott 2012

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

#define SYMBOL_TABLE_NAME_LEN   (32)

class symbolTableEntry
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    symbolTableEntry();
    symbolTableEntry(nodeType t, int value);
    ~symbolTableEntry() {}
    
#if CYGWIN
    void dumpEntry(void);
#endif /* CYGWIN */
    symbolTableEntry& operator=(const symbolTableEntry& p);
    bool operator==(symbolTableEntry& p) const ;
    
    int value(void) const {return _value;}
    void value(const int v) {_value = v;}
    
    nodeType type(void) const {return _type;}
    void type(const nodeType t) {_type = t;}
    
private:
    nodeType _type; // Either a variable or a constant
    //char _name[SYMBOL_TABLE_NAME_LEN];
    int _value;
    float _valueFloat;
};    

#endif /* _SYMBOLTABLEENTRY_H_ */