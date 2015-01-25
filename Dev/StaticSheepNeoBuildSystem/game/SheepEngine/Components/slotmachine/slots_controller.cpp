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
    m_timeLeft = 10;

    m_stage = Fast;
    m_timeData[0] = m_timeLeft - (m_timeLeft / 3);
    m_timeData[1] = m_timeData[0] - (m_timeLeft / 3);
  }

  void SlotController::Update(float dt)
  {
    Sprite* spr = (Sprite*)space->GetComponent(eSprite, owner);

    if (m_timeLeft > 0)
    {
      m_curTime += dt;
      m_timeLeft -= dt;

      float vMove = m_curSpeed * dt;

      spr->MinUV = spr->MinUV + Vec2(0, vMove);
      if (spr->MinUV.y > 1.0f)
      {
        spr->MinUV.y -= 1;
        spr->MaxUV.y -= 1;
      }
      spr->MaxUV = spr->MaxUV + Vec2(0, vMove);

      m_curSpeed = m_timeLeft*m_timeLeft - m_curTime*m_curTime;
    }

/*
    if (m_stage != Stop)
    {

      if (m_stage == Fast && m_timeLeft < m_timeData[0])
      {
        if (m_transition)
        {
          if (m_curSpeed < startSpeed / 3)
          {
            m_transition = false;
            m_stage = Slow;
          }
          else
            m_curSpeed -= ((startSpeed / 3) / fastToSlow) * dt;
        }
        else
          m_transition = true;
      }

      if (m_stage == Slow && m_timeLeft < m_timeData[1])
      {
        if (m_transition)
        {
          if (m_curSpeed < startSpeed / 6)
          {
            m_transition = false;
            m_stage = Crawl;
          }
          else
            m_curSpeed -= ((startSpeed / 2) / fastToSlow) * dt;
        }
        else
          m_transition = true;
      }

      if (m_stage == Slow && m_timeLeft <= slowToCrawl)
      {
        if (m_transition)
        {
          m_curSpeed -= (m_startCrawlSpeed / slowToCrawl) * dt;
          if (m_curSpeed < 0)
          {
            m_curSpeed = 0;
            m_transition = false;
            m_stage = Stop;
          }
        }
        else
        {
          m_transition = true;
          m_startCrawlSpeed = m_curSpeed;
        }
      }
      m_timeLeft -= dt;
    }*/
    

    
  }

  void SlotController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}