#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//#include <wtypes.h>
#include <Windows.h>
#include "engine\sheep_engine.h"
#include <string>

#include "engine\Engine.h"
#include "engine\GameLogic.h"
#include "Utilities.h"


// All content © 2014 DigiPen (USA) Corporation, all rights reserved.

#pragma comment (lib, "SheepEngine.lib")

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  //_CrtSetBreakAlloc(4224);

  OpenConsole();

  // Create the engine
  Framework::Engine* SheepEngine = new Framework::Engine();
  

  SheepEngine->AddSystem(new GameLogic());
  //SheepEngine->AddSystem(new SheepPhysics());
  //SheepEngine->AddSystem(new SheepGraphics());

  SheepEngine->MakeWindow(instanceH, show);
  SheepEngine->Initialize();

  SheepEngine->LoadLuaLevel("content/lua/engine/levels/editor.lua");


  while (SheepEngine->Running())
  {
    SheepEngine->MainLoop();
  }

  SheepEngine->Shutdown();

  delete SheepEngine;


  
  return 0;
}