/*****************************************************************
Filename: Vec4.h
Project: 
Author(s):

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Vec4
  {
  public:
    Vec4::Vec4(float _x = 0, float _y = 0, float _z = 0, float _a = 0)
    {
      X = _x;
      Y = _y;
      Z = _z;
      A = _a;
    }

    //static void ToLua(lua_State* L, Variable& var);
    //static void FromLua(lua_State* L, int index, Variable* var);

    union
    {
      float X;
      float R;
    };

    union
    {
      float Y;
      float G;
    };

    union
    {
      float Z;
      float B;
    };

    float A;
  };
}