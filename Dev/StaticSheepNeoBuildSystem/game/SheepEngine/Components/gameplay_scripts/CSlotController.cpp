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
   
	}

  SlotController::~SlotController()
	{

	}

  void SlotController::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SlotController::LogicUpdate));

    //gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    //gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
    SlotMachine *sm;
    sm->SetTextureCB(self, BUILD_FUNCTION(SlotController::SetSMTextures));
    //when you create a slot machine through an archtype you can set its call backs like this
	}

  void SlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void SlotController::LogicUpdate(float dt)
	{
    //Transform *pt = space->GetHandles().GetAs<Transform>(gTransfrom);
    //BoxCollider *pc = space->GetHandles().GetAs <BoxCollider>(gCollider);

	}

  void SlotController::SetSMTextures(int slotNum, int *spinTexID, int *stopTexID)
  {
    //*spinTexID = Draw::GetTextureID(/*string goes here*/);
  }

  void SlotController::SetSMResults(int slotNum, int *landResult)
  {

  }

  void SlotController::ReceiveSMResults(std::vector<int>* results)
  {

  }

}
