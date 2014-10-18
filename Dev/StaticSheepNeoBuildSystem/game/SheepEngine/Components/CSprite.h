
#pragma once

#include "Vec2.h"
#include "Component.h"
#include "CTransform.h"
#include "SheepGraphics\Handle.h"

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

    void SetTexture(const char * Texture);
    DirectSheep::Handle& GetTexture();

    Vec2 Size;
    Vec2 TextureSize;

    Vec4 Color;

    Handle transform;

    void TweakSetTexture(const void * Texture);

    // DO NOT WRITE DIRECTLY TO THIS SILLY
    std::string m_spriteName;

    void Draw();

    DirectSheep::Handle m_texture;
    
  };
}