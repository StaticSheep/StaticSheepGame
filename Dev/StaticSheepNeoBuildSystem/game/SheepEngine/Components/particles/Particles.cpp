/******************************************************************************
Filename: Particles.cpp
Project:  GAM 250
Author(s): Zakary Wilson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "Particles.h"
#include "../SheepUtil/include/SheepMath.h"

namespace Framework
{
  Particle::Particle()
  {
    position = Vec3();
    scale = 1.0f;
    endScale = 1.0f;
    color = Vec4();
    endColor = Vec4();
    direction = Vec3();
    endDirection = Vec3();
    speed = 0.0f;
    endSpeed = 0.0f;
    theta = 0.0f;
    life = 1.0f;
    endLife = 1.0f;
  }

  Particle::Particle(ParticleOption<float>& scale_,
                     ParticleOption<Vec4>&  color_,
                     ParticleOption<Vec3>&  dir_,
                     ParticleOption<float>& speed_,
                     ParticleOptionShort<float>& life_)
  {
    scale = GetRandom(scale_.m_startMin, scale_.m_startMax);
    currentScale = scale;
    endScale = GetRandom(scale_.m_endMin, scale_.m_endMax);

    color = GetRandom(color_.m_startMin, color_.m_startMax);
    currentColor = color;
    endColor = GetRandom(color_.m_endMin, color_.m_endMax);

    direction = GetRandomNormalizedVector(dir_.m_startMin, dir_.m_startMax);
    currentDirection = direction;
    endDirection = GetRandomNormalizedVector(dir_.m_endMin, dir_.m_endMax);

    speed = GetRandom(speed_.m_startMin, speed_.m_startMax);
    currentSpeed = speed;
    endSpeed = GetRandom(speed_.m_endMin, speed_.m_endMax);

    life = GetRandom(life_.m_startMin, life_.m_startMax);
    endLife = life;

    theta = GetRandom(0.0f, 2.0f * PI);
  }

}