/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLobbyController.h"
#include "types/space/Space.h"
#include "CJoinText.h"
#include "../../sprites/CSprite.h"

namespace Framework
{

  LobbyController::LobbyController()
  {
    numOfPlayers = 0;
    for (int i = 4; i < 4; ++i)
    {
      playerJoined[i] = false;
    }
    state_ = SPAWN;
    spawnTimer = 1.0f;
  }

  LobbyController::~LobbyController()
  {

  }

  void LobbyController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(LobbyController::LogicUpdate));
    //space->hooks.Add("Draw", self, BUILD_FUNCTION(LobbyController::Draw));

    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
  }

  void LobbyController::Remove()
  {

  }

  void LobbyController::LogicUpdate(float dt)
  {
    GoToState(dt);
  }

  void LobbyController::GoToState(float dt)
  {
    switch (state_)
    {
    case SPAWN:
      Spawn(dt);
      break;
    case JOIN:
      Join(dt);
      break;
    case GAME_START:
      GameStart(dt);
      break;
    }
  }

  void LobbyController::Spawn(float dt)
  {

  }

  void LobbyController::Join(float dt)
  {

  }

  void LobbyController::GameStart(float dt)
  {

  }

}