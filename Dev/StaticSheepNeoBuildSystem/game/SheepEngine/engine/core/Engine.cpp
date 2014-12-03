/*****************************************************************
Filename: Engine.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

// This is a way to force the project to include the libraries
// without messing around with project settings
#pragma comment (lib, "SheepGraphics.lib")
#pragma comment (lib, "SheepPhysics.lib")
#pragma comment (lib, "luaSource.lib")

#include "pch/precompiled.h"

#include "engine/core/Engine.h"

#include <iostream>


#include "engine/window/Window32.h"
#include "components/transform/CTransform.h"
#include "systems/input/Input.h"
#include "systems/graphics/SheepGraphics.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

namespace Framework
{
  Engine* ENGINE = NULL;

  Engine::Engine()
    : m_running(true), L(nullptr), m_time(0)
  {
    EngineTypeRegistration();

    Window = new SheepWindow();

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

    delete Window;
  }

  void Engine::AddSystem(ISystem* system)
  {
    m_systems.push_back(system);
    system->RegisterComponents();
  }

  void Engine::Initialize()
  {
    L = Lua::CreateEnvironment();

    Lua::CallFunc(L, "PostInit");

    //Lua::CallFunc(L, "filesystem.UpdateOldFiles");

    for (unsigned int i = 0; i < m_systems.size(); ++i)
    {
      m_systems[i]->Initialize();
    }

    Framerate.Initialize();
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

      Lua::CallFunc(ENGINE->Lua(), "gamespace.RemoveGameSpace", space->m_name);

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

  const std::vector<std::string>& Engine::LuaComponentList(void) const
  {
    return m_luaComponentList;
  }


  void Engine::Step(void)
  {
    Window->Update();

    m_time += Framerate.GetDT() * 1000.0f;

    for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
      if (!(*it)->m_ready)
        (*it)->m_ready = true;

    for (unsigned int i = 0; i < m_systems.size(); ++i)
    {
      Framerate.StartFrame();
      m_systems[i]->Update(Framerate.GetDT());
      Framerate.EndFrame(m_systems[i]->GetName().c_str());
    }


    CleanUp();

    if (m_levelChange)
      GotoNextLevel();

    if (m_loadLuaLevels)
      LoadLuaLevels();

    if (m_enteringPIE)
      StartPIE();

    if (m_returnFromPIE)
      ReloadEditor();

    if (m_PIE)
      CheckReturnFromPIE();
  }

  void Engine::MainLoop()
  {
    if(Framerate.FramerateCheck())
    {
      Step();
    }
  }

  bool Engine::Running() const
  {
    return m_running;
  }

  void Engine::Quit()
  {
    m_running = false;
  }

  unsigned int Engine::CurrentTime()
  {
    return ENGINE->m_time;
  }



  std::vector<GameSpace*>& Engine::Spaces()
  {
    return m_spaces;
  }

  GameSpace* Engine::CreateSpace(const char* name)
  {
    // Allocate space for a new space
    GameSpace* space = DBG_NEW GameSpace();

    space->m_name = name;

    m_spaceMap[space->m_name] = space;
    m_spaces.push_back(space);

    Lua::CallFunc(L, "gamespace.AddGameSpace", name, space);

    return space;
  }

  GameSpace* Engine::GetSpace(const char* name)
  {
    //std::string spaceName = name;
    if (m_spaceMap.find(name) != m_spaceMap.end())
      return m_spaceMap[name];
    return nullptr;
  }

  void Engine::RemoveSpace(GameSpace* space)
  {
    m_spaceMap.erase(space->m_name);
    for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
    {
      if (*it == space)
      {
        // Move the back onto the location we are deleting
        *it = m_spaces.back();
        // Pop off the back of the vector
        m_spaces.pop_back();
        break;
      }
    }

    space->m_shuttingDown = true;
    space->Cleanup();

    Lua::CallFunc(ENGINE->Lua(), "gamespace.RemoveGameSpace", space->m_name);

    delete space;
  }

  void Engine::ClearSpaces()
  {
    for (unsigned int i = 0; i < m_spaces.size(); ++i)
    {
      m_spaces[i]->Destroy();
    }
  }





  void Engine::ChangeLevel(const char* name)
  {
    m_levelChange = true;
    m_nextLevel = name;
  }

  void Engine::GotoNextLevel()
  {
    if (!m_spaces.empty())
    {
      ClearSpaces();
      CleanUp();
    }

    FACTORY->LoadLevel(m_nextLevel.c_str(), nullptr);

    m_nextLevel.clear();
    m_levelChange = false;
  }



  void Engine::CleanUp()
  {
    for (auto it = m_spaceRemoveList.begin(); it != m_spaceRemoveList.end(); ++it)
      RemoveSpace(*it);

    m_spaceRemoveList.clear();
  }





  void Engine::CheckReturnFromPIE()
  {
    if (SHEEPINPUT->Keyboard.KeyIsPressed(VK_F5))
    {
      PlayInEditor(false);
    }
  }

  void Engine::ReloadEditor()
  {
    // Wait until all of our game spaces have been deleted
    if (m_spaces.size() > 0)
      return;

    TRACELOG->Log(TraceLevel::DEBUG, "Reloading the editor.");

    std::string filePath = "cache\\spaces\\";

    boost::filesystem::directory_iterator it(filePath), eod;

    m_returnFromPIE = false;

    OpenEditor();

    BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod))
    {
      if (is_regular(p))
      {
        GameSpace* sp = FACTORY->LoadSpaceFilePath(p.string().c_str());
        sp->Tweak();
        sp->SetPaused(true);
      }
    }

    GRAPHICS->SetDefaultCam();

  }

  bool Engine::PlayingInEditor()
  {
	  return ENGINE->m_PIE;
  }

  // This is a static function!
  void Engine::PlayInEditor(bool play)
  {
    ENGINE->m_PIE = play;

    if (play)
    {
      TRACELOG->Log(TraceLevel::DEBUG, "Prepping PIE mode.");

      std::vector<GameSpace*>& gameSpaces = ENGINE->Spaces();

      std::string cacheLocation = "cache\\spaces\\";
      std::string filepath;

      // We want to clear out the cache
      boost::filesystem::remove_all(cacheLocation);

      boost::filesystem::create_directories(cacheLocation);

      // We want to cache all of our current spaces into our cache
      // so we can pull them up later
      for (size_t i = 0; i < gameSpaces.size(); ++i)
      {
        filepath = cacheLocation + gameSpaces[i]->GetName()
          + FACTORY->LevelFileExtension;
        FACTORY->SaveSpaceToFilePath(gameSpaces[i], filepath.c_str());

        //if (gameSpaces[i]->m_fileName.length() > 0)
        //  FACTORY->SaveSpaceToFile(gameSpaces[i], gameSpaces[i]->m_fileName.c_str(), true);
      }

      for (size_t i = 0; i < gameSpaces.size(); ++i)
        gameSpaces[i]->Destroy();
      
      ENGINE->m_enteringPIE = true;

      
    }
    else
    {
      TRACELOG->Log(TraceLevel::DEBUG, "PIE mode ended.");

      std::vector<GameSpace*>& gameSpaces = ENGINE->Spaces();

      // Mark all current gamespaces for deletion
      for (size_t i = 0; i < gameSpaces.size(); ++i)
        gameSpaces[i]->Destroy();
      
      ENGINE->m_PIE = false;
      ENGINE->m_returnFromPIE = true;

      

    }
  }
  
  void Engine::StartPIE()
  {
    TRACELOG->Log(TraceLevel::DEBUG, "Starting PIE mode.");

    std::string filePath = "cache\\spaces\\";

    boost::filesystem::directory_iterator it(filePath), eod;

    m_enteringPIE = false;

    BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod))
    {
      if (is_regular(p))
      {
        GameSpace* sp = FACTORY->LoadSpaceFilePath(p.string().c_str());
      }
    }
  }

}