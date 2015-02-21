/******************************************************************************
Filename: CPointLight.cpp
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CPointLight.h"
#include "systems/graphics/SheepGraphics.h"
#include "Context/Context.h"

namespace Framework
{
  PointLight::PointLight() : m_brightness(1, 1, 1, 1),
    m_attenuation(0.01f, 0.01f, 0.01f),
                             m_isOn(true), 
                             m_hardFalloff(false) {}

  PointLight::~PointLight() {}

  void PointLight::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    space->hooks.Add("Draw", self, BUILD_FUNCTION(PointLight::Render));
  }

  void PointLight::Remove()
  {
    space->hooks.Remove("Draw", self);
  }

  void PointLight::Render()
  {
    if (m_isOn)
    {
      Vec4 scaledBrightness(m_brightness.r,
        m_brightness.g,
        m_brightness.b,
        m_brightness.a);

      GRAPHICS->RC()->BatchPointLight(
        space->GetHandles().GetAs<Transform>(transform)->GetTranslation(),
        scaledBrightness, m_attenuation);
    }
  }

  void PointLight::TurnOn()
  {
    m_isOn = true;
  }

  void PointLight::TurnOff()
  {
    m_isOn = false;
  }

  void PointLight::Toggle()
  {
    m_isOn = !m_isOn;
  }

  void PointLight::IsHardFallOff(bool ishardfalloff)
  {
    m_hardFalloff = ishardfalloff;
  }

  // Manually change attenuation: Falloff
  void PointLight::SetAttenuation(float constant, float linear, float quadratic)
  {
    m_attenuation = Vec3(constant, linear, quadratic);
  }

  void PointLight::SetConstant(float constant)
  {
    m_attenuation.x = constant;
  }

  void PointLight::SetLinear(float linear)
  {
    m_attenuation.y = linear;
  }

  void PointLight::SetQuadratic(float quadratic)
  {
    m_attenuation.z = quadratic;
  }

  // Get attenuation values
  Vec3D PointLight::GetAttenuation(void)
  {
    return m_attenuation;
  }

  float PointLight::GetConstant(void)
  {
    return m_attenuation.x;
  }

  float PointLight::GetLinear(void)
  {
    return m_attenuation.y;
  }
  
  float PointLight::GetQuadratic(void)
  {
    return m_attenuation.z;
  }

  // Set and Get brightness
  void PointLight::SetBrightness(Vec4 brightness)
  {
    m_brightness = brightness;
  }

  Vec4 PointLight::GetBrightness(void)
  {
    return m_brightness;
  }
}