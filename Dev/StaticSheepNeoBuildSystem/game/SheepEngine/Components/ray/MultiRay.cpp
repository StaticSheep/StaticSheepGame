#pragma once
#include "pch/precompiled.h"

#include "MultiRay.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  void MultiRay::SetRayCast(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name)
  {
    PHYSICS->SetRayConfig(rayOrigin, rayDirection, name);
  }

  bool MultiRay::SimpleRayCast()
  {
    return PHYSICS->SimpleRayCast(space);
  }

  bool MultiRay::ComplexRayCast()
  {
    return PHYSICS->ComplexRayCast(space);
  }

  void MultiRay::RayDestruction(float damage)
  {
    PHYSICS->RayDestruction(space, damage);
  }
}