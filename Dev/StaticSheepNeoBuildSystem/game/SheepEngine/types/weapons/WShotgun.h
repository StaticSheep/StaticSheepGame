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