/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
    spawned = false;

    spawnPos[0] = Vec3(-600.0f, -300.0f, 0.0f);
    spawnPos[1] = Vec3(600.0f, -300.0f, 0.0f);
    spawnPos[2] = Vec3(600.0f, 300.0f, 0.0f);
    spawnPos[3] = Vec3(-600.0f, 300.0f, 0.0f);
  }

  LobbyController::~LobbyController()
  {

  }

  void LobbyController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(LobbyController::LogicUpdate));
    //space->hooks.Add("Draw", self, BUILD_FUNCTION(LobbyController::Draw));
    bounceDownTimer = .5f;
    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
  }

  void LobbyController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
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
    if (!spawned)
    {
      //spawn "press A to join"
      for (int i = 0; i < 4; ++i)
      {
        //create join text
        GameObject *jText = (FACTORY->LoadObjectFromArchetype(space, "JoinText"));
        JoinText *joinText = jText->GetComponent<JoinText>(eJoinText);
        //set its player
        joinText->playerNum = i;
        //set handle back to this
        joinText->lobbyController_ = owner;
        //move join text to correct corner
        jText->GetComponent<Transform>(eTransform)->SetTranslation(spawnPos[i]);
      }
      spawned = true;
    }
    if (!bounceDownDone)
      BounceDown(dt);

    spawnTimer -= dt;
    if (spawnTimer <= 0)
    {
      state_ = JOIN;
    }
  }

  void LobbyController::BounceDown(float dt)
  {
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);

    if (bounceDownTimer >= 0.3f)
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -80.0f, 0.0f));
    else if (bounceDownTimer >= 0.2)
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 40.0f, 0.0f));
    }
    else
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -20.0f, 0.0f));
    }

    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      //rt->SetRotation(0.0f);
      bounceDownDone = true;
    }
  }

  void LobbyController::Join(float dt)
  {
    if (startPressed)
    {
      state_ = GAME_START;
      timer_ = 0.5f;
      space->hooks.Call("GameStart");
    }
  }

  void LobbyController::GameStart(float dt)
  {
    //roll up lobby text
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 80.0f, 0.0f));

    timer_ -= dt;
    if (timer_ <= 0)
    {
      space->GetGameObject(owner)->Destroy();
    }

  }

}