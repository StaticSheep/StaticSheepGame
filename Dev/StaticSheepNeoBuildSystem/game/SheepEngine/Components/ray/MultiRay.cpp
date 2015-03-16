#pragma once
#include "pch/precompiled.h"

#include "MultiRay.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  MultiRay::MultiRay(Vec3D& rayOrigin, Vec3D& rayDirection, void* space, int width, int collisionGroup, std::vector<MCData>& results, int resolution)
  {
    m_ray.collisionGroup = (CollisionGroup)collisionGroup;
    m_ray.rayDirection = rayDirection;
    m_ray.rayOrigin = rayOrigin;

    m_ray.findFirstCollision = true;
    m_ray.gameSpace = space;


  }

  void MultiRay::Initialize()
  {


  }

  void MultiRay::SetWidth(int width)
  {
    m_width = width;
  }

  void MultiRay::ComplexCaster()
  {
    bool death = false;

    for (int i = 0; i < positionOffsets.size(); ++i)
    {
      PHYSICS->SetRayConfig(positionOffsets[i], direction, "RayCast");
      death = PHYSICS->ComplexRayCast(space, m_ray);
      if (death)
      {
        PHYSICS->RayDestruction(space);
        m_beamLengths.push_back((Vec3D(PHYSICS->GetFirstCollision()) - trans->GetTranslation()).Length());
      }
      else
        m_beamLengths.push_back(-1);
    }
  }

  void MultiRay::RayDestruction()
  {
    PHYSICS->RayDestruction(space);
  }
}