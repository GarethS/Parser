/*
	Copyright (c) Gareth Scott 2012

	parseTreeEntry.cpp 

    Each node in the parse tree is made up of one of these objects.
    
*/

#include "parseTreeEntry.h"
#include <assert.h>

parseTreeEntry::parseTreeEntry() :
#if CYGWIN 
					logc(std::string("PARSETREEENTRY"))
#endif /* CYGWIN */					
                    , _type(nodeInvalid) {
}

parseTreeEntry::parseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level) :
#if CYGWIN 
					logc(std::string("PARSETREEENTRY2"))
#endif /* CYGWIN */					
					, _type(t), _position(p), _level(level), _value(value) {
}

parseTreeEntry::parseTreeEntry(nodeType t, int value, unsigned int level) :
#if CYGWIN 
					logc(std::string("PARSETREEENTRY3"))
#endif /* CYGWIN */					
					, _type(t), _level(level), _value(value) {
}

parseTreeEntry& parseTreeEntry::operator=(const parseTreeEntry& p) {
	if (this != &p) {
		type(p.type());
        position(p.position());
        level(p.level());
        value(p.value());
	}
	return *this;
}

bool parseTreeEntry::operator==(parseTreeEntry& p) const {
    if (this == &p) {
        return true;
    }
    if (p.type() == type() && p.position() == position() && p.value() == value() && p.level() == level()) {
        return true;
    }
    return false;
}

#if CYGWIN
void parseTreeEntry::dumpEntry(void) {
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
    case nodeStartAction:
        oss() << "nodeStartAction";
        break;
    default:
        oss() << "ERROR nodeUnknown";
        break;
    }
    oss() << " " << level() << " " << value();
    dump();
}
#endif /* CYGWIN */
