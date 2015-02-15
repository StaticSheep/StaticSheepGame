/*****************************************************************
Filename: LEAsteroids.h
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "LEBase.h"

namespace Framework
{
  struct LEAsteroids : public LevelEvent
  {
    LEAsteroids();
    ~LEAsteroids();

    void Update(float dt);
    void FireEvent(GameObject *LogicController);

    float timer;
    int numOfSpawns;
    GameObject *LC;
  };
}
