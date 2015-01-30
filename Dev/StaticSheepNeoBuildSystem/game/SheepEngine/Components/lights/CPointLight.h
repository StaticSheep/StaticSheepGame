/******************************************************************************
Filename: CPointLight.h
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "types/vectors/Vec2.h"
#include "types/vectors/Vec4.h"
#include "components/base/Component.h"
#include "components/transform/CTransform.h"

namespace Framework
{
  class Transform;

  class PointLight : public GameComponent
  {
    PointLight();
    ~PointLight();

    virtual void Initialize();
    virtual void Remove();

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

    // Set falloff values
    void SetFalloff(float hundred, float fifty, float zero);
    void SetHundred(float hundred);
    void SetFifty(float fifty);
    void SetZero(float zero);

    // Get falloff values
    Vec3D GetFalloff(void);
    float GetHundred(void);
    float GetFifty(void);
    float GetZero(void);

    void IsHardFallOff(bool ishardfalloff);

    // RGB and brightness values
    Vec4 m_brightness;

    // Attenuation values
    float m_constant;
    float m_linear;
    float m_quadratic;

    // Falloff values
    float m_f100;
    float m_f50;
    float m_f0;

    // Hard falloff will cut light off all light at zero percent value
    bool m_hardFalloff;

    bool m_isOn;
  };
}