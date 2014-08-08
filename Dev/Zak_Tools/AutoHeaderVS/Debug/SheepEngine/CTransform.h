/*****************************************************************
Filename: CTransform.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  

  class Transform : public GameComponent
  {
  public:
    Transform();
    ~Transform();
    virtual void Initialize(){};

    Vec2 translation;
    Vec2 scale;
    float rotation;
  };
}