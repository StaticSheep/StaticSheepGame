/*****************************************************************
Filename: CSlotController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CSlotController.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../slotmachine/slotmachine.h"

namespace Framework
{
  SlotController::SlotController()
	{
    done = false;
    levelTimer = 3.0f;
    spawnedSM = nullptr;
    Stype = GOLD;
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
    
	}

  void SlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void SlotController::LogicUpdate(float dt)
	{
    if (done) //if this slot machine is done and there are no spawned slot machine
    {
      levelTimer -= dt;
      if (levelTimer <= 0)
      {
        if (Stype == GOLD)
        {
          //send message
          space->hooks.Call("SetMods", mod1, mod2);
          space->hooks.Call("SlotFinished", mode);
        }
        space->GetGameObject(owner)->Destroy();
      }
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
    case JACKPOT:
        *spinTexID = Draw::GetTextureID("slot_test_blur.png");
        *stopTexID = Draw::GetTextureID("slot_test.png");
      break;
    case INDIVIDUAL:

      break;
    }

    //*spinTexID = Draw::GetTextureID(/*string goes here*/);
  }

  void SlotController::SetSMResults(int slotNum, int *landResult)
  {

  }

  void SlotController::ReceiveSMResults(std::vector<int>* results)
  {
    if (Stype == GOLD)
    {
      if (CheckForJP(*results))
      {
        spawnedSM = SpawnChildSM(JACKPOT);
        Transform *origin = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
        Transform *spawnedTransform = spawnedSM->GetComponent<Transform>(eTransform);
        spawnedTransform->SetTranslation(origin->GetTranslation() + Vec3(0.0f, -260.0f, 0.0f));
        spawnedSM->GetComponent<SlotController>(eSlotController)->Stype = JACKPOT;
        levelTimer += 4.0f;
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
    else if (Stype == JACKPOT)
    {
      if (CheckForJP(*results))
      {
        for (int i = 0; i < 9; ++i)
        {
          float ranX = GetRandom(-600, 600);
          float ranY = GetRandom(-300, 300);
          Vec3 pos(ranX, ranY, 0.0f);
          space->hooks.Call("SpawnCoins", pos);
        }
        //spawn fan balls
      }
    }
    
    done = true;
  }

  bool SlotController::CheckForJP(std::vector<int> results)
  {
    if (Stype == GOLD)
    {
      if (results[1] == 1 || results[2] == 1)
        return true;
    }
    else if (Stype == JACKPOT)
    {
      if (results[1] == 1 && results[2] == 1 && results[0] == 1)
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
    else if (type == JACKPOT)
    {
      return (FACTORY->LoadObjectFromArchetype(space, "JPSlotMachine"));
    }
    else //individual
    {
      return (FACTORY->LoadObjectFromArchetype(space, "JPSlotMachine"));
    }

  }
}
