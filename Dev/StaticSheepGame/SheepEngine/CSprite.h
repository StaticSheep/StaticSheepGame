
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
    virtual void Remove();

    void SetTexture(std::string texture);

    Vec2 Size;
    Vec4 Color;

    Handle transform;

    float time;

    std::string SpriteName;
    int SpriteID;

    void Draw();

  private:
    
  };
}