/*****************************************************************
Filename: CChipController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CChipController.h"
#include "types/space/Space.h"
#include "../round/CRoundController.h"


#define CHIPS_PER_ROUND 2

namespace Framework
{

  ChipController::ChipController()
  {
    for (int i = 0; i < 4; ++i)
    {
      playerChips[i] = 0;
      totalPlayerDeaths[i] = 0;
      totalPlayerKills[i] = 0;
      totalTimeAsJugg[i] = 0;
      totalPlayerLMS[i] = 0;
      roundPlayerDeaths[i] = 0;
      roundPlayerKills[i] = 0;
      roundTimeAsJugg[i] = 0;
      LMSTimeAlive[i] = 0;
    }
  }

  ChipController::~ChipController()
  {

  }

  void ChipController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ChipController::LogicUpdate));
    space->hooks.Add("GivePlayerChip", self, BUILD_FUNCTION(ChipController::GivePlayerChip));
    //space->hooks.Add("Draw", self, BUILD_FUNCTION(ChipController::Draw));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(ChipController::PlayerDied));
    space->hooks.Add("JuggDied", self, BUILD_FUNCTION(ChipController::JuggDied));
    space->hooks.Add("RoundStart", self, BUILD_FUNCTION(ChipController::ResetRoundStats));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(ChipController::RoundOver));

    space->hooks.Add("FFAAwards", self, BUILD_FUNCTION(ChipController::FFAAwards));
    space->hooks.Add("JuggAwards", self, BUILD_FUNCTION(ChipController::JuggAwards));
    space->hooks.Add("SDAwards", self, BUILD_FUNCTION(ChipController::SDAwards));

    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
    LMSThisRound = -1;
  }

  void ChipController::LogicUpdate(float dt)
  {
    DrawChips();
  }

  void ChipController::GivePlayerChip(int player, int chips)
  {
    playerChips[player] += chips;
  }

  void ChipController::DrawChips()
  {
    for (int i = 0; i < 4; ++i)
    {
      if (playerChips[i] == 0)
        continue;
      //draw sprites
    }
  }

  void ChipController::PlayerDied(int ply, int who_killed_them)
  {
    RoundController *RC = space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController);

    if (RC->state_ == RoundController::RoundState::ROUNDINPRO)
    {
      totalPlayerDeaths[ply]++;
      roundPlayerDeaths[ply]++;

      if (who_killed_them != -1)
      {
        totalPlayerKills[who_killed_them]++;
        roundPlayerKills[who_killed_them]++;
      }
    }
  }

  void ChipController::JuggDied(int jugg, float timeAsJugg)
  {
    RoundController *RC = space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController);

    /*if (RC->state_ == RoundController::RoundState::ROUNDINPRO)
    {*/
      //totalTimeAsJugg[jugg] += timeAsJugg;
      //roundTimeAsJugg[jugg] += timeAsJugg;
   // }
  }

  void ChipController::ResetRoundStats()
  {
    for (int i = 0; i < 4; ++i)
    {
      roundPlayerDeaths[i] = 0;
      roundPlayerKills[i] = 0;
      roundTimeAsJugg[i] = 0;
      LMSTimeAlive[i] = 0;
    }
    LMSThisRound = -1;
  }

  void ChipController::RoundOver()
  {
    Level1_Logic *LL = space->GetGameObject(owner)->GetComponent<Level1_Logic>(eLevel1_Logic);
    for (int i = 0; i < 4; ++i)
    {
      if (LL->juggernaut[i])
      {
        //roundTimeAsJugg[i] += LL->timeAsJugg;
        totalTimeAsJugg[i] += roundTimeAsJugg[i];
        LL->timeAsJugg = 0;
        break;
      }
    }
  }



  void ChipController::FFAAwards()
  {
    int mostKills = 0;
    for (int i = 0; i < 4; ++i)
    {
      if (roundPlayerKills[i] > mostKills)
        mostKills = roundPlayerKills[i];
    }

    for (int i = 0; i < 4; ++i)
    {
      if (roundPlayerKills[i] >= mostKills && roundPlayerKills[i] != 0)
        GivePlayerChip(i, CHIPS_PER_ROUND);
    }
  }

  void ChipController::JuggAwards()
  {
    float mostTime = 0;
    for (int i = 0; i < 4; ++i)
    {
      if (roundTimeAsJugg[i] > mostTime)
        mostTime = roundTimeAsJugg[i];
    }

    for (int i = 0; i < 4; ++i)
    {
      if (roundTimeAsJugg[i] >= mostTime && roundTimeAsJugg[i] > 0.0f)
        GivePlayerChip(i, CHIPS_PER_ROUND);
    }
  }

  void ChipController::SDAwards()
  {
    if (LMSThisRound != -1)
    {
      GivePlayerChip(LMSThisRound, CHIPS_PER_ROUND);
      //LMSThisRound = -1;
    }
  }

  int ChipController::GetPlayerChips(int player)
  {
    return playerChips[player];
  }


  void ChipController::Draw()
  {
    //Vec3 pos;
    //Vec2D scale(50, 50);
    //char playerChipsString[10];
    //for (int i = 0; i < 4; ++i)
    //{
    //  //depending on the player, it draws the totals in the correct place
    //  if (i == 0)
    //    pos = Vec3(-764, -457.0f, 0.0f);
    //  if (i == 1)
    //    pos = Vec3(572.0f, -457.0f, 0.0f);
    //  if (i == 2)
    //    pos = Vec3(572.0f, 507.0f, 0.0f);
    //  if (i == 3)
    //    pos = Vec3(-764.0f, 507.0f, 0.0f);

    //  itoa(playerChips[i], playerChipsString, 10);
    //  Draw::SetPosition(pos.x, pos.y);
    //  Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    //  Draw::SetRotation(0);
    //  Draw::DrawString(playerChipsString, scale, 1);
    //}
  }

  void ChipController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}