/*****************************************************************
Filename: main.cpp
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//#include <wtypes.h>
#include <Windows.h>

//#include <vld.h>

#include "systems/input/Input.h"
#include "systems/debug/Debug.h"
#include "systems/graphics/SheepGraphics.h"
//#include "FramerateController.h"
#include "systems/audio/SheepAudio.h"
#include "systems/gamelogic/GameLogic.h"
#include "systems/physics/SheepPhysics.h"
#include "systems/anttweak/AntTweakModule.h"
#include <iostream>


//#include "wxeditor/App.h"
//#include "wx\app.h"

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
  //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  //_CrtSetBreakAlloc(6295);

  

  Engine* Core = AllocateEngine(argc, argv);
  InitEngine();

  while (ENGINE->Running())
  {
    ENGINE->MainLoop();
  }

  ENGINE->Shutdown();

  delete Core;

  return 0;
}

#endif


