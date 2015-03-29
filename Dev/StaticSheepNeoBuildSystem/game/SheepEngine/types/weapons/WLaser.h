#pragma once
#include "WBase.h"

namespace Framework
{
  struct WLaser : public Weapon
  {
    WLaser();
    virtual ~WLaser();

    virtual void Fire(GameObject*);
    virtual void Update(float dt);
    virtual void DelayUpdate(float dt);
    virtual void ResetDelay();

    float life;
    bool active;

  };
}