/*****************************************************************
Filename: Engine.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  Engine* ENGINE = NULL;

  Engine::Engine()
    : m_running(true)
  {

    EngineTypeRegistration();

    ENGINE = this;
  }

  Engine::~Engine()
  {
    for (unsigned int i = m_systems.size() - 1; i > 0; --i)
    {
      delete m_systems[i];
    }
  }

  void Engine::AddSystem(ISystem* system)
  {
    m_systems.push_back(system);
    system->RegisterComponents();
  }

  void Engine::Initialize()
  {
    for (unsigned int i = 0; i < m_systems.size(); ++i)
    {
      m_systems[i]->Initialize();
    }
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
    GameSpace* space = new GameSpace();

    space->m_name = name;

    m_spaceMap[space->m_name] = space;
    m_spaces.push_back(space);

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
    delete space;
  }
}