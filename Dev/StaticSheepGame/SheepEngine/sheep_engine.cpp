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
#include <functional>

// This is a way to force the project to include the libraries
// without messing around with project settings
#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")
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
int ExportedEngineGraphicsFunction(void)
{
  // Call an exported function from the graphics
  return ExportedGraphicsFunction();
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

void TestStuff(void)
{
  OpenConsole();

  Serializer* s;
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

  FACTORY->LoadArchetypeFromFile("test_type2");

  // Create an empty object inside "TestSpace"
  GameObject* obj = space->CreateEmptyObject();

  // Create a Transform component in the space
  Transform* comp = (Transform*)space->CreateComponent(eTransform);
  comp->val1 = 13; // Set some values
  comp->val2 = 42;
  comp->val3 = 128;
  // Add the component to the object
  obj->AddComponent(comp);

  // Create a Tester component and set some stuff
  Tester* comp2 = (Tester*)space->CreateComponent(eTester);
  comp2->testvalue1 = 6;
  comp2->testvalue2 = 505;

  obj->AddComponent(comp2);
  
  // Set a name
  obj->name = "TestName";

  // Save the object as an archetype named "test_type"
  FACTORY->SaveObjectToArchetype(obj, "test_type");

  obj->name = "Old Object";


  // Create 3 more objects from the archetype "test_type"
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj->name = "Bob Junior";
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj->archetype = "Nottheoldone";
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");

  // Get the Transform component from one of those objects
  comp = obj->GetComponent<Transform>(eTransform);
  comp->val2 = 300; // sparta

  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  obj->archetype = "test_type2";
  obj->name = "This is wow";
  FACTORY->SaveObjectToArchetype(obj, "test_type2");

  // Get the Transform component from one of those objects
  comp = obj->GetComponent<Transform>(eTransform);
  comp->val2 = 1300;
  

  // Save the space as a level named "test_level" with the following settings
  // ObjectInstanceDataList = NULL
  //    We are not providing a white list of object variables
  //    We wish to print out, regardless if they have changed or not.
  // IncludeGeneric = true
  //    We are going to save objects which do not belong to an archetype
  // AllData = true
  //    We are going to save all data relating to archetypes.
  //    If the archetype exists in our archetype map (which means it has been
  //    saved or loaded once before we are saving the space) then we are
  //    only going to save any data which differs from the original archetype
  FACTORY->SaveSpaceToLevel(space, "test_level", NULL, true, true);

  // Remove "TestSpace", except i don't even know if this works right
  SheepEngine->RemoveSpace(space);

  // Create a new space called "CoolSpace"
  space = SheepEngine->CreateSpace("CoolSpace");

  s = Serializer::Get();

  // Load the level into the space
  FACTORY->LoadLevelToSpace(space, "test_level");

  s = Serializer::Get();
  // Handle manager looks like this right now
  // Handle 0: Object
  // Handle 1: Transform Component
  // Handle 2: Tester Component
  // Handle 3: Object
  // Handle 4: Transform Component
  // Handle 5: Tester Component
  // Handle 6: Object
  // Handle 7: Transform Component
  // Handle 8: Tester Component
  // Handle 9: Object
  // Handle 10: Transform Component
  // Handle 11: Tester Component

  // Lets grab the 3rd object, in reality this would never work
  // But I can create a fake handle since this is such a controlled test
  obj = space->GetHandles().GetAs<GameObject>(Handle(9, 0));

  // And lets get the transform variable
  comp = obj->GetComponent<Transform>(eTransform);

  int a = comp->val2;

  if (a == 300)
  {
    comp->val2 = 1000;
  }

  // If we want to save instanced object data we need to create
  // a list of variables we wish to save
  // I may eventually add archetype caching and only save objects
  // whose values are different from their archetypes
  // Also i plan on adding non-archetype object saving support
  std::vector<std::string> dataList;

  // Lets save val2 from inside of transform
  dataList.push_back("Transform:val2");

  // Save the space as a level named "test_level2"
  FACTORY->SaveSpaceToLevel(space, "test_level2", &dataList, true, false);

  // Make 100 more objects
  for (unsigned int i = 0; i < 100; ++i)
    FACTORY->LoadObjectFromArchetype(space, "test_type");

  // Save the space as a standalone level (Does not rely on archetypes)
  FACTORY->SaveSpaceToLevel(space, "test_level_standalone", true);

  // Save the space as a level named "test_level3" Notice the difference in file size
  FACTORY->SaveSpaceToLevel(space, "test_level3", NULL, true, true);

  SheepEngine->RemoveSpace(space);

  space = SheepEngine->CreateSpace("Stand alone space");

  FACTORY->LoadLevelToSpace(space, "test_level_standalone");

  FACTORY->SaveArchetypeToFile("test_type");

  obj = FACTORY->LoadObjectFromArchetype(space, "test_type");
  
  comp = obj->GetComponent<Transform>(eTransform);

  Function g = BUILD_FUNCTION(GameObject::Destroy);
  g.Bind(obj);
  g();

  Lua::CallFunc(ENGINE->Lua(), "Test");



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