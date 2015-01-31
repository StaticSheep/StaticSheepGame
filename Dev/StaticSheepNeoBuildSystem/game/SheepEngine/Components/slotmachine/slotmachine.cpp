/******************************************************************************
Filename: slots_controller.cpp
Project:
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "components/slotmachine/slotmachine.h"
#include "components/sprites/CSprite.h"
#include "SheepMath.h"
#include "systems/input/Input.h"
#include "systems/graphics/SheepGraphics.h"

namespace Framework
{
  SlotMachine::SlotMachine()
  {

  }

  SlotMachine::~SlotMachine()
  {

  }

  void SlotMachine::SetupSlots()
  {
    int spinId = Draw::GetTextureID(m_spinTexture.c_str());
    int stopID = Draw::GetTextureID(m_stopTexture.c_str());

    Vec2 spinDim = GRAPHICS->GetTextureDim(
      DirectSheep::Handle(DirectSheep::TEXTURE, spinId));

    m_slotDimensions = spinDim;
    m_slotDimensions.x *= slotSize.x;
    m_slotDimensions.y *= slotSize.y;

    m_slots.clear();

    for (int i = 0; i < numSlots; ++i)
    {
      m_slots.emplace_back();
      Slot* slot = &m_slots.back();
      slot->spinTextureID = spinId;
      slot->stopTextureID = stopID;
      slot->curSpeed = m_startSpeed;
      slot->height = (spinDim.y / slotOptions) / spinDim.y;
      slot->realHeight = slot->height;
      slot->height = (int)(100 * slot->height) / 100.0f;
      slot->timed = m_timed;
      slot->spinning = true;
      slot->spinPos = GetRandom(0, 1000) / 300.0f;
    }
  }

  void SlotMachine::Initialize()
  {
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(SlotMachine::Update));

    space->hooks.Add("Draw", self,
      BUILD_FUNCTION(SlotMachine::Draw));

    SetupSlots();

    if (startSpinning)
      Start();

  }

  void SlotMachine::Start()
  {
    m_spinning = true;

    for (int i = 0; i < m_slots.size(); ++i)
    {
      Slot* slot = &m_slots[i];
      slot->spinning = true;
      slot->stopping = false;
      slot->land = -1;
      slot->curSpeed = startSpeed;

      if (m_timed)
      {
        slot->timed = true;
        slot->timeLeft = startTime + (bonusTime * i);
      }
    }
  }

  void SlotMachine::Stop()
  { 

  }

  void SlotMachine::Update(float dt)
  {
    //if (SHEEPINPUT->Keyboard.KeyIsPressed(VK_SPACE) && m_spinning && !m_stopping)
    //  Stop();
    if (m_spinning)
    {
      float vMove;

      for (int i = 0; i < m_slots.size(); ++i)
      {
        Slot* slot = &m_slots[i];
        if (!slot->spinning)
          continue;

        vMove = slot->curSpeed * dt;
        
        slot->spinPos = slot->spinPos - vMove;

        if (slot->spinPos < -1000.0f)
          slot->spinPos += 2000.0f;

        if (slot->timed)
        {
          if (slot->timeLeft <= 0)
            slot->stopping = true;

          slot->timeLeft -= dt;
        }

        if (slot->stopping)
        {
          if (slot->land < 0)
            slot->land = GetRandom(0, (slotOptions - 1));

          float stopV = slot->land * slot->realHeight + slot->height;
          if (slot->spinPos - stopV < slot->curSpeed * dt * 5)
          {
            slot->spinPos = stopV - slot->height;
            slot->spinning = false;
          }
        }

      }
    }
    
  }

  void SlotMachine::Draw()
  {
    Transform* tr = (Transform*)space->GetComponent(eTransform, owner);

    float xPos = tr->GetTranslation().x - 
      (numSlots / 2.0f) * (m_slotDimensions.x + slotMargin) +
      ((m_slotDimensions.x + slotMargin) / 2) + slotOffset.x;
    float yPos = tr->GetTranslation().y + slotOffset.y;

    Draw::ForceZ(true, tr->GetTranslation().z - 0.01f);
    for (int i = 0; i < m_slots.size(); ++i)
    {
      Slot* slot = &m_slots[i];

      Draw::SetColor(1, 1, 1, 1);
      Draw::SetUVs(Vec2(0, slot->spinPos), Vec2(1, slot->spinPos + slot->height));
      
      if (slot->spinning)
        Draw::SetTexture(slot->spinTextureID);
      else
        Draw::SetTexture(slot->stopTextureID);

      Draw::DrawTexturedRect(xPos, yPos, slotSize.x, slotSize.y);

      xPos += m_slotDimensions.x + slotMargin;
    }

    Draw::ForceZ(false, 0);
  }

  void SlotMachine::TweakSetNumSlots(const void* value)
  {
    numSlots = *(int*)value;
    SetupSlots();
  }

  void SlotMachine::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }
}