/*****************************************************************
Filename: CAOEDamage.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "systems/physics/SheepPhysics.h"

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

    float m_radius = 30;

    float m_damagePerSecond = 10;
    
    bool m_shrinks = true;
    float m_shrinkTime = 0.2f;
    float m_shrinkDelay = 0.0f;

  private:
    bool m_firstRun = true;
    float m_radiusPerSecond = 0;

  };
}