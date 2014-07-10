#include <iostream>
/*****************************************************************
Filename: Engine.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  Engine* ENGINE = NULL;

  Engine::Engine()
    : m_running(true), L(nullptr)
  {

    EngineTypeRegistration();

    ENGINE = this;
  }

  Engine::~Engine()
  {
    if (m_systems.size() > 0)
      for (unsigned int i = 0; i < m_systems.size(); ++i)
        delete m_systems[i];

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

  void Engine::Shutdown()
  {
    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Shutdown();

    Lua::Shutdown(L);
  }

  void Engine::MainLoop()
  {
    const float dt = 1.0f / 60.0f; // 60 frames per second

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Update(dt);
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
    //space->Cleanup();

    Lua::CallFunc(ENGINE->Lua(), "RemoveGameSpace", space->m_name);

    delete space;
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
    std::string spaceName = name;
    if (m_spaceMap.find(spaceName) != m_spaceMap.end())
      return m_spaceMap[spaceName];
    return nullptr;
  }

  void Engine::LoadLevel(const char* name)
  {
    GameSpace* space = CreateSpace(name);

    FACTORY->LoadLevelToSpace(space, name);
  }

  void Engine::LoadLuaLevel(const char* path)
  {
    Lua::LoadFile(L, path);
  }

  bool Engine::Running() const
  {
    return m_running;
  }

  GameSpace* Engine::LuaCreateSpace(const char* name)
  {
    // Creates the space
    return ENGINE->CreateSpace(name);
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


}