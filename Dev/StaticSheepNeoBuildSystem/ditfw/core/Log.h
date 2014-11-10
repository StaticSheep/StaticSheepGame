/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#ifdef COMPILER_MSVC
#define LogPrintDebug OutputDebugStringA
#else
#define LogPrintDebug(x) printf("%s", x)
#endif

#include "API.h"
#include "Timer.h"
#include "StringExt.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <miniformat/miniformat.h>

#include <functional>
#include <vector>

namespace dit {

    //-----------------------------------//

    class Log
    {
    public:
        enum class Level : uint32
        {
            Debug
            , Error
        };

        struct Entry
        {
            float time;
            Level lvl;
            std::string msg;
            std::string msgLine;
        };

        typedef std::function<void(Entry*)> Handler;

    public:
        Log()
        {
            if (_DefaultLog == nullptr)
                _DefaultLog = this;
        }

        ~Log()
        {
            if (_DefaultLog == this)
                _DefaultLog = nullptr;
        }

        void Write(const char* msg, Level lvl)
        {
            boost::unique_lock<boost::mutex> lock{ mutex_ };

            entry.lvl = lvl;
            entry.time = timer.GetElapsed();

            mini::format(entry.msg, msg);
            mini::format(entry.msgLine, "%0\n", entry.msg.c_str());

            Dispatch();
        }

        template < typename T >
        void Write(const char* msg, Level lvl, const T& a0)
        {
            boost::unique_lock<boost::mutex> lock{ mutex_ };

            entry.lvl = lvl;
            entry.time = timer.GetElapsed();

            mini::format(entry.msg, msg, a0);
            mini::format(entry.msgLine, "%0\n", entry.msg.c_str());

            Dispatch();
        }

        template < typename FirstT, typename... TT >
        void Write(const char* msg, Level lvl, const FirstT& a0, const TT&... an)
        {
            boost::unique_lock<boost::mutex> lock{ mutex_ };

            entry.lvl = lvl;
            entry.time = timer.GetElapsed();

            mini::format(entry.msg, msg, a0, an...);
            mini::format(entry.msgLine, "%0\n", entry.msg.c_str());

            Dispatch();
        }

    public:

        boost::mutex mutex_;

        Timer timer;
        Entry entry;

        std::vector<Handler> errorHandlers;
        std::vector<Handler> debugHandlers;

    public:
        // helper function do not call
        void Dispatch()
        {
            switch (entry.lvl)
            {
            case Level::Debug:
                for (auto& fn : debugHandlers)
                    fn(&entry);
                break;
            case Level::Error:
                for (auto& fn : errorHandlers)
                    fn(&entry);
                break;
            };
        }

    public:
        static Log* _DefaultLog;
    };

    //-----------------------------------//
    // error logging overloads
    
    void log(const char* msg);

    template < typename T >
    void log(const char* msg, const T& a0)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Error, a0);
    }

    template < typename T, typename... TT >
    void log(const char* msg, const T& a0, const TT&... an)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Error, a0, an...);
    }

    //-----------------------------------//
    // debug logging overloads

    void log_dbg(const char* msg);

    template < typename T >
    void log_dbg(const char* msg, const T& a0)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Debug, a0);
    }

    template < typename T, typename... TT >
    void log_dbg(const char* msg, const T& a0, const TT&... an)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Debug, a0, an...);
    }

    //-----------------------------------//

}