/******************************************************************************
Filename: slots_controller.cpp
Project:
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "components/slotmachine/slots_controller.h"
#include "../sprites/CSprite.h"
#include "SheepMath.h"
#include "systems/input/Input.h"

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
    m_realSnap = m_uvSnap;
    m_uvSnap = (int)(100 * m_uvSnap) / 100.0f;

    m_curSpeed = startSpeed;
    //m_timeLeft = 1 + GetRandom(1, 3);
    m_timed = false;
    m_spinning = true;

    spr->MaxUV = Vec2(1, m_uvSnap);
    spr->MinUV.y = spr->MaxUV.y - m_uvSnap;
  }

  void SlotController::Stop()
  { 
    if (!m_order)
      m_stopping = true;
    else
      --m_order;
  }

  void SlotController::Update(float dt)
  {
    if (SHEEPINPUT->Keyboard.KeyIsPressed(VK_SPACE) && m_spinning && !m_stopping)
      Stop();

    Sprite* spr = (Sprite*)space->GetComponent(eSprite, owner);
    if (!spr)
      return;

    if (m_spinning)
    {
      float vMove = m_curSpeed * dt;

      spr->MaxUV.y = spr->MaxUV.y - vMove;
      if (spr->MaxUV.y < -200.0f)
        spr->MaxUV.y += 300.0f;

      spr->MinUV.y = spr->MaxUV.y - m_uvSnap;

      if (m_timed)
      {
        if (m_timeLeft <= 0)
          m_stopping = true;

        m_timeLeft -= dt;
      }

      if (m_stopping)
      {
        if (m_land < 0)
          m_land = GetRandom(0, slotOptions - 1);

        float sl = m_land * m_realSnap;
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