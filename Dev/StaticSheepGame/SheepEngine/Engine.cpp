/*****************************************************************
Filename: Engine.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

// This is a way to force the project to include the libraries
// without messing around with project settings
#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")
#pragma comment (lib, "luaSource.lib")

#include <iostream>
#include "Window.h"

namespace Framework
{
  Engine* ENGINE = NULL;

  Engine::Engine()
    : m_running(true), L(nullptr)
  {
    shittyFramerate = 0;

    EngineTypeRegistration();

    ENGINE = this;
  }

  Engine::~Engine()
  {
    ENGINE = nullptr;

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      delete m_systems[i];

    m_systems.clear();
    m_spaces.clear();
    m_spaceMap.clear();
  }

  void Engine::AddSystem(ISystem* system)
  {
    m_systems.push_back(system);
    system->RegisterComponents();
  }

  void Engine::Initialize()
  {
    L = Lua::CreateEnvironment();

    Lua::BindDefaultFunctions();

    //Lua::CallFunc(L, "filesystem.UpdateOldFiles");

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Initialize();
  }

  void Engine::MakeWindow(void* hInstance, int show)
  {
    Window->MakeWindow(hInstance, show);
  }

  void Engine::Shutdown()
  {
    for (unsigned int i = 0; i < m_spaces.size(); ++i)
    {
      GameSpace* space = m_spaces[i];

      m_spaceMap.erase(space->m_name);

      space->m_shuttingDown = true;
      space->Cleanup();

      Lua::CallFunc(ENGINE->Lua(), "RemoveGameSpace", space->m_name);

      delete space;
    }

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Shutdown();

    Lua::Shutdown(L);

    L = nullptr;
  }

  void Engine::SystemMessage(Message& msg)
  {
    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->ReceiveMessage(msg);
  }

  void Engine::MainLoop()
  {
    const float dt = 1.0f / 60.0f; // 60 frames per second

    if (shittyFramerate > 100)
    {
      Window->Update();

      for (unsigned int i = 0; i < m_systems.size(); ++i)
        m_systems[i]->Update(dt);

      shittyFramerate = 0;
    }

    ++shittyFramerate;

    
  }

  GameSpace* Engine::CreateSpace(const char* name)
  {
    // Allocate space for a new space
    GameSpace* space = DBG_NEW GameSpace();

    space->m_name = name;

    m_spaceMap[space->m_name] = space;
    m_spaces.push_back(space);

    Lua::CallFunc(L, "AddGameSpace", name);

    return space;
  }

  void Engine::RemoveSpace(GameSpace* space)
  {
    m_spaceMap.erase(space->m_name);
    for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
    {
      if (*it == space)
      {
        m_spaces.erase(it);
        break;
      }
    }

    space->m_shuttingDown = true;
    space->Cleanup();

    Lua::CallFunc(ENGINE->Lua(), "RemoveGameSpace", space->m_name);

    delete space;
  }

  std::vector<GameSpace*>& Engine::Spaces()
  {
    return m_spaces;
  }

  lua_State* Engine::Lua() const
  {
    return L;
  }

  void Engine::LuaError(const char* msg)
  {
    Error("LuaError","%s", msg);
  }

  GameSpace* Engine::GetSpace(const char* name)
  {
    //std::string spaceName = name;
    if (m_spaceMap.find(name) != m_spaceMap.end())
      return m_spaceMap[name];
    return nullptr;
  }

  void Engine::LoadLevel(const char* name)
  {
    GameSpace* space = CreateSpace(name);

    FACTORY->LoadLevelToSpace(space, name);
  }

  void Engine::LoadLuaLevel(const char* path)
  {
    Lua::CallFunc(L, "LoadLuaLevel", path);
    //Lua::LoadFile(L, path);
  }

  void Engine::ClearSpaces()
  {
    for (unsigned int i = 0; i < m_spaces.size(); ++i)
    {
      GameSpace* space = m_spaces[i];
      space->m_valid = false;
    }
  }

  bool Engine::Running() const
  {
    return m_running;
  }

  GameSpace* Engine::LuaCreateSpace(const char* name)
  {
    GameSpace* space = ENGINE->GetSpace(name);
    if (space == nullptr)
    {
      // Creates the space
      return ENGINE->CreateSpace(name);
    }
    else
    {
      std::string realName = name;
      realName += "[nametaken]";
      // Creates the space
      return ENGINE->LuaCreateSpace(realName.c_str());
    }
    //return space;
  }

  void Engine::Quit()
  {
    m_running = false;
  }

  GameSpace* Engine::LuaGetSpace(const char* name)
  {
    return ENGINE->GetSpace(name);
  }

  GameComponent* Engine::LuaGetComponent(const char* name, unsigned int handle, const char* type)
  {
    GameSpace* space = (GameSpace*)ENGINE->GetSpace(name);

    ErrorIf(space == nullptr, "Engine-Lua Interface", "Space not found: %s", name);

    const char* mt = GET_TYPE(Transform)->LuaMetaTable();

    EComponent cType = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
    return space->GetHandles().GetAs<GameObject>(handle)->GetComponent(cType);
  }

  void Engine::LuaRemoveObjectFromEngine(const char* space, unsigned int handle)
  {
    ENGINE->GetSpace(space)->GetHandles().GetAs<GameObject>(handle)->Destroy();
  }

  void Engine::LuaQuit()
  {
    ENGINE->Quit();
  }

  void Engine::LuaSetVariable(Generic* obj, Member* member, int dummy)
  {
    Variable value;

    void* buffer = alloca(member->Type()->Size());

    new (&value) Variable(member->Type(), buffer);
    value.PlacementNew();

    value.FromLua(ENGINE->Lua(), 3); // 2 + 1 (Argument 2 [zero based], + 1) 

    member->Type()->PlacementCopy((char*)obj + member->Offset(), buffer);
  }

  void Engine::LuaGetVariable(Generic* obj, Member* member)
  {
    Variable value(member->Type(), (char*)obj + member->Offset());

    value.ToLua(ENGINE->Lua());
  }

  void Engine::LuaClearSpaces()
  {
    ENGINE->ClearSpaces();
  }

  void Engine::LuaRemoveSpace(const char* name)
  {
    ENGINE->GetSpace(name)->m_valid = false;
  }


}