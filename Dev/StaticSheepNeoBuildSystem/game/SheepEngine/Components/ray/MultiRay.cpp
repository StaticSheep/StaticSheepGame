#pragma once
#include "pch/precompiled.h"

#include "MultiRay.h"
#include "components/transform/CTransform.h"

namespace Framework
{

  bool MultiRayCaster(Vec3D& rayOrigin, Vec3D& rayDirection, GameSpace* space, int width,
    int collisionGroup, std::vector<MCData>& results, int resolution)
  {
    //initialization of ray
    //*************
    SheepFizz::RayConfig m_ray;
    m_ray.collisionGroup = (CollisionGroup)collisionGroup;
    m_ray.rayDirection = rayDirection;
    m_ray.rayOrigin = rayOrigin;

    m_ray.findFirstCollision = true;
    m_ray.gameSpace = space;

    //initialization of positions
    //**************
    std::vector<Vec3D> m_positionOffsets;

    float offsetVal = width / 2.0f;
    float differencePerOffset = width / (resolution - 1);

    Vec3D offsetDir = m_ray.rayDirection.CalculateNormal();
    Vec3D position = m_ray.rayOrigin + offsetDir * offsetVal;

    m_positionOffsets.push_back(position);

    for (int i = 1; i < resolution - 1; ++i)
    {
      position -= i * differencePerOffset * offsetDir;
      m_positionOffsets.push_back(position);
    }

    //caster of rays
    //**************
    bool death = false;

    for (int i = 0; i < m_positionOffsets.size(); ++i)
    {
      m_ray.rayOrigin = m_positionOffsets[i];
      death = PHYSICS->ComplexRayCast(&m_ray);
      if (death)
      {
        PHYSICS->RayDestruction((GameSpace*)m_ray.gameSpace);
        float length = (m_ray.firstCollisionLocation - m_ray.rayOrigin).Length();

        results.push_back(MCData(length, (unsigned)m_ray.firstCollisionObject));
      }
    }

    return death;
  }
}