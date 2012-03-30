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