/*****************************************************************
Filename: Vec3.h
Project:
Author(s): Jon Sourbeer (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "Vec3D.h"
#include "Vertices.h"

namespace Framework
{
  class Vec3 : public Vec3D
  {
  public:
    Vec3(float x = 0, float y = 0, float z = 0): Vec3D(x,y,z) { }
    Vec3(Vec3D rhs) :Vec3D(rhs) {}
    Vec3(Vec2 rhs) : Vec3D(rhs.x, rhs.y) {}
    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);
    
    operator DirectSheep::Vec3()
    {
      return DirectSheep::Vec3((float*)this);
    }
    
  };

  class LightVec3 : public Vec3
  {
  public:
    LightVec3(float x = 0, float y = 0, float z = 0) : Vec3(x, y, z) { }
    LightVec3(const Vec3& rhs) : Vec3(rhs) {}

  };
}