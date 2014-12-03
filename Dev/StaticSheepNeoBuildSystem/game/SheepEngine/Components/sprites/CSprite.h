/******************************************************************************
Filename: CSprite.h
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include "types/vectors/Vec2.h"
#include "components/base/Component.h"
#include "components/transform/CTransform.h"
#include "Handle.h"

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

    void SetFlipX(bool isFlipped);
    void SetFlipY(bool isFlipped);

    bool GetFlipX(void);
    bool GetFlipY(void);

    Vec2 Size;
    Vec2 TextureSize;

    Vec4 Color;

    Handle transform;

    bool m_flipX = false;
    bool m_flipY = false;

    void TweakSetTexture(const void * Texture);

    // DO NOT WRITE DIRECTLY TO THIS SILLY
    std::string m_spriteName;

    void Draw();

    DirectSheep::Handle m_texture;
    
  };
}