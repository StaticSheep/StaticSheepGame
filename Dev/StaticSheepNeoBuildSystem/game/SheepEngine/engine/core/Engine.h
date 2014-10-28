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

    // Adds a system to the engine system list
    void AddSystem(ISystem* system);

    // Creates a window for the engine to use
    void MakeWindow(void* hInstance, int show);

    // Initializes all engine systems and the engine
    void Initialize(void);

    // Runs one step of the main loop
    // Conditions: Framerate is ready
    void MainLoop(void);

    // Runs one step of the game loop regardless of time
    void Step(void);

    // Prepare the engine for destruction
    void Shutdown(void);

    // Lowers the running flag
    void Quit(void);

    // Returns if the engine's running flag
    bool Running() const;

    // Loads a serialized level
    void LoadLevel(const char* name);

    // Loads a lua level
    void LoadLuaLevel(const char* path);

    // Returns a list of GameSpaces in the engine
    std::vector<GameSpace*>& Spaces(void);

    // Sends a message to all systems in the engine
    void SystemMessage(Message& msg);

    static void PlayInEditor(bool play);
    static bool PlayingInEditor(void);

    static void OpenEditor();

    void ClearSpaces();

    GameSpace* CreateSpace(const char* name);

    void RemoveSpace(GameSpace* space);

    GameSpace* GetSpace(const char* name);


    // Returns the lua environment
    lua_State* Lua(void) const;

    // ==================================== ENGINE LUA BINDS ==================================== //
    static GameSpace* LuaCreateSpace(const char* name);
    static GameSpace* LuaGetSpace(const char* name);
    static GameComponent* LuaGetComponent(const char* name, unsigned int handle, const char* type);
    static void LuaRemoveObjectFromEngine(const char* space, unsigned int handle);
    static void LuaQuit();
    static void LuaSetVariable(Generic* obj, Member* member, int dummy);
    static void LuaGetVariable(Generic* obj, Member* member);
    static void LuaClearSpaces();
    static void LuaRemoveSpace(const char* name);
    // Function used by lua for printing errors
    static void LuaError(const char* msg);

    

    int shittyFramerate;

    Factory ObjectFactory;
    FramerateController Framerate;
    SheepWindow* Window;

    Tracelog m_traceLog;
  private:
    bool m_running;
    std::vector<ISystem*> m_systems;

    lua_State* L;

    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;


    // Play in editor fun stuff
    bool m_PIE = false;
    bool m_returnFromPIE = false;
    void ReloadEditor(void);
    void CheckReturnFromPIE(void);

    friend class GameLogic;
  };


  
}