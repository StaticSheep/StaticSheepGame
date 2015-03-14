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
#include "SheepMath.h"

namespace Framework
{
  BlockLights::BlockLights()
  {
    m_defaultSettings.color = LightColor(1, 1, 1, 1);
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

    m_curSettings = m_defaultSettings;
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

    switch (m_curSettings.fx)
    {
    case PULSE:
      UpdatePulse(dt);
      break;

    case FLICKER:
      UpdateFlicker(dt);
      break;

    case RANDOMFLICKER:
      UpdateRandomFlicker(dt);
      break;

    case CUSTOM:
      UpdateCustom(dt);
      break;

    default:
      break;
    }
  }

  void BlockLights::UpdatePulse(float dt)
  {
    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    PulseData& m_pulseData = m_curSettings.pulseData;

    m_pulseData.timeleft -= dt;

    if (m_pulseData.timeleft <= 0)
      m_pulseData.timeleft = 0;


    float intensity;
    
    switch (m_curSettings.ease)
    {
    case LINEAR:
      intensity = Ease::Linear(m_pulseData.timeleft /
        m_pulseData.duration,
        m_pulseData.prevTarget, m_pulseData.curTarget);
      break;

    case QUADRATICIN:
      intensity = Ease::QuadraticIn(m_pulseData.timeleft /
        m_pulseData.duration,
        m_pulseData.prevTarget, m_pulseData.curTarget);
      break;

    case QUADRATICOUT:
      intensity = Ease::QuadraticOut(m_pulseData.timeleft /
        m_pulseData.duration,
        m_pulseData.prevTarget, m_pulseData.curTarget);
      break;

    default:
      break;
    }
    

    sl->m_brightness.A = intensity;

    if (m_pulseData.timeleft == 0)
    {
      m_pulseData.timeleft = m_pulseData.duration;

      float temp = m_pulseData.prevTarget;
      m_pulseData.prevTarget = m_pulseData.curTarget;
      m_pulseData.curTarget = temp;
    }

  }

  void BlockLights::UpdateRandomFlicker(float dt)
  {
    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    FlickerData& m_flickerData = m_curSettings.flickerData;

    m_flickerData.timeleft -= dt;
    if (m_flickerData.timeleft <= 0)
    {
      m_flickerData.timeleft = GetRandom(0.05f, 0.15f);
      m_flickerData.high = !m_flickerData.high;

      float intensity = 0;

      if (m_flickerData.high)
      {
        intensity = m_flickerData.highIntensity +
          GetRandom(-m_flickerData.highRand, m_flickerData.highRand);
      }
      else
      {
        intensity = m_flickerData.lowIntensity +
          GetRandom(-m_flickerData.lowRand, m_flickerData.lowRand);
      }
      
      sl->m_brightness.a = intensity;
    }

  }

  void BlockLights::UpdateFlicker(float dt)
  {
    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    FlickerData& m_flickerData = m_curSettings.flickerData;

    m_flickerData.timeleft -= dt;
    if (m_flickerData.timeleft <= 0)
    {
      m_flickerData.timeleft = m_flickerData.duration;
      m_flickerData.high = !m_flickerData.high;

      float intensity = 0;

      if (m_flickerData.high)
        intensity = m_flickerData.highIntensity;
      else
        intensity = m_flickerData.lowIntensity;

      sl->m_brightness.a = intensity;
    }

  }

  void BlockLights::UpdateCustom(float dt)
  {
    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    CustomData& cdata = m_curSettings.customData;

    cdata.timeleft -= dt;

    if (cdata.timeleft <= 0)
    {
      cdata.timeleft = cdata.duration / cdata.length;
      
      ++cdata.pos;
      if (cdata.pos >= cdata.length)
        cdata.pos = 0;

      float intensity = (m_curSettings.customSequence[cdata.pos] - 'a') / 25.0f;
      sl->m_brightness.a = intensity;
    }
  }


  void BlockLights::UpdateSettings(SpriteLight* sl)
  {
    if (m_curSettings.useColor)
      sl->m_brightness = m_curSettings.color;

    sl->m_isOn = m_curSettings.isOn;

    switch (m_curSettings.fx)
    {
    case PULSE:
      StartPulse();
      break;
    case FLICKER:
      StartFlicker();
      break;
    case RANDOMFLICKER:
      StartRandomFlicker();
      break;
    case CUSTOM:
      StartCustom();
      break;
    default:
      break;
    }

    m_prevEffect = (Effect)m_curSettings.fx;
  }

  void BlockLights::StartPulse()
  {
    //m_pulseData.duration = 2;
    //m_pulseData.highIntensity = 1.0f;
    //m_pulseData.lowIntensity = 0.6f;

    m_curSettings.pulseData.timeleft = m_curSettings.pulseData.duration;

    m_curSettings.pulseData.curTarget = m_curSettings.pulseData.lowIntensity;
    m_curSettings.pulseData.prevTarget = m_curSettings.pulseData.highIntensity;
  }

  void BlockLights::StartRandomFlicker()
  {

    //m_flickerData.lowIntensity = 0.2f;
    //m_flickerData.lowRand = 0.1f;

    //m_flickerData.highIntensity = 0.9f;
    //m_flickerData.highRand = 0.1f;

    //m_flickerData.high = false;
    //m_flickerData.timeleft = 0;

    m_curSettings.flickerData.timeleft = 0;
    m_curSettings.flickerData.high = false;
  }

  void BlockLights::StartFlicker()
  {
    m_curSettings.flickerData.timeleft = 0;
    m_curSettings.flickerData.high = false;
  }

  void BlockLights::StartCustom()
  {
    m_curSettings.customData.length = m_curSettings.customSequence.length();
    m_curSettings.customData.timeleft = 0;
    m_curSettings.customData.pos = m_curSettings.customData.length;
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
          ed = m_eventStack.top();
          m_eventStack.pop();

          /* Remove the time delay from the event we skipped and
          then fill it with the (greater) time delay. */
          m_timeDelay.push(lostTime);
          m_timeDelay.pop();
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

    UpdateSettings(sl);
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

      if (!m_active)
      {
        m_curSettings = data->settings;
        UpdateSettings(sl);
      }
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

    UpdateSettings(sl);
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