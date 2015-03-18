/*****************************************************************
Filename: CPulser.cpp
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CPulser.h"
#include "../../sprites/CSprite.h"

namespace Framework
{
  Pulser::Pulser()
  {

  }

  Pulser::~Pulser()
  {

  }

  void Pulser::Initialize()
  {

    m_amount.x /= m_time;
    m_amount.y /= m_time;

    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(Pulser::LogicUpdate));
  }

  void Pulser::LogicUpdate(float dt)
  {
    Transform* tr = space->GetGameObject(owner)->
      GetComponent<Transform>(eTransform);

    m_cTime -= dt;

    if (m_cTime < 0.0f)
    {
      m_larger = !m_larger;
      m_cTime = m_time;
    }

    m_tempScale = tr->GetScale();

    if (m_larger)
    {
      m_tempScale.x += m_amount.x * dt;
      m_tempScale.y += m_amount.y * dt;
    }
    else
    {
      m_tempScale.x -= m_amount.x * dt;
      m_tempScale.y -= m_amount.y * dt;
    }

    tr->SetScale(m_tempScale);

  }

  void Pulser::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}