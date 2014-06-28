// Demo file
// Sorry for the terrible naming convention, I wanted to make it clear what each function represented

#include "sheep_engine.h"
#include "Object.h"

#include "Enum.h"

// Include the exported header file from physics / graphics
#include "physics\sheep_physics.h"
#include "graphics\sheep_graphics.h"
#include "SheepPhysics.h"
#include "PhysicsObject.h"
#include "GameLogic.h"
#include "Factory.h"

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
  // Create the engine
  Engine* SheepEngine = new Engine();
  // Add the GameLogic system which really doesn't do anything right now :v
  SheepEngine->AddSystem(new GameLogic());
  // Add the Physics engine which will register the transform component for us
  SheepEngine->AddSystem(new SheepPhysics());
  // Initialize the engine and the systems
  SheepEngine->Initialize();

  // Create a space named "TestSpace"
  GameSpace* space = SheepEngine->CreateSpace("TestSpace");

  // Create an empty object inside "TestSpace"
  GameObject* obj = space->CreateEmptyObject();

  // Create a Transform component in the space
  Transform* comp = (Transform*)space->CreateComponent(eTransform);
  comp->val1 = 13; // Set some values
  comp->val2 = 42;
  comp->val3 = 128;
  // Add the component to the object
  obj->AddComponent(comp);
  
  // Save the object as an archetype named "test_type"
  FACTORY->SaveObjectToArchetype(obj, "test_type");


  // Create 3 more objects from the archetype "test_type"
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");

  // Get the Transform component from one of those objects
  comp = obj->GetComponent<Transform>(eTransform);
  comp->val2 = 300; // sparta

  // If we want to save instanced object data we need to create
  // a list of variables we wish to save
  // I may eventually add archetype caching and only save objects
  // whose values are different from their archetypes
  // Also i plan on adding non-archetype object saving support
  std::vector<std::string> dataList;

  // Lets save val2 from inside of transform
  dataList.push_back("Transform:val2");

  // Save the space as a level named "test_level"
  FACTORY->SaveSpaceToLevel(space, "test_level", &dataList, true, false);

  // Remove "TestSpace", except i don't even know if this works right
  SheepEngine->RemoveSpace(space);

  // Create a new space called "CoolSpace"
  space = SheepEngine->CreateSpace("CoolSpace");

  // Load the level into the space
  FACTORY->LoadLevelToSpace(space, "test_level");

  // Handle manager looks like this right now
  // Handle 0: Object
  // Handle 1: Transform Component
  // Handle 2: Object
  // Handle 3: Transform Component
  // Handle 4: Object
  // Handle 5: Transform Component

  // Lets grab the 3rd object, in reality this would never work
  // But I can create a fake handle since this is such a controlled test
  obj = space->GetHandles().GetAs<GameObject>(Handle(4, 0));

  // And lets get the transform variable
  comp = obj->GetComponent<Transform>(eTransform);

  int a = comp->val2;

  if (a == 300)
  {
    comp->val2 = 1000;
  }

  // Save the space as a level named "test_level"
  FACTORY->SaveSpaceToLevel(space, "test_level2", &dataList, true, false);

}