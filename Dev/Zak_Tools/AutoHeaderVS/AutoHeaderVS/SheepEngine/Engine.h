/*****************************************************************
Filename: Engine.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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
    static GameSpace* LuaCreateSpace(const char* name);
    static GameSpace* LuaGetSpace(const char* name);
    static GameComponent* LuaGetComponent(const char* name, unsigned int handle, const char* type);
    static void LuaRemoveObjectFromEngine(const char* space, unsigned int handle);
    static void LuaQuit();
    static void LuaSetVariable(Generic* obj, Member* member, int dummy);
    static void LuaGetVariable(Generic* obj, Member* member);

    GameSpace* CreateSpace(const char* name);

    void RemoveSpace(GameSpace* space);

    GameSpace* GetSpace(const char* name);

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