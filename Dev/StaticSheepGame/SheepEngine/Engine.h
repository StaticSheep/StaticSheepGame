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

  SHEEP_API extern Engine* ENGINE;

  class Engine
  {
  public:
    SHEEP_API Engine();
    SHEEP_API ~Engine();

    SHEEP_API void AddSystem(ISystem* system);
    SHEEP_API void Initialize();
    SHEEP_API void MainLoop();
    SHEEP_API void Shutdown();

    SHEEP_API bool Running() const;

    SHEEP_API lua_State* Lua(void) const;

    SHEEP_API static void LuaError(const char* msg);


    GameSpace* CreateSpace(const char* name);
    void RemoveSpace(GameSpace* space);
    GameSpace* GetSpace(const char* name);

    Factory ObjectFactory;
  private:
    bool m_running;
    std::vector<ISystem*> m_systems;

    lua_State* L;

    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;

    friend class GameLogic;
  };


  // Bound to lua as "GetComponent"
  GameComponent* LuaGetComponent(const char* name, unsigned int handle, const char* type);

  // Bound to lua as "RemoveGameObject"
  void RemoveObjectFromEngine(const char* space, unsigned int handle);
}