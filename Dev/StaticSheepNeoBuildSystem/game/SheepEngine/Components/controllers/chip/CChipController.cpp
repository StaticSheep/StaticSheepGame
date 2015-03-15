/*****************************************************************
Filename: CChipController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CChipController.h"
#include "types/space/Space.h"

namespace Framework
{

  ChipController::ChipController()
  {
    for (int i = 0; i < 4; ++i)
    {
      playerChips[i] = 0;
    }
  }

  ChipController::~ChipController()
  {

  }

  void ChipController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ChipController::LogicUpdate));
    space->hooks.Add("GivePlayerChip", self, BUILD_FUNCTION(ChipController::GivePlayerChip));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(ChipController::Draw));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(ChipController::PlayerDied));

    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
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

  }

  void ChipController::Draw()
  {
    Vec3 pos;
    Vec2D scale(50, 50);
    char playerChipsString[10];
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-764, -457.0f, 0.0f);
      if (i == 1)
        pos = Vec3(572.0f, -457.0f, 0.0f);
      if (i == 2)
        pos = Vec3(572.0f, 507.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-764.0f, 507.0f, 0.0f);

      itoa(playerChips[i], playerChipsString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerChipsString, scale, 1);
    }
  }

  void ChipController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}