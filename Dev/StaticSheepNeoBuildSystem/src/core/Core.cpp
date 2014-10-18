/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "Core.h"
#include "Entity.h"
#include "String.h"

#include <FileWatcher/FileWatcherWin32.h>
#include <boost/filesystem.hpp>

namespace dit {

    //-----------------------------------//

    Core* Core::s_Instance = nullptr;

    //-----------------------------------//
    
    Core* core()
    {
        return Core::Instance();
    }

    //-----------------------------------//

    Core::Core()
    {
        if (s_Instance == nullptr)
            s_Instance = this;

        // init the logger / forward errors to stdout
        auto fn = [](Log::Entry* e) { printf("%s", e->msgLine.c_str()); };
        logger.errorHandlers.push_back(std::move(fn));
    }

    //-----------------------------------//

    Core::~Core()
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }

    //-----------------------------------//

    void Core::Init()
    {
        // create the root event pump
        root = new Entity{};
        root->Init();

        // log the cwd
        auto cwd = boost::filesystem::current_path();
        log("cwd: \"%0\"", cwd.string().c_str());
    }

    //-----------------------------------//

    void Core::Update()
    {
        // check for file changes from os
        fileWatcher.Update();

        // process any update messages
        root->Update(timer.GetCurrent() - lastFrameTime);
        lastFrameTime = timer.GetElapsed();
    }

    //-----------------------------------//

    void Core::Shutdown()
    {
        // destroy the root entity
        root->Shutdown();
        delete root;
        root = nullptr;
    }

}