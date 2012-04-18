/*
	Copyright (c) Gareth Scott 2012

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
					logc(std::string("PARSETREEENTRY2"))
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
    case nodeVar:
        oss() << "nodeVar";
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
    default:
        oss() << "nodeUnknown";
        break;
    }
    oss() << " " << " " << value();
    dump();
}
#endif /* CYGWIN */
