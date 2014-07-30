/*****************************************************************
Filename: Vec2.h
Project: 
Author(s):

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Vec2
  {
  public:
    Vec2::Vec2(float _x, float _y)
    {
      X = _x;
      Y = _y;
    }

    Vec2::Vec2()
    {
    }

    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);

    float X;
    float Y;
  };
}