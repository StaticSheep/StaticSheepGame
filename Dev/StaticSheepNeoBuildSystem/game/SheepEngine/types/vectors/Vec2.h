/*****************************************************************
Filename: Vec2.h
Project: 
Author(s):

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec3D.h"
#include "Vertices.h"

namespace Framework
{
  class Vec2 : public Vec2D
  {
  public:
    Vec2(float x = 0, float y = 0) : Vec2D(x, y) { }
    Vec2(Vec2D rhs) :Vec2D(rhs) {}
    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);

    operator DirectSheep::Vec2()
    {
      return DirectSheep::Vec2((float*)this);
    }

    Vec2 operator&(const Vec2& rhs) const
    {
      Vec2 out = *this;
      out.x *= rhs.x;
      out.y *= rhs.y;
      return out;
    }

    bool operator>(const Vec2D& rhs) const
    {
      if (x > rhs.x && y > rhs.y)
        return true;

      return false;
    }

    bool operator<(const Vec2D& rhs) const
    {
      if (x < rhs.x && y < rhs.y)
        return true;

      return false;
    }

    bool operator>=(const Vec2D& rhs) const
    {
      if (x >= rhs.x && y >= rhs.y)
        return true;

      return false;
    }

    bool operator<=(const Vec2D& rhs) const
    {
      if (x <= rhs.x && y <= rhs.y)
        return true;

      return false;
    }
  };
}