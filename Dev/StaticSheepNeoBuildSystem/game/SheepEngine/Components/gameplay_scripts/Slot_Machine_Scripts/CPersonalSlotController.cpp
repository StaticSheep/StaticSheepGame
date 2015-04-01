/*****************************************************************
Filename: CSlotController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CPersonalSlotController.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../slotmachine/slotmachine.h"
#include "../../sound/CSoundEmitter.h"
#include "../../basicps/CBasicPSystem.h"

namespace Framework
{
  PersonalSlotController::PersonalSlotController()
	{
    done = false;
    timer = 5.0f;
    playerNum = 0;
	}

  PersonalSlotController::~PersonalSlotController()
	{

	}

  void PersonalSlotController::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PersonalSlotController::LogicUpdate));

    Handle SM = space->GetGameObject(owner)->GetComponentHandle(eSlotMachine);
    SlotMachine *sm = space->GetHandles().GetAs<SlotMachine>(SM);
    //when you create a slot machine through an archetype you can set its call backs like this
    sm->SetTextureCB(self, BUILD_FUNCTION(PersonalSlotController::SetSMTextures));
    sm->SetFinishedCB(self, BUILD_FUNCTION(PersonalSlotController::ReceiveSMResults));
    //sm->SetSelectionCB(self, BUILD_FUNCTION(SlotController::SetSMResults));

    bounceDownTimer = 0.5f;
    bounceDownDone = false;
    space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter)->Play("slot_digital_coin", &SoundInstance(1.0f));
	}

  void PersonalSlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void PersonalSlotController::LogicUpdate(float dt)
	{
    if (!bounceDownDone)
    {
      BounceDown(dt);
      return;
    }
	}
  static bool soundFlag_ = false;
  void PersonalSlotController::BounceDown(float dt)
  {
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);

    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    if (bounceDownTimer >= 0.3f)
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -80.0f, 0.0f));
    else if (bounceDownTimer >= 0.2)
    {
      if (!soundFlag_)
      {
        se->Play("impact1", &SoundInstance(1.0f));
        soundFlag_ = true;
      }
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 40.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() + 0.035f);
    }
    else
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -20.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() - 0.017f);
    }

    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      rt->SetRotation(0.0f);
      bounceDownDone = true;
    }
  }

  void PersonalSlotController::SetSMTextures(int slotNum, int *spinTexID, int *stopTexID)
  {
    std::string texID;
    if (playerNum == 0)
      texID = "p1_";
    else if (playerNum == 1)
      texID = "p2_";
    else if (playerNum == 2)
      texID = "p3_";
    else if (playerNum == 3)
      texID = "p4_";
    
    if (psmNum == 1)
    {
      texID.append("reel1.png");
      *spinTexID = Draw::GetTextureID(texID.c_str());
      *stopTexID = Draw::GetTextureID(texID.c_str());
    }
    else if (psmNum == 2)
    {
      texID.append("reel2.png");
      *spinTexID = Draw::GetTextureID(texID.c_str());
      *stopTexID = Draw::GetTextureID(texID.c_str());
    }
    else
    {
      texID.append("reel3.png");
      *spinTexID = Draw::GetTextureID(texID.c_str());
      *stopTexID = Draw::GetTextureID(texID.c_str());
    }
  }

  void PersonalSlotController::SetSMResults(int slotNum, int *landResult)
  {
    
  }

  void PersonalSlotController::ReceiveSMResults(std::vector<int>* results)
  {
    space->GetGameObject(owner)->GetComponent<BasicParticleSystem>(eBasicParticleSystem)->Toggle(false);
    if ((*results)[0] == 0)
    {
      //chip
      space->hooks.Call("GivePlayerChip", playerNum, 1);
    }
    if ((*results)[0] == 1)
    {
      //coin
      space->hooks.Call("GivePlayerCoins", playerNum, 2500);
    }
    if ((*results)[0] == 2)
    {
      //special
      if (psmNum == 0)
      {

      }
      else if (psmNum == 1)
      {

      }
      else if (psmNum == 2)
      {

      }

    }
  }

  bool PersonalSlotController::CheckForJP(std::vector<int> results)
  {
    return true;
  }

}
