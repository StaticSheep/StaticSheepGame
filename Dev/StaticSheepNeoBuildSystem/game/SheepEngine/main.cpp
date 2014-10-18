#include "precompiled.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//#include <wtypes.h>
#include <Windows.h>
//#include <vld.h>

#include "Input.h"
#include "Debug.h"
#include "SheepGraphics.h"
//#include "FramerateController.h"
#include "SheepAudio.h"
#include "GameLogic.h"
#include "SheepPhysics.h"
#include "AntTweakModule.h"
#include <iostream>


#include "App.h"
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

using dit::App;
IMPLEMENT_APP_CONSOLE(App)

int mainb(int argc, char** argv)
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

