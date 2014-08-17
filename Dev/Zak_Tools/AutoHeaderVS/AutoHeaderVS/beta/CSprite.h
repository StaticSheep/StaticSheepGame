
#pragma once

#include "CTransform.h"
namespace Framework
{
  class Transform;

  class Sprite : public GameComponent
  {
  public:



    Vec2 Size;

    Handle transform;

    float time;

    std::string SpriteName;
    int SpriteID;


  private:
  };
}
