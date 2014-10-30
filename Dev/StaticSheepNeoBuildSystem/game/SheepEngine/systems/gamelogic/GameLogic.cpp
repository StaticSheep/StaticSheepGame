/*****************************************************************
Filename: GameLogic.cpp
Project: 
Author(s): Zachary Nawar

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include <iostream>

#include "GameLogic.h"

#include "components/lua/CLuaComponent.h"
#include "components/gamepad/CGamePad.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "components/gameplay_scripts/CElevatorPlat.h"
#include "components/gameplay_scripts/CLevel1_Logic.h"
#include "Components/sound/CSoundEmitter.h"

namespace Framework
{
  // Global pointer
  GameLogic* GAMELOGIC = NULL;

  GameLogic::GameLogic()
  {

    GAMELOGIC = this;
  }

  GameLogic::~GameLogic()
  {
    GAMELOGIC = nullptr;
    // Free anything that was allocated
  }

  void GameLogic::RegisterComponents()
  {
    REGISTER_COMPONENT(LuaComponent);
    REGISTER_COMPONENT(GamePad);
	  REGISTER_COMPONENT(PlayerController);
	  REGISTER_COMPONENT(Bullet_Default);
    REGISTER_COMPONENT(ElevatorPlat);
    REGISTER_COMPONENT(Level1_Logic);
    REGISTER_COMPONENT(SoundEmitter);
  }

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    GameSpace* space;
    std::vector<GameSpace*> removeList;

    m_debugData = { 0 };

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      if (!space->Paused())
        space->hooks.Call("LogicUpdate", dt);

      space->hooks.Call("FrameUpdate", dt);

#if SHEEP_DEBUG
      ++(m_debugData.numSpaces);
      m_debugData.objectsAllocated += space->m_objects.Size();
      for (size_t i = 0; i < ecountComponents; ++i)
      {
        m_debugData.componentsAllocated += space->m_components[i].Size();
      }
#endif
    }

    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "LogicUpdate", dt);

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      space->Cleanup();

      if (space->m_valid == false)
        removeList.push_back(space);
    }

    for (auto it = removeList.begin(); it != removeList.end(); ++it)
      ENGINE->RemoveSpace(*it);

  }

  const void* GameLogic::GetDebugData()
  {
    return &m_debugData;
  }

}