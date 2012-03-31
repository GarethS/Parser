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
					//_positionCurrent(0), _directionPositive(true), _timerRunning(false), _superState(IDLE) {
                    {
}

parseTreeEntry::parseTreeEntry(nodeType t, nodePosition p, int value, unsigned int level) :
#if CYGWIN 
					logc(std::string("PARSETREEENTRY"))
#endif /* CYGWIN */					
					, _type(t), _position(p), _level(level), _value(value) {
}

bool parseTreeEntry::operator==(parseTreeEntry& p) {
    if (this == &p) {
        return true;
    }
    if (p.type() == type() && p.position() == position() && p.value() == value() && p.level() == level()) {
        return true;
    }
    return false;
}
