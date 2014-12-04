/*****************************************************************
Filename: WShotgun.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "WBase.h"

namespace Framework
{
  struct Shotgun : public Weapon
  {
    Shotgun();
    virtual ~Shotgun();

    virtual void Fire(GameObject*);

  };
}