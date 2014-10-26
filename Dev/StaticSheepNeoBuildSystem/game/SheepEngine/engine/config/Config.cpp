#include "pch/precompiled.h"

#include "Config.h"

#include "editor/App.h"

#include "engine/core/Engine.h"

#include "systems/input/Input.h"
#include "systems/debug/Debug.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/audio/SheepAudio.h"
#include "systems/gamelogic/GameLogic.h"
#include "systems/physics/SheepPhysics.h"
#include "systems/anttweak/AntTweakModule.h"

#include "WxWindow.h"
#include "gfx/wxw/d3d/WxD3DCanvas.h"
#include "gfx/wxw/d3d/WxD3DContext.h"

namespace Framework
{
  Engine* AllocateEngine(void)
  {
    Engine* Core = new Engine();

    Core->AddSystem(new InputManager());
    Core->AddSystem(new GameLogic());
    Core->AddSystem(new SheepPhysics());
    Core->AddSystem(new SheepAudio());

    void* rc = nullptr;

#if USE_EDITOR
    //rc = ((dit::App*)dit::EDITOR_WINDOW)->window->canvas->context->GetRenderContext();
#endif

    Core->AddSystem(new SheepGraphics());
    Core->AddSystem(new AntTweakModule());
    Core->AddSystem(new Debug());

#if USE_EDITOR

#else
	Core->MakeWindow(GetModuleHandle(NULL), 1);
#endif
    return Core;
  }

  void InitEngine(void)
  {
    ENGINE->Initialize();

#if USE_EDITOR
#else
	  ENGINE->OpenEditor();
#endif
    
  }


}