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

  GameSpace* space = SheepEngine->CreateSpace("test_space");

  GameObject* obj = space->CreateEmptyObject();

  Transform* trans = (Transform*)space->CreateComponent(eTransform);
  trans->translation.X = 10.0f;
  obj->AddComponent(trans);

  Sprite* sprite = (Sprite*)space->CreateComponent(eSprite);
  obj->AddComponent(sprite);

  obj->Initialize();

  //obj->archetype = "drawtest";

  //FACTORY->SaveObjectToArchetype(obj, "drawtest");

  //FACTORY->SaveSpaceToLevel(space, "WowSpace", true);
  

  //SheepEngine->LoadLevel("WowSpace");

  //SheepEngine->LoadLuaLevel("content/lua/engine/levels/editor.lua");


  while (SheepEngine->Running())
  {
    SheepEngine->MainLoop();
  }

  SheepEngine->Shutdown();

  delete SheepEngine;

  // Add a comment here
}