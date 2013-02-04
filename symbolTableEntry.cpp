/*
	Copyright (c) Gareth Scott 2012, 2013

	symbolTableEntry.cpp 

*/

#include "symbolTableEntry.h"
#include <assert.h>

symbolTableEntry::symbolTableEntry() :
#if CYGWIN 
					logc(std::string("SYMBOLTABLEENTRY"))
#endif /* CYGWIN */					
                    , _type(nodeInvalid) {
}

symbolTableEntry::symbolTableEntry(nodeType t, int value) :
#if CYGWIN 
					logc(std::string("SYMBOLTABLEENTRY2"))
#endif /* CYGWIN */					
					, _type(t), _value(value) {
}

symbolTableEntry& symbolTableEntry::operator=(const symbolTableEntry& p) {
	if (this != &p) {
		type(p.type());
        value(p.value());
	}
	return *this;
}

bool symbolTableEntry::operator==(symbolTableEntry& p) const {
    if (this == &p) {
        return true;
    }
    if (p.type() == type() && p.value() == value()) {
        return true;
    }
    return false;
}

#if CYGWIN
void symbolTableEntry::dumpEntry(void) {
    switch (type()) {
    case nodeVariable:
        oss() << "nodeVariable";
        break;
    case nodeConst:
        oss() << "nodeConst";
        break;
    case nodeOperator:
        oss() << "nodeOperator";
        break;
    case nodeInvalid:
        oss() << "nodeInvalid";
        break;
    case nodeTemporary:
        oss() << "nodeTemporary";
        break;
    case nodeFunctionCall:
        oss() << "nodeFunctionCall";
        break;
    case nodeFunctionDefinition:
        oss() << "nodeFunctionDefinition";
        break;
    case nodeArgumentValue:
        oss() << "nodeArgumentValue";
        break;
    case nodeArgumentReference:
        oss() << "nodeArgumentReference";
        break;
    default:
        oss() << "ERROR symbolTableEntry nodeUnknown:" << type();
        break;
    }
    oss() << " " << " " << value();
    dump();
}
#endif /* CYGWIN */
