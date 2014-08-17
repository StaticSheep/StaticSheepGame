/*****************************************************************
Filename: Engine.h
*****************************************************************/

#pragma once

#include "Window.h"

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
    SHEEP_API void MakeWindow(HINSTANCE hInstance, int show);
    SHEEP_API void Initialize();
    SHEEP_API void MainLoop();
    SHEEP_API void Shutdown();

    SHEEP_API void Quit();

    SHEEP_API bool Running() const;

    SHEEP_API lua_State* Lua(void) const;

    SHEEP_API static void LuaError(const char* msg);

    SHEEP_API void LoadLevel(const char* name);
    SHEEP_API void LoadLuaLevel(const char* path);

    SHEEP_API std::vector<GameSpace*>& Spaces();

    // Static Lua binds




    int shittyFramerate;

    Factory ObjectFactory;
    SheepWindow Window;
  private:
    bool m_running;
    std::vector<ISystem*> m_systems;

    lua_State* L;

    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;

    friend class GameLogic;
  };


  
}
