/*****************************************************************
Filename: LEGrinderBig.h
Project:  Gam250
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "LEBase.h"

namespace Framework
{
  struct LEGrinderBig : public LevelEvent
  {
    LEGrinderBig();
    ~LEGrinderBig();

    void Update(float dt);
    void FireEvent(GameObject *LogicController);
    void FireEventSmall(GameObject *LogicController);

    void Warn();

    float timer;
    bool warned = false;
    bool spawnLeft = false;
    GameObject *LC;
  };
}
