/*****************************************************************
Filename: Vec4.h
Project: 
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "Vec4D.h"
#include "Vertices.h"

namespace Framework
{
  class Vec4 : public Vec4D
  {
  public:
    Vec4(float x = 0, float y = 0, float z = 0, float w = 0) : Vec4D(x, y, z, w) { }
    Vec4(Vec4D rhs) :Vec4D(rhs) {}

    operator DirectSheep::Vec4()
    {
      return DirectSheep::Vec4((float*)this);
    }

    operator DirectSheep::Color()
    {
      return DirectSheep::Color((float*)this);
    }

  };

  class LightColor : public Vec4
  {
  public:
    LightColor(float x = 0, float y = 0, float z = 0, float w = 0)
      : Vec4(x, y, z, w) { }
    LightColor(const Vec4& rhs)
      :Vec4(rhs) {}
  };
}