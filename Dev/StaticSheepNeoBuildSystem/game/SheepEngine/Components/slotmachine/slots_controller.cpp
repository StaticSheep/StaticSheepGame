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
  
    spr->SetTexture("slot_test_blur.png");

    m_uvSnap = (spr->TextureSize.y / slotOptions) / spr->TextureSize.y;

    m_curSpeed = startSpeed;
    m_timeLeft = 3;
    m_spinning = true;

    spr->MaxUV = Vec2(1, m_uvSnap);
    spr->MinUV.y = spr->MaxUV.y - m_uvSnap;
  }

  void SlotController::Update(float dt)
  {
    Sprite* spr = (Sprite*)space->GetComponent(eSprite, owner);

    if (m_spinning)
    {
      m_timeLeft -= dt;

      float vMove = m_curSpeed * dt;

      spr->MaxUV.y = spr->MaxUV.y - vMove;
      if (spr->MaxUV.y < -200.0f)
        spr->MaxUV.y += 300.0f;

      spr->MinUV.y = spr->MaxUV.y - m_uvSnap;

      if (m_timeLeft <= 0)
      {
        float sl = (int)(spr->MaxUV.y / m_uvSnap) * m_uvSnap;
        float sn = sl + m_uvSnap;

        if (spr->MaxUV.y - sn < m_curSpeed * dt * 2)
        {
          spr->MinUV.y = sl;
          spr->MaxUV.y = sn;
          spr->SetTexture("slot_test.png");
          m_spinning = false;
        }
      }
    }

    
  }

  void SlotController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}