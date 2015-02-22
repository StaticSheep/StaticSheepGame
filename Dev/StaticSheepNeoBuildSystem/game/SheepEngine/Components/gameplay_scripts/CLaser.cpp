/*****************************************************************
Filename: CLaser.cpp
Project:  Giga Gravity Games
Author(s): Jon Sourbeer (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLaser.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CCircleCollider.h"

namespace Framework
{
  Laser::Laser() : startDelay(0), duration(1), damage(1), width(1), arcRotation(0), arcDelay(0)
  {
    if (duration > arcDelay)
      arcPerSec = arcRotation / (duration - arcDelay);
  }

  Laser::~Laser()
  {

  }

  void Laser::Initialize()
  {
    //logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Laser::LogicUpdate));

    lTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    lCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);

    Transform *lt = space->GetHandles().GetAs<Transform>(lTransfrom);
    CircleCollider *lc = space->GetHandles().GetAs <CircleCollider>(lCollider);

    Vec3D direction = lc->GetBodyRotationAsVector();

    lc->SetRayCast(lc->GetBodyPosition(), direction, "Collide");

    if (duration > arcDelay)
      arcPerSec = arcRotation / (duration - arcDelay);
    else
      arcPerSec = 0;

    if (type)
      Caster = &Framework::Laser::ComplexCaster;
    else
      Caster = &Framework::Laser::SimpleCaster;

    if (width > 4)
    {
      Vec3D offsetDir = direction.CalculateNormal();
      int numberOfRays = width / 4;

      for (int i = 1; i < numberOfRays + 1; ++i)
      {
        positionOffsets.push_back(offsetDir * 2 * i);
        positionOffsets.push_back(-offsetDir * 2 * i);
      }

      if (width % 4 != 0)
      {
        float valueOffset = ((float)width) / 2.0f;
        positionOffsets.push_back(offsetDir * valueOffset);
        positionOffsets.push_back(-offsetDir * valueOffset);
      }
    }
  }

  void Laser::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void Laser::LogicUpdate(float dt)
  {
    Transform *lt = space->GetHandles().GetAs<Transform>(lTransfrom);
    CircleCollider *lc = space->GetHandles().GetAs <CircleCollider>(lCollider);

    if (startDelay > 0)
    {
      startDelay -= dt;
      return;
    }

    if (duration < 0)
      space->GetGameObject(owner)->Destroy();

    duration -= dt;
    arcDelay -= dt;

    if (arcDelay < 0)
      (this->*Caster)();

    float curRotation = lc->GetBodyRotation();
    curRotation += arcPerSec;
    lc->SetBodyRotation(curRotation);
  }

  void Laser::SimpleCaster(void)
  {

  }

  void Laser::ComplexCaster(void)
  {

  }

}