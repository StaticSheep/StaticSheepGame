/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "Timer.h"
#include "Log.h"
#include "ThreadPool.h"

#include <FileWatcher/FileWatcher.h>

namespace dit {

    //-----------------------------------//

    class Entity;

    //-----------------------------------//

    class Core
    {
    public:
        Core();
        ~Core();

        void Init();
        void Update();
        void Shutdown();

    public:
        Timer       timer;
        Log         logger;
        ThreadPool  threads = { 8 };
        FileWatcher fileWatcher;

        Entity* root = nullptr;

        float lastFrameTime;

        // static access
    public:
        static Core* Instance() { return s_Instance; }
    private:
        static Core* s_Instance;
    };

    //-----------------------------------//

    Core* core();

    //-----------------------------------//

}