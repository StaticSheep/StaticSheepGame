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
    // Free anything that was allocated
  }

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    for (auto it = ENGINE->m_spaceMap.begin(); it != ENGINE->m_spaceMap.end(); ++it)
    {
      GameSpace* space = ENGINE->m_spaceMap.at(it->first);
      // Send update events for the space
    }

    for (auto it = ENGINE->m_spaceMap.begin(); it != ENGINE->m_spaceMap.end(); ++it)
    {
      GameSpace* space = ENGINE->m_spaceMap.at(it->first);
      space->Cleanup();
    }
  }
}