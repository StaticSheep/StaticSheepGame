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
  }

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    GameSpace* space;
    std::vector<GameSpace*> removeList;

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->Paused())
        space->hooks.Call("LogicUpdate", dt);

      space->hooks.Call("FrameUpdate", dt);
    }

    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "LogicUpdate", dt);

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;
      space->Cleanup();

      if (space->m_valid == false)
        removeList.push_back(space);
    }

    for (auto it = removeList.begin(); it != removeList.end(); ++it)
      ENGINE->RemoveSpace(*it);

  }


}