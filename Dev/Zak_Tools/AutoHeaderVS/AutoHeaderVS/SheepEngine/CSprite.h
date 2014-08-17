
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

    Vec2 Size;

    Handle transform;

    float time;

    std::string SpriteName;
    int SpriteID;

    void Draw();

  private:
    void SetTexture(std::string& texture);
  };
}