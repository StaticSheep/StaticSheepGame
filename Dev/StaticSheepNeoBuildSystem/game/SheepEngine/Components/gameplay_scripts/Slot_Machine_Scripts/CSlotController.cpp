/*****************************************************************
Filename: CSlotController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CSlotController.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../slotmachine/slotmachine.h"
#include "../../sound/CSoundEmitter.h"
#include "CBonusSlotController.h"

namespace Framework
{
  SlotController::SlotController()
	{
    done = false;
    levelTimer = 2.5f;
    spawnedSM = nullptr;
    Stype = GOLD;
    roundNum = 1;
	}

  SlotController::~SlotController()
	{

	}

  void SlotController::Initialize()
	{
    Stype = GOLD;
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SlotController::LogicUpdate));

    Handle SM = space->GetGameObject(owner)->GetComponentHandle(eSlotMachine);
    SlotMachine *sm = space->GetHandles().GetAs<SlotMachine>(SM);
    //when you create a slot machine through an archetype you can set its call backs like this
    sm->SetTextureCB(self, BUILD_FUNCTION(SlotController::SetSMTextures));
    sm->SetFinishedCB(self, BUILD_FUNCTION(SlotController::ReceiveSMResults));
    sm->SetSelectionCB(self, BUILD_FUNCTION(SlotController::SetSMResults));
    bounceDownTimer = 0.5f;
    bounceDownDone = false;
    spawnLeftBonus = false;
    spawnRightBonus = false;
	}

  void SlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void SlotController::LogicUpdate(float dt)
	{
    if (!bounceDownDone)
      BounceDown(dt);

    if (done) //if this slot machine is done and there are no spawned slot machine
    {
      levelTimer -= dt;
      if (levelTimer <= 0)
      {
        Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
        if (Stype == GOLD)
          rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 80.0f, 0.0f));

        else if (Stype == JACKPOT)
        {
          if (LeftHandBonus)
            rt->SetTranslation(rt->GetTranslation() + Vec3(60.0f, 0.0f, 0.0f));
          else
            rt->SetTranslation(rt->GetTranslation() + Vec3(-60.0f, 0.0f, 0.0f));
        }

        if (Stype == GOLD)
        {
          if (rt->GetTranslation().y >= 950.0f)
          {
            //send message
            space->hooks.Call("SetMods", mod1, mod2);
            space->hooks.Call("SlotFinished", mode);
            space->GetGameObject(owner)->Destroy();
          }
        }
        else if (Stype == JACKPOT)
        {
          if (rt->GetTranslation().x <= 40.0f && rt->GetTranslation().x >= -40.0f)
            space->GetGameObject(owner)->Destroy();
        }
      }
    }
	}

  static bool soundFlag_ = false;
  void SlotController::BounceDown(float dt)
  {
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);

    if (Stype == GOLD)
    {
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
    }
    else if (Stype == JACKPOT)
    {
      if (bounceDownTimer >= 0.3f)
      {
        if (LeftHandBonus)
          rt->SetTranslation(rt->GetTranslation() + Vec3(-50.0f, 0.0f, 0.0f));
        else
          rt->SetTranslation(rt->GetTranslation() + Vec3(50.0f, 0.0f, 0.0f));
      }
      else if (bounceDownTimer >= 0.2)
      {
        if (LeftHandBonus)
          rt->SetTranslation(rt->GetTranslation() + Vec3(20.0f, 0.0f, 0.0f));
        else
          rt->SetTranslation(rt->GetTranslation() + Vec3(-20.0f, 0.0f, 0.0f));
      }
      else
      {
        if (LeftHandBonus)
          rt->SetTranslation(rt->GetTranslation() + Vec3(-10.0f, 0.0f, 0.0f));
        else
          rt->SetTranslation(rt->GetTranslation() + Vec3(10.0f, 0.0f, 0.0f));
      }
    }

    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      rt->SetRotation(0.0f);
      bounceDownDone = true;
    }
  }

  void SlotController::SetSMTextures(int slotNum, int *spinTexID, int *stopTexID)
  {
    if (StypeInt == 0)
      Stype = GOLD;
    else if (StypeInt == 1)
      Stype = JACKPOT;
    else if (StypeInt == 2)
      Stype = INDIVIDUAL;

    switch (Stype)
    {
    case GOLD:
      if (slotNum == 1)
      {
        *spinTexID = Draw::GetTextureID("game_mod1_reel.png");
        *stopTexID = Draw::GetTextureID("game_mod1_reel.png");
      }
      else if (slotNum == 2)
      {
        *spinTexID = Draw::GetTextureID("game_mod2_reel.png");
        *stopTexID = Draw::GetTextureID("game_mod2_reel.png");
      }
      else
      {
        *spinTexID = Draw::GetTextureID("game_mode_reel.png");
        *stopTexID = Draw::GetTextureID("game_mode_reel.png");
      }
      break;

      //Jon - change this
      //textures and images for slot
    case JACKPOT:
        *spinTexID = Draw::GetTextureID("slot_coin_blur.png");
        *stopTexID = Draw::GetTextureID("slot_coin.png");
      break;
    case INDIVIDUAL:

      break;
    }

    //*spinTexID = Draw::GetTextureID(/*string goes here*/);
  }

  void SlotController::SetSMResults(int slotNum, int *landResult)
  {
    SetNextWeights(slotNum);
    int ranChance = GetRandom(1, 100);

    if (ranChance < slotWeights[0])
      *landResult = 0;
    else if (ranChance >= slotWeights[0] && ranChance < slotWeights[0] + slotWeights[1])
      *landResult = 1;
    else if (ranChance >= 100 - slotWeights[2])
      *landResult = 2;
    else
      *landResult = 1;
  }

  void SlotController::ReceiveSMResults(std::vector<int>* results)
  {
    if (Stype == GOLD)
    {
      if (CheckForJP(*results))
      {
        //Jon
        //fix this, make one call with translation
        //slot creation
        if (spawnLeftBonus)
        {
          spawnedSM = SpawnChildSM(JACKPOT);
          Transform *origin = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
          Transform *spawnedTransform = spawnedSM->GetComponent<Transform>(eTransform);
          spawnedTransform->SetTranslation(origin->GetTranslation() + Vec3(0.0f, 0.0f, 0.0f));
          spawnedSM->GetComponent<BonusSlotController>
            (eBonusSlotController)->leftHand = true;
        }
        if (spawnRightBonus)
        {
          spawnedSM = SpawnChildSM(JACKPOT);
          Transform *origin = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
          Transform *spawnedTransform = spawnedSM->GetComponent<Transform>(eTransform);
          spawnedTransform->SetTranslation(origin->GetTranslation() + Vec3(0.0f, 0.0f, 0.0f));
          
        }
        if (spawnRightBonus || spawnLeftBonus)
          levelTimer += 3.5f;

      }
      if ((*results)[0] == 0)
        mode = FFA;
      else if ((*results)[0] == 1)
        mode = JUGGERNAUT;
      else if ((*results)[0] == 2)
        mode = SUDDENDEATH;

      if ((*results)[1] == 0)
        mod1 = LIGHTSOUT;
      else if ((*results)[1] == 1)
        mod1 = BONUS;
      else if ((*results)[1] == 2)
        mod1 = EXPLOSIVEROUNDS;

      if ((*results)[2] == 0)
        mod2 = SHOTGUNS;
      else if ((*results)[2] == 1)
        mod2 = BONUS;
      else if ((*results)[2] == 2)
        mod2 = ROCKETS;
    }

    //Jon
    //results of slot
    else if (Stype == JACKPOT)
    {

      /*int coinBallSpawns = 0;
      if (CheckForJP(*results))
      {
        coinBallSpawns += 5;
      }
      else
      {
        for (int i = 0; i < 3; ++i)
        {
          if ((*results)[i] == 0)
            coinBallSpawns += 2;
          else if ((*results)[i] == 1)
            coinBallSpawns += 6;
          else if ((*results)[i] == 2)
            coinBallSpawns += 4;
        }
      }
      coinBallSpawns = coinBallSpawns / 5;
      for (int i = 0; i < coinBallSpawns; ++i)
      {
        float ranX = GetRandom(-600, 600);
        float ranY = GetRandom(-300, 300);
        Vec3 pos(ranX, ranY, 0.0f);
        space->hooks.Call("SpawnItem", "CoinBall", pos);
      }*/
    }
    
    done = true;
  }

  bool SlotController::CheckForJP(std::vector<int> results)
  {
    if (Stype == GOLD)
    {
      bool retValue = false;
      if (results[1] == 1)
      {
        spawnLeftBonus = true;
        retValue = true;
      }
      if (results[2] == 1)
      {
        spawnRightBonus = true;
        retValue = true;
      }
      
      return retValue;
    }

    //Jon
    //results of slot machine?
    else if (Stype == JACKPOT)
    {
      if (results[0] == 2 && results[1] == 2 && results[2] == 2)
        return true;
    }

    return false;
  }

  GameObject* SlotController::SpawnChildSM(SlotType type)
  {
    if (type == GOLD)
    {
      return (FACTORY->LoadObjectFromArchetype(space, "BigSlotMachine"));
    }

    //Jon - create child 
    else if (type == JACKPOT)
    {
      return (FACTORY->LoadObjectFromArchetype(space, "JPSlotMachine"));
    }
    else //individual
    {
      return (FACTORY->LoadObjectFromArchetype(space, "JPSlotMachine"));
    }

  }

  void SlotController::SetNextWeights(int reel)
  {
    if (reel == 0) //game mode
    {
      switch (roundNum)
      {
      case 1:
      case 2:
      case 3:
        slotWeights[0] = 50;
        slotWeights[1] = 50;
        slotWeights[2] = 0;
        break;
      default:
        slotWeights[0] = 40;
        slotWeights[1] = 40;
        slotWeights[2] = 20;
      }

      int div = 0;
      switch (lastMode)
      {
      case FFA:
        div = slotWeights[0] / 2;
        slotWeights[0] = 0;
        slotWeights[1] += div;
        slotWeights[2] += div;
        break;
      case JUGGERNAUT:
        div = slotWeights[1] / 2;
        slotWeights[0] += div;
        slotWeights[1] = 0;
        slotWeights[2] += div;
        break;
      case SUDDENDEATH:
        if (roundNum < 3)
          break;
        div = slotWeights[2] / 2;
        slotWeights[0] += div;
        slotWeights[1] += div;
        slotWeights[2] = 0;
        break;
      default:
        break;
      }
    }
    else if (reel == 1) //mod 1
    {
      switch (roundNum)
      {
      case 1:
      case 2:
        slotWeights[0] = 0;
        slotWeights[1] = 80;
        slotWeights[2] = 20;
        break;
      default:
        slotWeights[0] = 30;
        slotWeights[1] = 60;
        slotWeights[2] = 10;
      }
    }
    else if (reel == 2) //mod 2
    {
      slotWeights[0] = 50;
      slotWeights[1] = 45;
      slotWeights[2] = 5;
    }
  }
}
