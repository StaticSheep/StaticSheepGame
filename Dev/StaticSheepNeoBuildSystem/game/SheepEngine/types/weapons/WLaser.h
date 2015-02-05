#pragma once
#include "WBase.h"

namespace Framework
{
  struct Laser : public Weapon
  {
    Laser();
    virtual ~Laser();

    virtual void Fire(GameObject*);

  };
}