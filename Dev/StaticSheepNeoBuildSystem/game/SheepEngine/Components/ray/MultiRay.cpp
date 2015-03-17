#pragma once
#include "pch/precompiled.h"

#include "MultiRay.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  MultiRay::MultiRay(Vec3D& rayOrigin, Vec3D& rayDirection, void* space, int width, 
    int collisionGroup, std::vector<MCData>& results, int resolution)
  {
    m_ray.collisionGroup = (CollisionGroup)collisionGroup;
    m_ray.rayDirection = rayDirection;
    m_ray.rayOrigin = rayOrigin;

    m_ray.findFirstCollision = true;
    m_ray.gameSpace = space;

    m_width = width;
    m_resolution = resolution;
    m_results = &results;
  }

  void MultiRay::Initialize()
  {
    float offsetVal = m_width / 2.0f;
    float differencePerOffset = m_width / (m_resolution - 1);
    
    Vec3D offsetDir = m_ray.rayDirection.CalculateNormal();

    Vec3D position = m_ray.rayorigin + offsetDir * offsetVal;

    m_positionOffsets.push_back(position);

    for (int i = 1; i < m_resolution - 1; ++i)
    {
      position -= i * differencePerOffset * offsetDir;
      m_positionOffsets.push_back(position);
    }

  }

  void MultiRay::SetWidth(int width)
  {
    m_width = width;
  }

  bool MultiRay::ComplexCaster()
  {
    bool death = false;

    for (int i = 0; i < m_positionOffsets.size(); ++i)
    {
      m_ray.rayOrigin = m_positionOffsets[i];
      death = PHYSICS->ComplexRayCast(m_ray);
      if (death)
      {
        PHYSICS->RayDestruction(m_ray.gameSpace);
        float length = (m_ray.firstCollisionLocation - m_ray.rayOrigin).Length();

        m_results->push_back(MCData(length, m_ray.firstCollisionObject));
      }
    }

    return death;
  }
}