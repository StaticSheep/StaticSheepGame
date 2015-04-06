/*****************************************************************
Filename: CSlotController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
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
    sm->SetSelectionCB(self, BUILD_FUNCTION(PersonalSlotController::SetSMResults));

    bounceDownTimer = 0.5f;
    bounceDownDone = false;
    space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter)->Play("slot_digital_coin", &SoundInstance(1.0f));
    gold_ = false;
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
    if (gold_)
    {
      space->GetGameObject(owner)->GetComponent<SlotMachine>(eSlotMachine)->slotOptions = 3;
      *spinTexID = Draw::GetTextureID("gold_reel.png");
      *stopTexID = Draw::GetTextureID("gold_reel.png");
    }
    else
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

      if (psmNum == 0)
      {
        texID.append("reel1.png");
        *spinTexID = Draw::GetTextureID(texID.c_str());
        *stopTexID = Draw::GetTextureID(texID.c_str());
      }
      else if (psmNum == 1)
      {
        texID.append("reel2.png");
        *spinTexID = Draw::GetTextureID(texID.c_str());
        *stopTexID = Draw::GetTextureID(texID.c_str());
      }
      else if (psmNum == 2)
      {
        texID.append("reel3.png");
        *spinTexID = Draw::GetTextureID(texID.c_str());
        *stopTexID = Draw::GetTextureID(texID.c_str());
      }
    }
  }

  void PersonalSlotController::SetSMResults(int slotNum, int *landResult)
  {
    int ranChance = GetRandom(0, 99);
    if (gold_)
    {
      if (ranChance <= 69)
        *landResult = 0; //one chip
      else if (ranChance > 69 && ranChance <= 94)
        *landResult = 1; //two chips
      else //95 - 99
        *landResult = 2; //three chips
      return;
    }

    int weights_[5] = {0,0,0,0,0};
    if (psmNum == 0)
    {
      weights_[0] = 5;  //chip
      weights_[1] = 40; //2,500
      weights_[2] = 35; //powerup
      weights_[3] = 18; //7,500
      weights_[4] = 2;  //15,000
    }
    else if (psmNum == 1)
    {
      weights_[0] = 5;
      weights_[1] = 30;
      weights_[2] = 35;
      weights_[3] = 25;
      weights_[4] = 5;
    }
    else if (psmNum == 2)
    {
      weights_[0] = 5;
      weights_[1] = 20;
      weights_[2] = 30;
      weights_[3] = 32;
      weights_[4] = 13;
    }

    if (ranChance <= weights_[0])
    {
      //chip
      *landResult = 0;
    }
    else if (ranChance > weights_[0] && ranChance <= weights_[0] + weights_[1])
    {
      //2,500
      *landResult = 1;
    }
    else if (ranChance > weights_[0] + weights_[1] && ranChance <= weights_[0] + weights_[1] + weights_[2])
    {
      //powerup
      *landResult = 2;
    }
    else if (ranChance > weights_[0] + weights_[1] + weights_[2] && ranChance <= weights_[0] + weights_[1] + weights_[2] + weights_[3])
    {
      //7,500
      *landResult = 3;
    }
    else if (ranChance > weights_[0] + weights_[1] + weights_[2] && ranChance <= weights_[0] + weights_[1] + weights_[2] + weights_[3] + weights_[4])
    {
      //15,000
      *landResult = 4;
    }

  }

  void PersonalSlotController::ReceiveSMResults(std::vector<int>* results)
  {
    space->GetGameObject(owner)->GetComponent<BasicParticleSystem>(eBasicParticleSystem)->Toggle(false);
    if (gold_)
    {
      if ((*results)[0] == 0)
      {
        space->hooks.Call("GivePlayerChip", playerNum, 1);
      }
      if ((*results)[0] == 1)
      {
        space->hooks.Call("GivePlayerChip", playerNum, 2);
      }
      if ((*results)[0] == 2)
      {
        space->hooks.Call("GivePlayerChip", playerNum, 3);
      }
    }
    else
    {
      if ((*results)[0] == 0)
      {
        //chip
        space->hooks.Call("GivePlayerChip", playerNum, 1);
      }
      if ((*results)[0] == 1)
      {
        //2500
        space->hooks.Call("GivePlayerCoins", playerNum, 2500);
      }
      if ((*results)[0] == 2)
      {
        //special
        if (psmNum == 0)
        {
          //shield
          space->hooks.Call("GivePlayerLoadoutItem", playerNum, BonusShield);
        }
        else if (psmNum == 1)
        {
          //double damage
          space->hooks.Call("GivePlayerLoadoutItem", playerNum, DoubleDamage);
        }
        else if (psmNum == 2)
        {
          //explosive damage
          space->hooks.Call("GivePlayerLoadoutItem", playerNum, ExplosiveRounds);
        }

      }
      if ((*results)[0] == 3)
      {
        //7500
        space->hooks.Call("GivePlayerCoins", playerNum, 7500);
      }
      if ((*results)[0] == 4)
      {
        //15000
        space->hooks.Call("GivePlayerCoins", playerNum, 15000);
      }
    }
  }

  bool PersonalSlotController::CheckForJP(std::vector<int> results)
  {
    return true;
  }

}
