/*****************************************************************
Filename: Engine.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "systems/debug/tracelog/TraceLog.h"

namespace Framework
{
  class Engine;
  class SheepWindow;

  extern Engine* ENGINE;

  class Engine
  {
  public:
    Engine();
    ~Engine();

    void AddSystem(ISystem* system);
    void MakeWindow(void* hInstance, int show);
    void Initialize();
    void MainLoop();
    void Step();
    void Shutdown();

    void Quit();

    bool Running() const;

    lua_State* Lua(void) const;

    static void LuaError(const char* msg);

    void LoadLevel(const char* name);
    void LoadLuaLevel(const char* path);

    std::vector<GameSpace*>& Spaces();

    void SystemMessage(Message& msg);

    // Static Lua binds
    static GameSpace* LuaCreateSpace(const char* name);
    static GameSpace* LuaGetSpace(const char* name);
    static GameComponent* LuaGetComponent(const char* name, unsigned int handle, const char* type);
    static void LuaRemoveObjectFromEngine(const char* space, unsigned int handle);
    static void LuaQuit();
    static void LuaSetVariable(Generic* obj, Member* member, int dummy);
    static void LuaGetVariable(Generic* obj, Member* member);
    static void LuaClearSpaces();
    static void LuaRemoveSpace(const char* name);

    static void OpenEditor();

    void ClearSpaces();

    GameSpace* CreateSpace(const char* name);

    void RemoveSpace(GameSpace* space);

    GameSpace* GetSpace(const char* name);

    int shittyFramerate;

    Factory ObjectFactory;
    FramerateController Framerate;
    SheepWindow* Window;

  private:
    bool m_running;
    std::vector<ISystem*> m_systems;

    lua_State* L;

    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;

	  Tracelog m_traceLog;

    friend class GameLogic;
    friend class FramerateController;
  };


  
}