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
  SpriteLight::SpriteLight() :m_brightness(1, 1, 1, 1), 
    m_size(1, 1), m_textureSize(1, 1), m_isOn(true)
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

    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(SpriteLight::Render));
  }

  void SpriteLight::Remove()
  {
    space->hooks.Remove("Draw", self);
  }

  void SpriteLight::SetTexture(const char * Texture)
  {
    m_spriteName = Texture;
    m_texture = GRAPHICS->LoadTexture(Texture);
    m_textureSize = GRAPHICS->GetTextureDim(m_texture);
  }

  void SpriteLight::TweakSetTexture(const void * Texture)
  {
    SetTexture(((std::string *)Texture)->c_str());
  }

  DirectSheep::Handle& SpriteLight::GetTexture()
  {
    return m_texture;
  }

  void SpriteLight::Render()
  {
    if (!m_isOn)
      return;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, trans->GetTranslation().Z);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(trans->GetScale().X * m_size.X,
      trans->GetScale().Y * m_size.Y);

    GRAPHICS->SetColor(m_brightness);

    GRAPHICS->SetSize(trans->GetScale().X * m_size.X,
      trans->GetScale().Y * m_size.Y);

      GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));

    GRAPHICS->SetSpriteFlip(false, false);
    GRAPHICS->SetCamState(0);

    GRAPHICS->RC()->DrawLightBatched(m_texture);
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