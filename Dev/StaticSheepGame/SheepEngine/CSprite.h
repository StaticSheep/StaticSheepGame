
#pragma once

#include "CTransform.h"
namespace Framework
{
  class Transform;

  class Sprite : public GameComponent
  {
  public:

    Sprite();
    ~Sprite();

    virtual void Initialize();

    Vec2 Size;

    Handle transform;

    float time;

    std::string SpriteName;

    void Draw();
  };
}