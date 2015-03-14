/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CStarController.h"
#include "types/space/Space.h"

namespace Framework
{

  StarController::StarController()
  {
    for (int i = 0; i < 4; ++i)
    {
      playerStars[i] = 0;
    }
  }

  StarController::~StarController()
  {

  }

  void StarController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(StarController::LogicUpdate));

    LevelLogic = space->GetGameObject(owner)->GetComponentHandle(eLevel1_Logic);
  }

  void StarController::LogicUpdate(float dt)
  {
    DrawStars();
  }

  void StarController::GivePlayerStar(int player, int stars)
  {
    playerStars[player] += stars;
  }

  void StarController::DrawStars()
  {

  }

  void StarController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}