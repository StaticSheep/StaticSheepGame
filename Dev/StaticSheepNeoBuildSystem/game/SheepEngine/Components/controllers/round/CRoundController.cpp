/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundController.h"
#include "types/space/Space.h"
#include "CRoundText.h"
#include "../../sprites/CSprite.h"
#include "../chip/CChipController.h"
#include "CRoundResults.h"

namespace Framework
{

  RoundController::RoundController()
  {
    current_round = 1;
    max_rounds = 6; //default value
    spawned_round_start = false;
    timeOfRound = 93.0f; //default round length, (round length + 3.0f)
    state_ = INTRO;
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
    ChipController_ = space->GetGameObject(owner)->GetComponentHandle(eChipController);
    round_state_timer = 5.0f;
    EORAwarded = false;


    font_index = Draw::GetFontIndex("BN_Jinx");
  }

  void RoundController::LogicUpdate(float dt)
  {
    GoToState(dt);
  }

  void RoundController::GoToState(float dt)
  {
    switch (state_)
    {
    case INTRO:
      IntroSequence(dt);
      break;
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
    case IDLE_WAIT:
      break;
    }
  }

  void RoundController::IntroSequence(float dt)
  {
    //light-up sequence
    //Welcome to the Games!
    if (!spawned_round_start)
    {
      GameObject *welcome = (FACTORY->LoadObjectFromArchetype(space, "welcome_text"));
      GameObject *toThe = (FACTORY->LoadObjectFromArchetype(space, "ToThe_text"));
      GameObject *games = (FACTORY->LoadObjectFromArchetype(space, "games_text"));
      welcome->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, 96.0f, 0.0f));
      toThe->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 0.0f, 0.0f));
      games->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -96.0f, 0.0f));
      spawned_round_start = true;
    }

    round_state_timer -= dt;
    if (round_state_timer <= 0)
    {
      //change the level logic state to spawn the lobby
      space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->mode = LOBBY;
      //set the round controller to wait until lobby is done
      state_ = IDLE_WAIT;
      spawned_round_start = false;
      round_state_timer = 2.0f;
    }

  }

  void RoundController::RoundStart(float dt)
  {
    if (!spawned_round_start)
    {
      GameObject *roundText = (FACTORY->LoadObjectFromArchetype(space, "round_text"));
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      roundText->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));
      roundText->GetComponent<RoundText>(eRoundText)->LeftToRight = true;
      roundText->GetComponent<RoundText>(eRoundText)->text = true;
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = current_round;
      spawned_round_start = true;
      ResultsSpawned = false;
      space->hooks.Call("RoundStart");
    }
    round_state_timer -= dt;
    if (round_state_timer <= 0)
    {
      state_ = ROUNDINPRO;
      slotMachineDone = false;
      EORAwarded = false;
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

    itoa((int)round_state_timer + 1, round_timer, 10);


    if (round_state_timer <= 0)
    {
      state_ = ROUNDOVER;
      round_state_timer = 10.0f;
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
    if (round_state_timer >= 8.0f && !roundUp_spawned)
    {
      //display "round over"
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "roundUp_text"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->LeftToRight = true;
      round_number->GetComponent<RoundText>(eRoundText)->text = true;
      roundUp_spawned = true;
      space->hooks.Call("RoundOver");
    }
    else if (round_state_timer >= 6.0f && round_state_timer <= 6.8f)
    {
      //display results
      if (!ResultsSpawned)
      {

        GameObject *ResultsTV = (FACTORY->LoadObjectFromArchetype(space, "ResultsTV"));
        ResultsTV->GetComponent<RoundResults>(eRoundResults)->ChipCont = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->owner;
        ResultsTV->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0f, 950.0f, -1.0f));
        ResultsTV->GetComponent<RoundResults>(eRoundResults)->mode_ = mode_;
        ResultsSpawned = true;
        space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->ResetPlayers();
        space->hooks.Call("DestroyPickups");
      }
      
      if (!EORAwarded)
        AwardEndOfRoundChips();
    }

    round_state_timer -= dt;

    if (round_state_timer <= 0)
    {
      current_round++;
      if (current_round > max_rounds)
      {
        state_ = GAMEOVER;
        space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic)->mode = GameTypes::GAMEOVER;
        spawned_round_start = false;
      }
      else
        state_ = ROUNDSTART;

      round_state_timer = 2.0f;
      ResultsSpawned = false;
    }

  }

  void RoundController::GameOver(float dt)
  {
    //spawn game over
    if (!spawned_round_start)
    {
      GameObject *roundText = (FACTORY->LoadObjectFromArchetype(space, "gameOver_text"));
      roundText->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));
      spawned_round_start = true;
      round_state_timer = 23.0f;
      return;
    }

    //spawn tv
    if (!ResultsSpawned && round_state_timer <= 90.0f)
    {
      GameObject *ResultsTV = (FACTORY->LoadObjectFromArchetype(space, "ResultsTV"));
      ResultsTV->GetComponent<RoundResults>(eRoundResults)->ChipCont = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->owner;
      ResultsTV->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0f, 950.0f, -1.0f));
      ResultsTV->GetComponent<RoundResults>(eRoundResults)->mode_ = GameTypes::GAMEOVER;
      ResultsSpawned = true;
    }
    
    round_state_timer -= dt;

    if (round_state_timer <= 0)
    {
      ENGINE->ChangeLevel("Asteroid");
    }
  }

  void RoundController::SlotMachineDone(GameTypes mode)
  {
    slotMachineDone = true;
    state_ = ROUNDINPRO;
    round_state_timer = timeOfRound;
    mode_ = mode;
  }

  void RoundController::RoundCountDown()
  {
    Sprite *ls = space->GetGameObject(owner)->GetComponent<Sprite>(eSprite);
    //run countdown
    if (round_state_timer <= 5.0f && round_state_timer > 4.0f && !num_spawned[4])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 5;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[4] = true;
    }
    else if (round_state_timer <= 4.0f && round_state_timer > 3.0f && !num_spawned[3])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 4;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[3] = true;
    }
    else if (round_state_timer <= 3.0f && round_state_timer > 2.0f && !num_spawned[2])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 3;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[2] = true;
    }
    else if (round_state_timer <= 2.0f && round_state_timer > 1.0f && !num_spawned[1])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 2;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[1] = true;
    }
    else if (round_state_timer <= 1.0f && round_state_timer > 0.0f && !num_spawned[0])
    {
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 1;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[0] = true;
    }

  }

  void RoundController::AwardEndOfRoundChips()
  {
    ChipController *CC = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController);
    switch (mode_)
    {
    case FFA:
      CC->FFAAwards();
      break;
    case JUGGERNAUT:
      CC->JuggAwards();
      break;
    case SUDDENDEATH:
      CC->SDAwards();
      break;
    case BONUSMODE:
      break;
    }
    EORAwarded = true;
  }

  void RoundController::Draw()
  {
    if (state_ == ROUNDINPRO)
    {
      Vec2D scale(60, 60);
      Vec3 pos(-100.0f, 600.0f, 0.0f);
      char playerString[10];

      sprintf(playerString, "%5.2f", round_state_timer);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);

      if (round_state_timer > timeOfRound - 3)
        itoa(timeOfRound - 3, round_timer, 10);

      scale.x = 90;
      scale.y = 90;
      Draw::SetPosition(-60.0f, 510.0f);

      if (round_state_timer < 9)
        Draw::SetPosition(-30.0f, 510.0f);

      else if (round_state_timer < 19)
        Draw::SetPosition(-50.0f, 510.0f);

      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(round_timer, scale, font_index);
    }

  }

  void RoundController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}