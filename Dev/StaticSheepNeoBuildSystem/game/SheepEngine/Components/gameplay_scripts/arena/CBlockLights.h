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

    enum Effect
    {
      NONE,
      FLICKER,
      PULSE
    };

    struct LightSettings
    {
      Vec4 color;
      unsigned fx = NONE;
      bool useColor = false;
      bool isOn = true;
    };

    struct EventData
    {
      float duration = 0;
      bool overrideDefault = false;
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

    LightSettings m_curSettings;

    LightSettings m_defaultSettings;

    std::stack<EventData> m_eventStack;
    std::stack<float> m_timeDelay;


  private:
    void NextEvent(SpriteLight* sl);
  };
}