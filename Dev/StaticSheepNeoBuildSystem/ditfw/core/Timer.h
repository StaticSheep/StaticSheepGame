/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"

namespace dit {

    //-----------------------------------//

    class Timer
    {
    public:

        Timer();

        void Reset();

        // get elapsed time since timer creation or last reset.
        float GetElapsed();
        float GetCurrent();

        int64 time; //!< time of timer creation
    };

    //-----------------------------------//
}