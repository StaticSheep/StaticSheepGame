/******************************************************************************
Filename: slots_controller.cpp
Project:
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "components/slotmachine/slots_controller.h"
#include "../sprites/CSprite.h"

namespace Framework
{
  SlotController::SlotController()
  {

  }

  SlotController::~SlotController()
  {

  }

  void SlotController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(SlotController::Update));

    Sprite* spr = (Sprite*)space->GetComponent(eSprite, owner);

    if (spr == nullptr)
      return;
    m_uvSnap = slotHeight / spr->TextureSize.y;

    spr->MaxUV = Vec2(1, m_uvSnap);

    m_curSpeed = startSpeed;
    m_timeLeft = 5;
    m_spinning = true;
  }

  void SlotController::Update(float dt)
  {
    Sprite* spr = (Sprite*)space->GetComponent(eSprite, owner);

    if (m_spinning)
    {
      m_timeLeft -= dt;

      float vMove = m_curSpeed * dt;

      spr->MinUV = spr->MinUV + Vec2(0, vMove);
      if (spr->MinUV.y > 1.0f)
      {
        spr->MinUV.y -= 1;
        spr->MaxUV.y -= 1;
      }
      spr->MaxUV = spr->MaxUV + Vec2(0, vMove);

      if (m_timeLeft <= 0)
      {
        float r = spr->MinUV.y - (int)(spr->MinUV.y / m_uvSnap) * m_uvSnap;
        if (r <= m_curSpeed)
        {
          spr->MinUV.y -= r;
          spr->MaxUV.y -= r;
        }
      }
    }

    
  }

  void SlotController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}