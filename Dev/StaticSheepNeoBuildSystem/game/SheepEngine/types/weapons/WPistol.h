#pragma once
#include "WBase.h"

namespace Framework
{
  struct Pistol : public Weapon
  {
    Pistol();
    virtual ~Pistol();

    virtual void Fire();

  };
}