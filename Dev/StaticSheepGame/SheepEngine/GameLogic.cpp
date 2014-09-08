/*****************************************************************
Filename: GameLogic.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "GameLogic.h"
#include "CGamePad.h"
#include <iostream>

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
  }

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    GameSpace* space;

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
    }

  }


}