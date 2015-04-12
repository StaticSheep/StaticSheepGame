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
#include "systems/anttweak/TweakHelper.h"
#include "../sound/CSoundEmitter.h"
#include "../sound/CSoundPlayer.h"

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
    Transform* tr = (Transform*)space->GetComponent(eTransform, owner);
    Vec2 trsc = tr->GetScale();

    int spinId = Draw::GetTextureID(m_spinTexture.c_str());
    int stopId = Draw::GetTextureID(m_stopTexture.c_str());
    m_slotbackTextureID = Draw::GetTextureID(m_slotBackTexture.c_str());

    Vec2 spinDim = GRAPHICS->GetTextureDim(
      DirectSheep::Handle(DirectSheep::TEXTURE, spinId));

    m_slotBackDimensions = GRAPHICS->GetTextureDim(
      DirectSheep::Handle(DirectSheep::TEXTURE, m_slotbackTextureID));

    m_slotDimensions = spinDim;

    m_slots.clear();

    void* context = customReelCBObj != Handle::null ? space->GetHandles().Get(customReelCBObj) : nullptr;
    customReelCB.ForceBind(context);

    for (int i = 0; i < numSlots; ++i)
    {
      m_slots.emplace_back();
      Slot* slot = &m_slots.back();
      slot->spinTextureID = spinId;
      slot->stopTextureID = stopId;

      if (customReelCB)
        customReelCB(i, &slot->spinTextureID, &slot->stopTextureID);

      slot->curSpeed = m_startSpeed;
      slot->height = (spinDim.y / slotOptions) / spinDim.y;
      slot->realHeight = slot->height;
      slot->height = (int)(100 * slot->height) / 100.0f;
      slot->timed = m_timed;
      slot->spinning = true;
      slot->spinPos = GetRandom(0, 1000) / 300.0f;
    }

    
  }

  void SlotMachine::TestFunction(int slotNum, int* spinID, int* stopID)
  {
    int a = 10;
    a++;
  }

  void SlotMachine::Initialize()
  {
    //GREG LOOK AT THIS
    //SetTextureCB(self, BUILD_FUNCTION(SlotMachine::TestFunction));

    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(SlotMachine::Update));

    if (m_hooked)
    {
      space->hooks.Remove("PreDraw", self);
      space->hooks.Remove("Draw", self);
      space->hooks.Remove("PostDraw", self);
    }

    switch (m_layer)
    {
    case 2:
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(SlotMachine::Draw));
      break;
    case 0:
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(SlotMachine::Draw));
      break;
    case 1:
    default:
      space->hooks.Add("Draw", self, BUILD_FUNCTION(SlotMachine::Draw));
      break;
    }

    m_hooked = true;

    if (ENGINE->m_editorActive)
      SetupSlots();
  }

  void SlotMachine::TweakSetupSlots(const void* value)
  {
    AntTweak::Tweaker::currentGeneric->genericMember->Type()->Copy(
      (char*)this + AntTweak::Tweaker::currentGeneric->realOffset, value);
    
    SetupSlots();
  }

  void SlotMachine::Start()
  {
    SetupSlots();

    m_spinning = true;
    m_numSpinning = m_slots.size();
    m_results.clear();

    for (unsigned int i = 0; i < m_slots.size(); ++i)
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

      m_results.emplace_back();
    }

    //start spin noise here
    SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
    SoundInstance instance;
    instance.volume = 1.0f;
    instance.mode = PLAY_LOOP;
    sp->Play("slot_digital_spin2", &instance);
    sp->Play("slot_digital_spin2", &instance);
  }

  void SlotMachine::SetTextureCB(Handle obj, Function cb)
  {
    customReelCB = cb;
    customReelCBObj = obj;
  }

  void SlotMachine::SetSelectionCB(Handle obj, Function cb)
  {
    selectionCB = cb;
    selectionCBObj = obj;
  }

  void SlotMachine::SetFinishedCB(Handle obj, Function cb)
  {
    finishCB = cb;
    finishCBObj = obj;
  }

  void SlotMachine::Stop()
  { 
    for (unsigned int i = 0; i < m_slots.size(); ++i)
    {
      if (m_slots[i].spinning)
      {
        m_slots[i].spinning = false;
        return;
      }
    }

  }

  void SlotMachine::SlotsFinished() //all slots are finished
  {
    if (finishCB)
    {
      void* context = finishCBObj != Handle::null ?
        space->GetHandles().Get(finishCBObj) : nullptr;

      finishCB.ForceBind(context);
      finishCB(&m_results);
    }
    //play sound
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    SoundInstance instance;
    instance.volume = 1.0f;
    instance.mode = PLAY_ONCE;
    se->Play("slot_digital_win", &instance);

    //stop spin noise here
    SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
    sp->Stop("slot_digital_spin2", INSTANT);
  }

  void SlotMachine::Update(float dt)
  {
    if (startSpinning && !m_spinning)
    {
      Start();
      startSpinning = false;
    }
      

    //if (SHEEPINPUT->Keyboard.KeyIsPressed(VK_SPACE) && m_spinning && !m_stopping)
    //  Stop();
    if (m_spinning)
    {
      float vMove;

      for (unsigned int i = 0; i < m_slots.size(); ++i)
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
          {
            if (selectionCB)
            {
              void* context = selectionCBObj != Handle::null ? space->GetHandles().Get(selectionCBObj)
                : nullptr;

              selectionCB.ForceBind(context);
              selectionCB(i, &(slot->land));
            }
              
            else
              slot->land = GetRandom(0, (slotOptions - 1));
          }
            

          float stopV = slot->land * slot->realHeight + slot->height;
          if (slot->spinPos - stopV < slot->curSpeed * dt * 5)
          {
            slot->spinPos = stopV - slot->height;
            m_results[i] = slot->land;
            slot->spinning = false;
            --m_numSpinning;
            //reel finished
            SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
            SoundInstance instance;
            instance.volume = 1.0f;
            instance.mode = PLAY_ONCE;

            se->Play("slot_digital_reelStop", &instance);
            se->Play("slot_digital_reelStop", &instance);
          }
        }

      }
    }

    if (m_numSpinning == 0 && m_spinning)
    {
      m_spinning = false;
      SlotsFinished();
    }
    
  }

  void SlotMachine::Draw()
  {
    Transform* tr = (Transform*)space->GetComponent(eTransform, owner);
    Sprite* sp = (Sprite*)space->GetComponent(eSprite, owner);
    Draw::SetRotation(0);
    Vec2 trsc = tr->GetScale();

    Vec2 finalSlotSize = slotSize & trsc;
    Vec2 finalBackSize = finalSlotSize & slotBackSize;
    Vec2 finalSlotDim = finalSlotSize & m_slotDimensions;
    float finalMargin = slotMargin * trsc.x;

    float xPos = tr->GetTranslation().x -
      ((numSlots - 1) / 2.0f) * (finalSlotDim.x + finalMargin) +
      (slotOffset.x);
    //-      (numSlots / 2.0f) * (finalSlotDim.x + finalMargin);

    float yPos = tr->GetTranslation().y + slotOffset.y * trsc.y
      + slotOffset.y;

    
    for (unsigned int i = 0; i < m_slots.size(); ++i)
    {
      Slot* slot = &m_slots[i];

      if (useSpriteColor && sp)
        Draw::SetColor(sp->Color.r, sp->Color.g, sp->Color.b, 1);
      else
        Draw::SetColor(backingColor.r, backingColor.g, backingColor.b, backingColor.a);

      Draw::SetTexture(m_slotbackTextureID);
      Draw::ForceZ(true, tr->GetTranslation().z - 0.01f);
      Draw::DrawTexturedRect(xPos, yPos, finalBackSize.x, finalBackSize.y);


      Draw::SetColor(1, 1, 1, 1);
      Draw::SetUVs(Vec2(0, slot->spinPos), Vec2(1, slot->spinPos + slot->height));
      
      if (slot->spinning)
        Draw::SetTexture(slot->spinTextureID);
      else
        Draw::SetTexture(slot->stopTextureID);
      
      Draw::ForceZ(true, tr->GetTranslation().z - 0.02f);
      Draw::DrawTexturedRect(xPos, yPos, finalSlotSize.x, finalSlotSize.y);

      xPos += finalSlotDim.x + finalMargin;
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

    switch (m_layer)
    {
    case 2:
      space->hooks.Remove("PostDraw", self);
      break;
    case 0:
      space->hooks.Remove("PreDraw", self);
      break;
    case 1:
    default:
      space->hooks.Remove("Draw", self);
      break;
    }
  }
}