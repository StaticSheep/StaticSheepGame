/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "Timer.h"

#if defined(PLATFORM_MACOSX)
#pragma TODO("OSX: Use Mach timers: http://developer.apple.com/library/mac/#qa/qa2004/qa1398.html")
#elif defined(PLATFORM_NACL)
#include <ppapi/c/pp_time.h>
#elif defined(PLATFORM_LINUX)
#include <sys/time.h>
#define tv_time_ms(t) ((t.tv_sec * 1000000.0) + t.tv_usec)
#pragma TODO("Linux: http://linux.die.net/man/3/clock_gettime")
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#endif

namespace dit {

    //-----------------------------------//

#ifdef PLATFORM_WINDOWS

    static bool CheckHighResolution();

    static int64 g_TicksPerSecond;
    static int g_HighResolutionSupport = CheckHighResolution();

    static bool CheckHighResolution()
    {
        LARGE_INTEGER* frequency = (LARGE_INTEGER*) &g_TicksPerSecond;
        return QueryPerformanceFrequency(frequency) != 0;
    }

    static int64 GetTime()
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        return time.QuadPart;
    }

#elif defined(PLATFORM_NACL)

    PP_Time GetTime();

#elif PLATFORM_LINUX

    static timeval GetTime()
    {
        timeval time;
        gettimeofday(&time, nullptr);
        return time;
    }

#endif

    //-----------------------------------//

    Timer::Timer()
    {
        Reset();
    }

    //-----------------------------------//

    float Timer::GetCurrent()
    {
#ifdef PLATFORM_WINDOWS
        int64 time = GetTime();
        return float(time) / float(g_TicksPerSecond);
#elif defined(PLATFORM_NACL)
        PP_Time time = GetTime();
        return time;
#else
        timeval time = GetTime();
        return tv_time_ms(time);
#endif
    }

    //-----------------------------------//

    void Timer::Reset()
    {
        time = GetTime();
    }

    //-----------------------------------//

    float Timer::GetElapsed()
    {
#ifdef PLATFORM_WINDOWS
        int64 diff = GetTime() - time;
        return float(diff) / float(g_TicksPerSecond);
#elif defined(PLATFORM_NACL)
        int64 diff = GetTime() - time;
        return diff;
#else
        return tv_time_ms(currentTime) - tv_time_ms(lastTime.tv_sec);
#endif
    }

    //-----------------------------------//

}