/*****************************************************************
Filename: Light.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "Vertices.h"
#include "DataTypes.h"
namespace DirectSheep
{
  class Light {
  public:
    const Vec3& getAttenuation() const;
    const Color& getColor() const;
    const Vec3& getPosition() const;

    Light();

    Light(const Vec3& position,
      const Color& color,
      const Vec3& attenuation);

  private:
    Vec3 m_position;
    Color m_color;
    Vec3 m_attenuation;
  };
}