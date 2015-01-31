/******************************************************************************
Filename: CPointLight.h
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include "types/vectors/Vec4.h"
#include "components/base/Component.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  class Transform;

  class PointLight : public GameComponent
  {
  public:
    PointLight();
    ~PointLight();

    virtual void Initialize();
    virtual void Remove();

    void Render();

    void TurnOn();
    void TurnOff();

    void Toggle();

    // Manually change attenuation: Falloff
    void SetAttenuation(float constant, float linear, float quadratic);
    void SetConstant(float constant);
    void SetLinear(float linear);
    void SetQuadratic(float quadratic);

    // Get attenuation values
    Vec3D GetAttenuation(void);
    float GetConstant(void);
    float GetLinear(void);
    float GetQuadratic(void);

    // Set and Get brightness
    void SetBrightness(Vec4 brightness);
    Vec4 GetBrightness(void);

    void IsHardFallOff(bool ishardfalloff);

    // RGB and brightness values
    LightColor m_brightness;

    LightVec3 m_attenuation;

    // Hard falloff will cut light off all light at zero percent value
    bool m_hardFalloff;

    bool m_isOn;

    Handle transform;
  };
}