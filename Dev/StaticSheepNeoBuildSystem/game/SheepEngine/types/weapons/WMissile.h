/*****************************************************************
Filename: WPistol.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "WBase.h"

namespace Framework
{
  struct Missile : public Weapon
  {
    Missile();
    virtual ~Missile();

    virtual void Fire(GameObject*);
    virtual void DelayUpdate(float dt);
    virtual void Update(float dt);
    virtual void ResetDelay();
  };
}