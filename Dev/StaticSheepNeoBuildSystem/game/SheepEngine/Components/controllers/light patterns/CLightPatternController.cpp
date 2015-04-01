/*****************************************************************
Filename: CLightPatternController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLightPatternController.h"
#include "types/space/Space.h"
#include "../round/CRoundController.h"

namespace Framework
{

  LightPatternController::LightPatternController()
  {
    
  }

  LightPatternController::~LightPatternController()
  {

  }

  void LightPatternController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(LightPatternController::LogicUpdate));

  }

  void LightPatternController::LogicUpdate(float dt)
  {

  }

  void LightPatternController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}