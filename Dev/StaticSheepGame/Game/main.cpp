#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//#include <wtypes.h>
#include <Windows.h>
#include "engine\sheep_engine.h"
#include <string>



// All content © 2014 DigiPen (USA) Corporation, all rights reserved.

#pragma comment (lib, "SheepEngine.lib")

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  //_CrtSetBreakAlloc(3322);
  int a, b, c, d = 0;

  // Call some functions from inside of the engine
  //a = ExportedEngineFunction();
  b = ExportedEngineGraphicsFunction();
  //c = ExportedEnginePhysicsFunction();

  // This will not compile since the function was not exported from the engine project
  //d = InternalEngineFunction();
  
  TestStuff(instanceH, show);

  //_CrtDumpMemoryLeaks();
  return d;
}