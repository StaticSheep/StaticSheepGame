/*****************************************************************
Filename: blocklights.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "components/base/Component.h"
#include "components/lights/CSpriteLight.h"

namespace Framework
{
  class BlockLights : public GameComponent
  {
  public:
    BlockLights();

    /* Effect Types */
    enum Effect
    {
      NONE,
      FLICKER,
      RANDOMFLICKER,
      PULSE,
      CUSTOM
    };

    /* Eases for PULSE effect */
    enum EffectEase
    {
      LINEAR,
      QUADRATICIN,
      QUADRATICOUT
    };

    /* Pulse Effect Data */
    struct PulseData
    {
      float lowIntensity;
      float highIntensity;
      float duration;

      // PRIVATE:
      float timeleft;
      float prevTarget;
      float curTarget;
    };

    /* Flicker & RandomFlicker Data */
    struct FlickerData
    {
      float lowIntensity;
      float lowRand; // (Random only)

      float highIntensity;
      float highRand; // (Random only)

      float duration;
      float durationRand; // (Random only)

      // PRIVATE:
      float timeleft;
      bool high;
    };

    /* Custom Effect Data */
    struct CustomData
    {
      float duration;

      // PRIVATE:
      float timeleft;
      int length;
      int pos;
    };

    /* Light Settings */
    struct LightSettings
    {
      LightSettings()
      {
        /* Generic settings */
        pulseData.highIntensity = 1.0f;
        pulseData.lowIntensity = 0.6f;
        pulseData.duration = 5;
      }

      /* Color of light (Alpha is intensity) */
      LightColor color;

      unsigned fx = PULSE;
      unsigned ease = LINEAR;

      /* Incase you don't want to change the color */
      bool useColor = true;

      /* Incase you want to turn the light off */
      bool isOn = true;

      /* Custom sequence, see 
      https://developer.valvesoftware.com/wiki/Light#Appearances
      for examples of this. */
      std::string customSequence;

      /* Based on the effect you are using fill these in.
      Only one can be used at a time. */
      union
      {
        /* For pulse effect data */
        PulseData pulseData;
        /* For flicker and random flicker data */
        FlickerData flickerData;
        /* For custom effect data */
        CustomData customData;
      };
    };

    /* Event Settings to be passed to the block light when
    the light has a triggered event on it. */
    struct EventData
    {
      /* How long the event should last */
      float duration = 0;

      /* Should this override the default settings this block's light
      will return to when all events finished. If this is true then
      duration is ignored. */
      bool overrideDefault = false;

      /* The settings for the light. */
      LightSettings settings;
    };

    void Initialize();
    void Remove();

    void Update(float dt);
    void LightingEvent(unsigned group, EventData* data);

    void Activate(EventData* data);
    void Deactivate();

    

    // This is a bit-masked integer
    int m_group = 0;

    // Time left
    float m_timeLeft = 0;
    // If an event is happening
    bool m_active = false;

    /* The current settings the light has */
    LightSettings m_curSettings;

    /* The default settings the light has */
    LightSettings m_defaultSettings;

    std::stack<EventData> m_eventStack;
    std::stack<float> m_timeDelay;

    

  private:
    void NextEvent(SpriteLight* sl);
    void UpdateSettings(SpriteLight* sl);

    void StartPulse();
    void StartFlicker();
    void StartRandomFlicker();
    void StartCustom();

    void UpdatePulse(float dt);
    void UpdateFlicker(float dt);
    void UpdateRandomFlicker(float dt);
    void UpdateCustom(float dt);

    Effect m_prevEffect = NONE;
  };
}