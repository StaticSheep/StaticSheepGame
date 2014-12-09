/*****************************************************************
Filename: Vec3.h
Project:
Author(s): Jon Sourbeer (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "Vec3D.h"

namespace Framework
{
  class Vec3 : public Vec3D
  {
  public:
    Vec3(float x = 0, float y = 0, float z = 0): Vec3D(x,y,z) { }
    Vec3(Vec3D rhs) :Vec3D(rhs) {}
    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);

    
  };
}