// Demo file
// Sorry for the terrible naming convention, I wanted to make it clear what each function represented

#include "sheep_engine.h"
#include "Object.h"

// Include the exported header file from physics / graphics
#include "physics\sheep_physics.h"
#include "graphics\sheep_graphics.h"

// This is a way to force the project to include the libraries
// without messing around with project settings
#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")

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
SHEEP_API int ExportedEngineFunction(void)
{
  // Call an internal function from inside of the engine
  return InternalStaticEngineFunction();
}

// This is an exported Engine function that calls
// an exported function from the graphics
SHEEP_API int ExportedEngineGraphicsFunction(void)
{
  // Call an exported function from the graphics
  return ExportedGraphicsFunction();
}

// This is an exported Engine function that calls
// an exported function from the graphics
SHEEP_API int ExportedEnginePhysicsFunction(void)
{
  // Call an exported function from the physics
  return ExportedPhysicsFunction();
}


// IGNORE THE STUFF BELOW THIS

using namespace Framework;

SHEEP_API void TestStuff(void)
{
  std::shared_ptr<GameObject> obj1(new GameObject(1));
  obj1->self = obj1;

  obj1->fastChildSearch = true;

  std::shared_ptr<GameObject>* temp;
  for (int i = 0; i < 100; ++i)
  {
    temp = new std::shared_ptr<GameObject>(new GameObject(i * i - 30 * i + 1000));
    temp->get()->self = *temp;
    obj1->AddChild(**temp);
  }
  

  size_t tempID = 0;
  ChildArray &children = obj1->GetChildren();
  for(ChildArray::iterator it = children.begin(); it != children.end(); ++it)
  {	
    tempID = it->lock().get()->GetID();
  }
}