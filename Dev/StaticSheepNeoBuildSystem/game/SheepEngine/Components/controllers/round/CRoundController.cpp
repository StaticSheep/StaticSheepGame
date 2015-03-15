/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundController.h"
#include "types/space/Space.h"
#include "../../gameplay_scripts/Level_Scripts/CLevel1_Logic.h"
#include "CRoundText.h"
#include "../../sprites/CSprite.h"

namespace Framework
{

  RoundController::RoundController()
  {
    current_round = 1;
    max_rounds = 6; //default value
    spawned_round_start = false;
  }

  RoundController::~RoundController()
  {

  }

  void RoundController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(RoundController::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(RoundController::Draw));
    space->hooks.Add("SlotFinished", self, BUILD_FUNCTION(RoundController::SlotMachineDone));

    for (int i = 0; i < 4; ++i)
      num_spawned[i] = false;

    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
    round_state_timer = 2.0f;
  }

  void RoundController::LogicUpdate(float dt)
  {
    GoToState(dt);
  }

  void RoundController::GoToState(float dt)
  {
    switch (state_)
    {
    case ROUNDSTART:
      RoundStart(dt);
      break;
    case ROUNDINPRO:
      RoundInProgress(dt);
      break;
    case ROUNDOVER:
      RoundOver(dt);
      break;
    case GAMEOVER:
      GameOver(dt);
      break;
    }
  }

  void RoundController::RoundStart(float dt)
  {
    if (!spawned_round_start)
    {
      (FACTORY->LoadObjectFromArchetype(space, "round_text"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = current_round;
      spawned_round_start = true;
    }
    round_state_timer -= dt;
    if (round_state_timer <= 0)
    {
      state_ = ROUNDINPRO;
      slotMachineDone = false;
      space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->roundStart = true;
      for (int i = 0; i < 4; ++i)
        num_spawned[i] = false;
    }
  }

  void RoundController::RoundInProgress(float dt)
  {
    if (!slotMachineDone)
      return;

    round_state_timer -= dt;
    if (round_state_timer <= 0)
    {
      state_ = ROUNDOVER;
      round_state_timer = 7.0f;
      space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->roundStart = false;
      space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->mode = SLOTMACHINE;
      spawned_round_start = false;
      roundUp_spawned = false;
    }
    else if (round_state_timer <= 6.0f)
    {
      RoundCountDown();
    }
  }

  void RoundController::RoundOver(float dt)
  {
    if (round_state_timer >= 5.0f && !roundUp_spawned)
    {
      //display "round over"
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "roundUp_text"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));
      roundUp_spawned = true;
    }
    else
    {
      //display results
      //award stars
    }

    round_state_timer -= dt;

    if (round_state_timer <= 0)
    {
      current_round++;
      if (current_round > max_rounds)
        state_ = GAMEOVER;
      else
        state_ = ROUNDSTART;
      round_state_timer = 2.0f;
    }

  }

  void RoundController::GameOver(float dt)
  {
    //display winner
    //wait for start button
    //restart game
    round_state_timer -= dt;

    if (round_state_timer <= 0)
    {
      //manual restart
    }
  }

  void RoundController::SlotMachineDone(GameTypes mode)
  {
    slotMachineDone = true;
    state_ = ROUNDINPRO;
    round_state_timer = 20.0f;
  }

  void RoundController::RoundCountDown()
  {
    Sprite *ls = space->GetGameObject(owner)->GetComponent<Sprite>(eSprite);
    //run countdown
    if (round_state_timer <= 5.0f && round_state_timer > 4.0f && !num_spawned[4])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = 5;
      num_spawned[4] = true;
    }
    else if (round_state_timer <= 4.0f && round_state_timer > 3.0f && !num_spawned[3])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = 4;
      num_spawned[3] = true;
    }
    else if (round_state_timer <= 3.0f && round_state_timer > 2.0f && !num_spawned[2])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = 3;
      num_spawned[2] = true;
    }
    else if (round_state_timer <= 2.0f && round_state_timer > 1.0f && !num_spawned[1])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = 2;
      num_spawned[1] = true;
    }
    else if (round_state_timer <= 1.0f && round_state_timer > 0.0f && !num_spawned[0])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->roundNum = 1;
      num_spawned[0] = true;
    }

  }

  void RoundController::Draw()
  {
    
  }

  void RoundController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}