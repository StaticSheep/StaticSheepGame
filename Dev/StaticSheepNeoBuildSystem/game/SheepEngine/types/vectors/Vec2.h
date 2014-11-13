/*****************************************************************
Filename: Vec2.h
Project: 
Author(s):

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec3D.h"

namespace Framework
{
  class Vec2 : public Vec2D
  {
  public:
    Vec2(float x = 0, float y = 0) : Vec2D(x, y) { }
    Vec2(Vec2D rhs) :Vec2D(rhs) {}
    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);
  };
}