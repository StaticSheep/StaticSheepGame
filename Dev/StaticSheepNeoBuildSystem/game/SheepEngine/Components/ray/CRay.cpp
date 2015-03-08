#pragma once
#include "pch/precompiled.h"

#include "CRay.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  void Ray::SetRayCast(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name)
  {
    PHYSICS->SetRayConfig(rayOrigin, rayDirection, name);
  }

  bool Ray::SimpleRayCast()
  {
    return PHYSICS->SimpleRayCast(space);
  }

  bool Ray::ComplexRayCast()
  {
    return PHYSICS->ComplexRayCast(space);
  }

  void Ray::RayDestruction(float damage)
  {
    PHYSICS->RayDestruction(space, damage);
  }
}