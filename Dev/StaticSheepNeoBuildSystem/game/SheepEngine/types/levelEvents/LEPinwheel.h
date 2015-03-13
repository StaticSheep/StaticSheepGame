/*****************************************************************
Filename: LEPinwheel.h
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "LEBase.h"

namespace Framework
{
  struct LEPinwheel : public LevelEvent
  {
    LEPinwheel();
    ~LEPinwheel();

    void Update(float dt);
    void FireEvent(GameObject *LogicController);

    float timer;
    GameObject *LC;
  };
}