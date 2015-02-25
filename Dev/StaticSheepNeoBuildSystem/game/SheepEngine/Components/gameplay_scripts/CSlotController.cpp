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
    Stype = GOLD;
    done = false;
    levelTimer = 2.0f;
	}

  SlotController::~SlotController()
	{

	}

  void SlotController::Initialize()
	{
    Stype = GOLD;
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SlotController::LogicUpdate));
    //gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    //gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
    Handle SM = space->GetGameObject(owner)->GetComponentHandle(eSlotMachine);
    SlotMachine *sm = space->GetHandles().GetAs<SlotMachine>(SM);
    sm->SetTextureCB(self, BUILD_FUNCTION(SlotController::SetSMTextures));
    sm->SetFinishedCB(self, BUILD_FUNCTION(SlotController::ReceiveSMResults));
    //when you create a slot machine through an archetype you can set its call backs like this

	}

  void SlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void SlotController::LogicUpdate(float dt)
	{
    if (done)
    {
      levelTimer -= dt;
      if (levelTimer <= 0)
        space->GetGameObject(owner)->Destroy();
    }
	}

  void SlotController::SetSMTextures(int slotNum, int *spinTexID, int *stopTexID)
  {
    switch (Stype)
    {
    case GOLD:
      if (slotNum == 1 || slotNum == 2)
      {
        *spinTexID = Draw::GetTextureID("slot_test_blur.png");
        *stopTexID = Draw::GetTextureID("slot_test.png");
      }
      else
      {
        *spinTexID = Draw::GetTextureID("slot_test_blur.png");
        *stopTexID = Draw::GetTextureID("slot_test.png");
      }
      break;
    case JACKPOT:

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
    /*
    if(CheckForJP(results))
      //run function for special jackpot
    if(
    */
    done = true;
  }

}
