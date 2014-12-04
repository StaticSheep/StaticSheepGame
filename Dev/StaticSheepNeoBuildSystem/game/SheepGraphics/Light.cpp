/*****************************************************************
Filename: Light.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Light.h"
namespace DirectSheep
{
  const Vec3& Light::getAttenuation() const
  {
    return m_attenuation;
  }

  const Color& Light::getColor() const
  {
    return m_color;
  }

  const Vec3& Light::getPosition() const
  {
    return m_position;
  }

  Light::Light() : m_attenuation(), m_color(), m_position()
  {
  }

  Light::Light(const Vec3& position,
    const Color& color,
    const Vec3& attenuation) : 

    m_attenuation(attenuation), 
    m_color(color), 
    m_position(position)
  {}

}