
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

    Vec2 m_Size;
    Vec4 m_Color;

    Handle transform;

    float time;

    std::string SpriteName;
    int SpriteID;

    void Draw();

  private:
    
  };
}