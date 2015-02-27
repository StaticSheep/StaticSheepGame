/*****************************************************************
Filename: LELaser.h
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "LEBase.h"

namespace Framework
{
  struct LELaser : public LevelEvent
  {
    LELaser();
    ~LELaser();

    void Update(float dt);
    void FireEvent(GameObject *LogicController);

    float timer;
    GameObject *LC;
  };
}