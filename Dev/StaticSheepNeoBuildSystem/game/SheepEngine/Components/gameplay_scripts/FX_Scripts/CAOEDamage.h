/*****************************************************************
Filename: CAOEDamage.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "SheepPhysics/step.h"

namespace Framework
{
  class AOEDamage : public GameComponent
  {
  public:
    AOEDamage();
    ~AOEDamage();

    void Initialize();

    void LogicUpdate(float dt);

    void OnCollision(Handle otherObject,
      SheepFizz::ExternalManifold manifold);

    void Remove();

  };
}