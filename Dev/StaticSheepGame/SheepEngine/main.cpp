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

#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")

//#pragma comment (lib, "SheepGraphics.lib")

using namespace Framework;

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  //_CrtSetBreakAlloc(6295);


  OpenConsole();

  Engine* Core = new Engine();

  Core->AddSystem(new InputManager());
  Core->AddSystem(new GameLogic());
  Core->AddSystem(new SheepPhysics());
  Core->AddSystem(new SheepAudio());
  Core->AddSystem(new SheepGraphics());
  Core->AddSystem(new AntTweakModule());
  Core->AddSystem(new Debug());

  Core->MakeWindow(&hInstance, nShowCmd);

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

