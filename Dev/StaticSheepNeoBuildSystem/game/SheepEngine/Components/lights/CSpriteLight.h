/******************************************************************************
Filename: CSpriteLight.h
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include "types/vectors/Vec4.h"
#include "components/base/Component.h"
#include "components/sprites/CSprite.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  class Transform;

  class SpriteLight : public GameComponent
  {
  public:
    SpriteLight();
    ~SpriteLight();

    virtual void Initialize();
    virtual void Remove();

    void SetTexture(const char * Texture);
    DirectSheep::Handle& GetTexture();

    void Render();

    void TurnOn();
    void TurnOff();

    void Toggle();

    // Set and Get brightness
    void SetBrightness(Vec4 brightness);
    Vec4 GetBrightness(void);

    void TweakSetTexture(const void * Texture);

    DirectSheep::Handle m_texture;

    LightColor m_brightness;

    std::string m_spriteName;

    Vec2 m_size;
    Vec2 m_textureSize;

    bool m_isOn;

    Handle transform;
  };
}