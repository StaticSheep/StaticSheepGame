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
  struct Pistol : public Weapon
  {
    Pistol();
    virtual ~Pistol();

    virtual void Fire(GameObject*);

  };
}