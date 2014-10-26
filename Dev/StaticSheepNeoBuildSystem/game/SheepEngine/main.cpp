#include "pch/precompiled.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//#include <wtypes.h>
#include <Windows.h>

#include <vld.h>

#include "systems/input/Input.h"
#include "systems/debug/Debug.h"
#include "systems/graphics/SheepGraphics.h"
//#include "FramerateController.h"
#include "systems/audio/SheepAudio.h"
#include "systems/gamelogic/GameLogic.h"
#include "systems/physics/SheepPhysics.h"
#include "systems/anttweak/AntTweakModule.h"
#include <iostream>


#include "editor/App.h"
#include "wx\app.h"

#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")

//#pragma comment (lib, "SheepGraphics.lib")

//namespace boost {
//  void throw_exception(std::exception const & e)
//  {
//    throw e;
//  }
//}

using namespace Framework;

namespace boost {
  void throw_exception(std::exception const & e)
  {
    throw e;
  }
}


#if USE_EDITOR

using dit::App;
IMPLEMENT_APP_CONSOLE(App)

#else

int main(int argc, char** argv)
{
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  //_CrtSetBreakAlloc(6295);

  HINSTANCE hInst = GetModuleHandle(NULL);

  OpenConsole();

  Engine* Core = new Engine();

  Core->AddSystem(new InputManager());
  Core->AddSystem(new GameLogic());
  Core->AddSystem(new SheepPhysics());
  Core->AddSystem(new SheepAudio());
  Core->AddSystem(new SheepGraphics());
  Core->AddSystem(new AntTweakModule());
  Core->AddSystem(new Debug());

  Core->MakeWindow(&hInst, 1);

  Core->Initialize();

  //Framework::AntTweak::TBar* bar = ATWEAK->CreateBar("TestBar");

  //GameSpace* space = Core->CreateSpace("TestSpace");
  //GameObject* obj = FACTORY->LoadObjectFromArchetype(space, "drawtest");
  ////GET_TYPE(GameObject)->Tweak(nullptr, obj, "obj", "GameObject");
  //GET_TYPE(GameSpace)->Tweak(nullptr, space, nullptr, nullptr);
  //Core->LoadLuaLevel("content/lua/engine/levels/testlevel.lua");

  Core->OpenEditor();

  //Core->LoadLevel("content/data/spaces/AnimationSound.space");

  while (Core->Running())
  {
    Core->MainLoop();
  }

  Core->Shutdown();

  delete Core;

  return 0;
}

#endif


