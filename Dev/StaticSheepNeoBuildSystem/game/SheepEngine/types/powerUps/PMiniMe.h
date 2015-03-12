/*****************************************************************
Filename: WPistol.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "PBase.h"

namespace Framework
{
  struct MiniMe : public PowerUp
  {
    MiniMe();
    ~MiniMe();

    virtual void Use(GameObject *player);
    virtual void Update(float dt);

    float timer; //timer for how long the powerup is in effect. -1 if infinite time
    float uses;  //how many uses until the powerup is gone. -1 for infinite uses
  };
}