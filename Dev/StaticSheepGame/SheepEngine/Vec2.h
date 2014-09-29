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
    Vec2::Vec2(float _x = 0, float _y = 0)
    {
      X = _x;
      Y = _y;
    }

    static void ToLua(lua_State* L, Variable& var);
    static void FromLua(lua_State* L, int index, Variable* var);


    union
    {
      float x_;
      float X;
    };

    union
    {
      float y_;
      float Y;
    };

  };
}