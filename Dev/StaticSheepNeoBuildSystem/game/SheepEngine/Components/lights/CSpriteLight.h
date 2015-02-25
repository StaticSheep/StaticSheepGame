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

  class SpriteLight : public Sprite
  {
  public:
    SpriteLight();
    ~SpriteLight();

    virtual void Initialize();
    virtual void Remove();

    void Render();

    void TurnOn();
    void TurnOff();

    void Toggle();

    // Set and Get brightnessd
    void SetBrightness(Vec4 brightness);
    Vec4 GetBrightness(void);

    void TweakSetTexture(const void * Texture);

    LightColor m_brightness;

    bool m_isOn;

    bool m_mimicSprite = false;
    void TweakMimicSprite(const void* mimic);
  };
}