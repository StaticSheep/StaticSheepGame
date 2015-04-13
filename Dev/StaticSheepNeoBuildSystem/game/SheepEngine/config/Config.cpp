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

//#include "WxWindow.h"
#include "systems/editor/GizmoEditor.h"
#include "systems/metrics/MetricController.h"

//#include "gfx/wxw/d3d/WxD3DCanvas.h"
//#include "gfx/wxw/d3d/WxD3DContext.h"

static bool editor;
static bool vsync = true;

namespace Framework
{
  Engine* AllocateEngine(int argc, char** argv)
  {
    Engine* Core = new Engine();
    
#ifdef _DEBUG
    editor = true;
    OpenConsole();
#else

#if PRESENTATION
    editor = true;
#endif
    if(argc > 1)
    {
      for(int i = 1; i < argc; ++i)
      {
        if(strcmp(argv[i], "-editor")  == 0)
          editor = true;

        if (strcmp(argv[i], "-console") == 0)
          OpenConsole();

        if (strcmp(argv[1], "-novsync") == 0)
          vsync = false;
      }
    }

    /* if (editor)
       OpenConsole();*/
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

    SheepGraphics* gp = new SheepGraphics();
    gp->m_vsync = vsync;
    Core->AddSystem(gp);
    
    Core->AddSystem(new Debug());
    if(editor)
    {
      
      Core->AddSystem(new AntTweakModule());
      Core->AddSystem(new GizmoEditor());
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
      //ENGINE->LoadLuaLevel("content/lua/engine/lua_levels/uisandbox.lua");
      ENGINE->OpenEditor();

     // ENGINE->ChangeLevel("Asteroid");
    }

    ENGINE->SystemMessage(Message(Message::EngineReady));

#if PRESENTATION
    editor = false;
    ENGINE->PlayInEditor(true);
#endif

    /*GameSpace* sp = FACTORY->LoadSpace("AMFA");
    sp->SetPaused(true);*/

    if (!editor)
    {
      //ShowCursor(false);
      //ENGINE->ChangeLevel("Intro");
      ENGINE->ChangeLevel("Intro");
    }
	  //ENGINE->LoadLevel("content/data/spaces/Level1.space");
#endif

    
    
  }


}