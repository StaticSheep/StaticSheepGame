/******************************************************************************
Filename: CPointLight.cpp
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CPointLight.h"
#include "systems/graphics/SheepGraphics.h"

namespace Framework
{
  PointLight::PointLight()
  {

  }

  PointLight::~PointLight()
  {

  }

  void PointLight::Initialize()
  {

  }

  void PointLight::Remove()
  {

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

  }

  void PointLight::SetConstant(float constant)
  {

  }

  void PointLight::SetLinear(float linear)
  {

  }

  void PointLight::SetQuadratic(float quadratic)
  {

  }

  // Get attenuation values
  Vec3D PointLight::GetAttenuation(void)
  {
    return Vec3D(m_constant, m_linear, m_quadratic);
  }

  float PointLight::GetConstant(void)
  {
    return m_constant;
  }

  float PointLight::GetLinear(void)
  {
    return m_linear;
  }
  
  float PointLight::GetQuadratic(void)
  {
    return m_quadratic;
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

  // Set falloff values
  void PointLight::SetFalloff(float hundred, float fifty, float zero)
  {

  }

  void PointLight::SetHundred(float hundred)
  {

  }

  void PointLight::SetFifty(float fifty)
  {

  }

  void PointLight::SetZero(float zero)
  {

  }

  // Get falloff values
  Vec3D PointLight::GetFalloff(void)
  {
    return Vec3D(m_f100, m_f50, m_f0);
  }

  float PointLight::GetHundred(void)
  {
    return m_f100;
  }

  float PointLight::GetFifty(void)
  {
    return m_f50;
  }

  float PointLight::GetZero(void)
  {
    return m_f0;
  }
}