/*
	Copyright (c) Gareth Scott 2014

	profiler.h 

*/

#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <windows.h>
#include <sys/time.h>

#include <string>
#include <sstream>
#if CYGWIN
#include "log.h"

using namespace std;
#endif /* CYGWIN */

class profiler
#if CYGWIN
				: public logc
#endif /* CYGWIN */
				{
public:
    profiler(float& microsec) : logc(std::string("PROFILER")), _microsec(microsec) {
        //gettimeofday(&_timevalBegin, NULL);   // These don't seem to work well under Windows running under Cygwin
        QueryPerformanceFrequency(&_freq);
        QueryPerformanceCounter(&_tBegin);
    }
    ~profiler() {
        //sleep(0.2);
        //gettimeofday(&_timevalEnd, NULL);
        //_microsec += (_timevalEnd.tv_sec - _timevalBegin.tv_sec) * 1000000 + _timevalEnd.tv_usec - _timevalBegin.tv_usec;
        QueryPerformanceCounter(&_tEnd);
        _microsec += (_tEnd.QuadPart - _tBegin.QuadPart) * 1000.0 / _freq.QuadPart;
#if CYGWIN
        oss() << endl << "profiler() usec: " << _microsec;
        //oss() << endl << "profiler() microsec total: " << _microsec << " End.tv_usec: " << _timevalEnd.tv_usec << " Begin.tv_usec: " << _timevalBegin.tv_usec;
        dump();
#endif /* CYGWIN */                
    }
    
private:
    _LARGE_INTEGER _freq;
    _LARGE_INTEGER _tBegin;
    _LARGE_INTEGER _tEnd;
    //struct timeval _timevalBegin;
    //struct timeval _timevalEnd;
    float& _microsec;
};

#endif /* _PROFILER_H_ */