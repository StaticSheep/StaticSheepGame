/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "Log.h"

namespace dit {

    //-----------------------------------//

    Log* Log::_DefaultLog = nullptr;

    //-----------------------------------//

    void log(const char* msg)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Error);
    }

    //-----------------------------------//

    void log_dbg(const char* msg)
    {
        auto log = Log::_DefaultLog;
        if (log == nullptr)
            return;

        log->Write(msg, Log::Level::Debug);
    }

    //-----------------------------------//

}