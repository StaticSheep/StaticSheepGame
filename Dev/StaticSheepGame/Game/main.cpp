
//#include <wtypes.h>
#include <Windows.h>
#include "engine\sheep_engine.h"

// All content © 2014 DigiPen (USA) Corporation, all rights reserved.

#pragma comment (lib, "SheepEngine.lib")

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
  int a, b, c, d = 0;

  // Call some functions from inside of the engine
  a = ExportedEngineFunction();
  b = ExportedEngineGraphicsFunction();
  c = ExportedEnginePhysicsFunction();

  // This will not compile since the function was not exported from the engine project
  //d = InternalEngineFunction();
  
  TestStuff();

  Foo f = DoCoolStuff();
  f.func1();

  return d;
}