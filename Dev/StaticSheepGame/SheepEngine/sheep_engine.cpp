// Demo file
// Sorry for the terrible naming convention, I wanted to make it clear what each function represented

#include "sheep_engine.h"

#include "Object.h"

#include "Enum.h"

// Include the exported header file from physics / graphics
#include "physics\sheep_physics.h"
#include "graphics\sheep_graphics.h"
#include "SheepPhysics.h"
#include "SheepGraphics.h"
#include "PhysicsObject.h"
#include "GameLogic.h"
#include "Factory.h"
#include <functional>

// This is a way to force the project to include the libraries
// without messing around with project settings
#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")
//#pragma comment (lib, "lua51.lib")
//#pragma comment (lib, "lfs.lib")
#pragma comment (lib, "luaSource.lib")

// ********* INTERNAL FUNCTIONS ********* //

// A random function inside the engine dll, this function is "internal" meaning it cannot
// be called from outside of the dll EVEN if it has a matching header file.
// For it to be external it needs the SHEEP_API prefix
// For out project however, we will never have non-exported functions which are declared in the .cpp
// file we are using for exporting functions (UNLESS THEY ARE STATIC FUNCTIONS)
int InternalEngineFunction(void)
{
  return 1;
}

// This is ok to do inside of a .cpp file that contains exported functions since it is a
// static function (compared to the one above)
static int InternalStaticEngineFunction(void)
{
  return 1;
}

// ********* EXPORTED FUNCTIONS ********* //

// These functions are Exported
// This means that they can be accessed via
// the .dll file (with a corresponding header)
// Compared to internal functions, these functions have
// the SHEEP_API prefix infront of their definition AND declaration

// This is an exported Engine function that calls
// the private function inside this file
int ExportedEngineFunction(void)
{
  // Call an internal function from inside of the engine
  return InternalStaticEngineFunction();
}

// This is an exported Engine function that calls
// an exported function from the graphics
int ExportedEnginePhysicsFunction(void)
{
  // Call an exported function from the physics
  return ExportedPhysicsFunction();
}


// IGNORE THE STUFF BELOW THIS

void TestMore(void)
{

}

using namespace Framework;

void TestStuff(HINSTANCE hInstance, int show)
{

  OpenConsole();

  // Create the engine
  Engine* SheepEngine = new Engine();
  

  SheepEngine->AddSystem(new GameLogic());
  SheepEngine->AddSystem(new SheepPhysics());
  SheepEngine->AddSystem(new SheepGraphics());

  SheepEngine->MakeWindow(hInstance, show);
  SheepEngine->Initialize();

  //GameSpace* space = SheepEngine->CreateSpace("test_space");

  //GameObject* obj = FACTORY->LoadObjectFromArchetype(space, "test_type2");
  //SheepEngine->LoadLevel("test_level");

  SheepEngine->LoadLuaLevel("content/lua/engine/levels/editor.lua");


  while (SheepEngine->Running())
  {
    SheepEngine->MainLoop();
  }

  SheepEngine->Shutdown();

  delete SheepEngine;

  // Add a comment here
}



void Foo::func1()
{

}

void Foo::func2()
{

}

Foo DoCoolStuff()
{
  Foo f;
  f.value1 = 10;
  f.func1();
  f.func2();
  return f;
}