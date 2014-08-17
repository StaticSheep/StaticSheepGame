/*****************************************************************
Filename: CTransform.h
*****************************************************************/

#pragma once

namespace Framework
{
  

  class Transform : public GameComponent
  {
  public:

    Vec2 translation;
    Vec2 scale;
    float rotation;
  };
}
