/*****************************************************************
Filename: WBase.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "..\object\Object.h"

namespace Framework
{
  struct PowerUp
  {
    PowerUp() {};
    virtual ~PowerUp() {};
    virtual void Use(GameObject *player) = 0;
    virtual void Update(float dt) = 0;

    bool inUse;  //has the powerup been activated or used? if false the player will delete the powerup
    float timer; //timer for how long the powerup is in effect. -1 if infinite time
    float uses;  //how many uses until the powerup is gone. -1 for infinite uses

  };
}