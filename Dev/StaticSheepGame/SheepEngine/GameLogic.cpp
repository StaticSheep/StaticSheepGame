/*****************************************************************
Filename: GameLogic.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "GameLogic.h"

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

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    GameSpace* space;

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;
      space->hooks.Call("LogicUpdate", dt);
    }

    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "LogicUpdate", dt);

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;
      space->Cleanup();
    }
  }
}