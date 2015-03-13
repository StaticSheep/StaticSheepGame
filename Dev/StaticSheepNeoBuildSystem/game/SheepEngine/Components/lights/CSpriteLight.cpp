/******************************************************************************
Filename: CSpriteLight.cpp
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CSpriteLight.h"
#include "systems/graphics/SheepGraphics.h"
#include "Context/Context.h"

namespace Framework
{
  SpriteLight::SpriteLight()
    : m_brightness(1, 1, 1, 1), m_isOn(true)
  {
    transform = NULL;
  }

  SpriteLight::~SpriteLight() {}

  void SpriteLight::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    if (m_spriteName.length() == 0)
      m_spriteName = "Default.png";

    if (m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture(m_spriteName.c_str());
    }
    else
      SetTexture(m_spriteName.c_str());

    if (m_hooked)
    {
      space->hooks.Remove("PreDraw", self);
      space->hooks.Remove("Draw", self);
      space->hooks.Remove("PostDraw", self);
    }

    switch (m_layer)
    {
    case 2:
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(SpriteLight::Render));
      break;
    case 0:
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(SpriteLight::Render));
      break;
    case 1:
    default:
      space->hooks.Add("Draw", self, BUILD_FUNCTION(SpriteLight::Render));
      break;
    }

    m_hooked = true;
  }

  void SpriteLight::Remove()
  {
    switch (m_layer)
    {
    case 2:
      space->hooks.Remove("PostDraw", self);
      break;
    case 0:
      space->hooks.Remove("PreDraw", self);
      break;
    case 1:
    default:
      space->hooks.Remove("Draw", self);
      break;
    }
  }

  void SpriteLight::Render()
  {
    if (!m_isOn)
      return;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, trans->GetTranslation().Z);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(trans->GetScale().X * Size.X,
      trans->GetScale().Y * Size.Y);

    GRAPHICS->SetColor(m_brightness);

    if (m_uvScale)
    {
      GRAPHICS->SetSize(Size.X,
        Size.Y);

      GRAPHICS->SetObjectOrigin(m_origin.x * trans->GetScale().X,
        m_origin.y * trans->GetScale().Y);

      GRAPHICS->SetUV(MinUV, Vec2(MaxUV.x * trans->GetScale().X,
        MaxUV.y * trans->GetScale().y));
    }
    else
    {
      GRAPHICS->SetSize(trans->GetScale().X * Size.X,
        trans->GetScale().Y * Size.Y);

      GRAPHICS->SetObjectOrigin(m_origin.x, m_origin.y);

      GRAPHICS->SetUV(MinUV, MaxUV);
    }

    GRAPHICS->SetSpriteFlip(m_flipX, m_flipY);
    GRAPHICS->SetCamState(0);

    GRAPHICS->RC()->DrawLightBatched(this->m_texture, m_emissive);

    GRAPHICS->SetObjectOrigin(0, 0);
  }

  void SpriteLight::TurnOn()
  {
    m_isOn = true;
  }

  void SpriteLight::TurnOff()
  {
    m_isOn = false;
  }

  void SpriteLight::Toggle()
  {
    m_isOn = !m_isOn;
  }

  // Set and Get brightness
  void SpriteLight::SetBrightness(Vec4 brightness)
  {
    m_brightness = brightness;
  }

  Vec4 SpriteLight::GetBrightness(void)
  {
    return m_brightness;
  }
}