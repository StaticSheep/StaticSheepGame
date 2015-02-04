/*****************************************************************
Filename: Particles.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/


#pragma once
#include "types/vectors/Vec3.h"
#include "types/vectors/Vec4.h"

namespace Framework
{
  enum Eases
  {
    EaseNone = 0,
    EaseLinear,
    EaseQuadraticIn,
    EaseQuadraticOut,
    EaseCubicIn,
    EaseCubicOut,
    Catspline
  };

  // options for antweak bar
  template<typename T>
  struct ParticleOptionShort
  {
    T m_startMin;
    T m_startMax;
  };

  // options for antweak bar
  template<typename T>
  struct ParticleOption
  {
    T m_startMin;
    T m_startMax;
    T m_endMin;
    T m_endMax;
  };

  // contains all data for manipulating particles
  struct Particle
  {
    Particle();
    Particle(ParticleOption<float>& scale,
             ParticleOption<Vec4>&  color,
             ParticleOption<Vec3>&  dir,
             ParticleOption<float>& speed,
             ParticleOptionShort<float>& life);

    // scale, color, direction and speed have three data members for each
    // for interpolating between values. May need to cut this down later
    // if the complex interpolation functions are not wanted/needed.

    Vec3 position;
    float scale;
    float currentScale;
    float endScale;
    Vec4 color;
    Vec4 currentColor;
    Vec4 endColor;
    Vec3 direction;
    Vec3 currentDirection;
    Vec3 endDirection;
    float speed;
    float currentSpeed;
    float endSpeed;
    float theta;
    float life;
    float endLife;
  };
}