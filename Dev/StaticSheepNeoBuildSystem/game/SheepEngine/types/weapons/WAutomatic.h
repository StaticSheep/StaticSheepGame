/*****************************************************************
Filename: WAutomatic.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "WPistol.h"

namespace Framework
{
  struct Automatic : public Pistol
  {
    Automatic();
    virtual ~Automatic();

    virtual void Fire(GameObject*);
    virtual void DelayUpdate(float dt);
    virtual void ResetDelay();
  };
}