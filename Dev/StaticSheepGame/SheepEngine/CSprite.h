
#pragma once

#include "CTransform.h"
#include "graphics\Handle.h"

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

    DirectSheep::Handle& SetTexture(const std::string& texture);
    DirectSheep::Handle& GetTexture();

    Vec2 Size;

    Vec4 Color;

    Handle transform;

    std::string SpriteName;

    void Draw();

  private:

    DirectSheep::Handle m_texture;
    
  };
}