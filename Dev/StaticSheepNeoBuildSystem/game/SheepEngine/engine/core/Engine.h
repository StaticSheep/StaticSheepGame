/*****************************************************************
Filename: Engine.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "engine/tracelog/TraceLog.h"
#include <stack>

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

    // Sends a message to all systems in the engine
    void SystemMessage(Message& msg);

    // ==================================== LEVELS ==================================== //

    // Loads a lua level
    void LoadLuaLevel(const char* path);

    // Changes the level at the end of the frame
    void ChangeLevel(const char* name);

    
    // ==================================== SPACES ==================================== //

    // Returns a list of GameSpaces in the engine
    std::vector<GameSpace*>& Spaces(void);

    // Marks all spaces to be destroyed
    void ClearSpaces();

    // Creates a new game space
    GameSpace* CreateSpace(const char* name);

    // Retrieves a game space of a corresponding name
    GameSpace* GetSpace(const char* name);

    // ==================================== EDITOR ==================================== //

    static void PlayInEditor(bool play);
    static bool PlayingInEditor(void);
    static void OpenEditor();

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


    // =========================== PUBLIC MEMBERS ========================== //
    
    // Object factory of the engine
    Factory ObjectFactory;

    // Framerate controller of the engine
    FramerateController Framerate;

    // Win32 Window used by the engine
    SheepWindow* Window;

    // Trace log
    Tracelog TraceLog;
  private:

    // ========================== PRIVATE FUNCTIONS ===================== //

    /*---------- Cleanup -----------*/

    // Destroys any old spaces
    void CleanUp(void);

    // Actually destroys a game space [ENGINE USE ONLY]
    void RemoveSpace(GameSpace* space);

    /*---------- Levels -----------*/

    // Actually unloads the current level and loads a new one [ENGINE USE ONLY]
    void GotoNextLevel(void);;

    // Loads any new lua levels
    void LoadLuaLevels(void);

    // =========================== PRIVATE MEMBERS =========================== //


    /*---------- Level Data -----------*/

    // Name of the next level to load
    std::string m_nextLevel;
    bool m_levelChange = false;

    /*---------- System Data ----------*/

    // Engine is running
    bool m_running;

    // All the systems
    std::vector<ISystem*> m_systems;

    // Map & Vector of all the spaces
    std::hash_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;
    std::vector<GameSpace*> m_spaceRemoveList;

    /*---------- Lua Data ----------*/

    // The Lua environment
    lua_State* L;

    // Lua Level Stack
    std::stack<std::string> m_luaLevelsToLoad;
    bool m_loadLuaLevels = false;
    
    /*---------- Play In Editor ----------*/

    // Play in editor fun stuff
    bool m_PIE = false;
    bool m_returnFromPIE = false;
    void ReloadEditor(void);
    void CheckReturnFromPIE(void);

    /*---------- Friends ----------*/

    friend class GameLogic;
    friend class GameSpace;
    friend class FramerateController;
  };


  
}