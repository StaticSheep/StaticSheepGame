/*****************************************************************
Filename: Engine.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Engine;

  extern Engine* ENGINE;

  class Engine
  {
  public:
    Engine();
    ~Engine();

    void AddSystem(ISystem* system);
    void Initialize();
    void MainLoop();
    void Shutdown();

    bool Running() const;


    GameSpace* CreateSpace(const char* name);
    void RemoveSpace(GameSpace* space);
    GameSpace* GetSpace(const char* name);

    Factory ObjectFactory;
  private:
    bool m_running;
    std::vector<ISystem*> m_systems;

    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;

    friend class GameLogic;
  };
}