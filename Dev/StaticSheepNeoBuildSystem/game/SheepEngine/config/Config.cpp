/*****************************************************************
Filename: Config.cpp
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"

#include "Config.h"

//#include "wxeditor/App.h"

#include "engine/core/Engine.h"

#include "systems/input/Input.h"
#include "systems/debug/Debug.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/audio/SheepAudio.h"
#include "systems/gamelogic/GameLogic.h"
#include "systems/physics/SheepPhysics.h"
#include "systems/anttweak/AntTweakModule.h"
#include "systems/skynet/Skynet.h"

#include "WxWindow.h"
#include "systems/metrics/MetricController.h"

//#include "gfx/wxw/d3d/WxD3DCanvas.h"
//#include "gfx/wxw/d3d/WxD3DContext.h"

static bool editor;

namespace Framework
{
  Engine* AllocateEngine(int argc, char** argv)
  {
    Engine* Core = new Engine();
    
#ifdef _DEBUG
    editor = true;
    OpenConsole();
#else
    if(argc > 1)
    {
      for(int i = 1; i < argc; ++i)
      {
        if(strcmp(argv[i], "-editor")  == 0)
          editor = true;
      }
    }

    if (editor)
      OpenConsole();
#endif

    Core->AddSystem(new InputManager());
    Core->AddSystem(new Skynet());
    Core->AddSystem(new GameLogic());
    Core->AddSystem(new SheepPhysics());
    Core->AddSystem(new SheepAudio());
    Core->AddSystem(new MetricController());

    void* rc = nullptr;

#if USE_EDITOR
    //rc = ((dit::App*)dit::EDITOR_WINDOW)->window->canvas->context->GetRenderContext();
#endif

    Core->AddSystem(new SheepGraphics());
    

    if(editor)
    {
      Core->AddSystem(new Debug());
      Core->AddSystem(new AntTweakModule());
    }

#if USE_EDITOR

#else
	Core->MakeWindow(GetModuleHandle(NULL), 1, !editor);
#endif
    return Core;
  }

  void InitEngine(void)
  {
    ENGINE->Initialize();
    

#if USE_EDITOR
#else

    if (editor)
    {
      ENGINE->LoadLuaLevel("content/lua/engine/lua_levels/uisandbox.lua");
      ENGINE->OpenEditor();
    }

    ENGINE->SystemMessage(Message(Message::EngineReady));

    if (!editor)
    {
      ENGINE->ChangeLevel("Asteroid");
    }
	  //ENGINE->LoadLevel("content/data/spaces/Level1.space");
#endif

    
    
  }


}