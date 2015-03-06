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

  }

  void BlockLights::Initialize()
  {
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(BlockLights::Update));

    space->hooks.Add("TriggerLights", self,
      BUILD_FUNCTION(BlockLights::TriggerLight));
  }

  void BlockLights::Remove()
  {

  }

  void BlockLights::Update(float dt)
  {
    if (m_activated)
    {
      m_timeLeft -= dt;
      if (m_timeLeft < 0)
        Deactivate();
    }
  }

  void BlockLights::Activate(TriggerData* data)
  {
    m_activated = true;
    m_timeLeft = data->duration;
    m_effect = data->fx;

    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    m_prevStatus = sl->m_isOn;

    sl->m_isOn = true;

    if (data->overrideColor)
    {
      m_prevColor = sl->Color;
      sl->Color = data->color;
    }

    m_overrideColor = data->overrideColor;
      
  }

  void BlockLights::Deactivate()
  {
    m_activated = false;
    m_timeLeft = 0;

    SpriteLight* sl = (SpriteLight*)space->GetGameObject(owner)
      ->GetComponent(eSpriteLight);

    if (!sl)
      return;

    sl->m_isOn = m_prevStatus;
    if (m_overrideColor)
      sl->Color = m_prevColor;
  }

  void BlockLights::TriggerLight(int group, TriggerData* data)
  {
    if (group & m_group)
      Activate(data);
  }


}