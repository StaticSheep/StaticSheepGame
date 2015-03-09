/*****************************************************************
Filename: blocklights.cpp
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CBlockLights.h"
#include "types\space\Space.h"
#include "components\lights\CSpriteLight.h"

namespace Framework
{
  BlockLights::BlockLights()
  {
    m_defaultSettings.color = Vec4(1, 1, 1, 1);
    m_defaultSettings.useColor = true;
  }

  void BlockLights::Initialize()
  {
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(BlockLights::Update));

    space->hooks.Add("LightingEvent", self,
      BUILD_FUNCTION(BlockLights::LightingEvent));

    space->GetGameObject(owner)->hooks.Add(
      "LightingEvent", self, BUILD_FUNCTION(BlockLights::Activate));
  }

  void BlockLights::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("LightingEvent", self);

    space->GetGameObject(owner)->hooks.Remove("LightingEvent", self);
  }

  void BlockLights::Update(float dt)
  {
    /* Deactivates after a certain time */
    if (m_active)
    {
      m_timeLeft -= dt;
      if (m_timeLeft <= 0)
        Deactivate();
    }
  }

  void BlockLights::NextEvent(SpriteLight* sl)
  {
    if (m_eventStack.size() == 0)
    {
      m_active = false;
      m_curSettings = m_defaultSettings;
    }
    else
    {
      // Our next event data
      EventData& ed = m_eventStack.top();

      float lostTime = 0;

      while (true)
      {
        lostTime += m_timeDelay.top();
        m_timeDelay.pop();

        /* Check to see if the event's duration was too short and
        was passed over during our previous event. */
        if (ed.duration <= lostTime)
        {
          m_eventStack.pop();
          ed = m_eventStack.top();

          /* Remove the time delay from the event we skipped and
          then fill it with the (greater) time delay. */
          m_timeDelay.pop();
          m_timeDelay.push(lostTime);
          lostTime = 0;
        }
        else
        {
          break;
        }
      }

      m_timeLeft = ed.duration - lostTime;
      m_curSettings = ed.settings;
      m_eventStack.pop();

    }

    if (m_curSettings.useColor)
      sl->m_brightness = m_curSettings.color;

    sl->m_isOn = m_curSettings.isOn;
  }

  void BlockLights::Activate(EventData* data)
  {
    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    if (data->overrideDefault)
    {
      m_defaultSettings = data->settings;
      return;
    }

    EventData old; // Our old event data

    if (m_active)
    {
      old.duration = m_timeLeft;
      old.settings = m_curSettings;
      m_timeDelay.push(data->duration);
      m_eventStack.push(old);
    }

    m_active = true;
    m_timeLeft = data->duration;

    m_curSettings = data->settings;

    if (m_curSettings.useColor)
      sl->m_brightness = m_curSettings.color;

    sl->m_isOn = m_curSettings.isOn;
  }

  void BlockLights::Deactivate()
  {
    m_timeLeft = 0;

    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;
    NextEvent(sl);
  }

  void BlockLights::LightingEvent(unsigned group, EventData* data)
  {
    if (group & m_group)
      Activate(data);
  }


}