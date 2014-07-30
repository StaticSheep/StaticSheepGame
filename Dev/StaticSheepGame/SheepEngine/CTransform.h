/*****************************************************************
Filename: CTransform.h
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Vec2
  {
  public:
    Vec2::Vec2(float _x, float _y)
    {
      x = _x;
      y = _y;
    }

    Vec2::Vec2()
    {
    }

    float x;
    float y;
  };

  class Transform : public GameComponent
  {
  public:
    Transform();
    ~Transform();
    virtual void Initialize(){};

    Vec2 position;
    float rotation;
  };
}