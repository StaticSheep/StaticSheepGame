#pragma once
#include "Vertices.h"
#include "DataTypes.h"
namespace DirectSheep
{
  class Light {
  public:


    Light(const Vec3& position,
      const Color& color,
      const Vec3& attenuation);

  private:
    Vec3 m_position;
    Color m_color;
    Vec3 m_attenuation;
  };
}
