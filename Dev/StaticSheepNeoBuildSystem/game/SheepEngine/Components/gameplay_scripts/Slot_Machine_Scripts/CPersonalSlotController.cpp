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

namespace Framework
{
  PersonalSlotController::PersonalSlotController(PersonalSlotOwner owner) : owner_(owner)
	{
    done = false;
    timer = 5.0f;
    spawnedSM = nullptr;
    roundNum = 1;
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
	}

  void PersonalSlotController::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void PersonalSlotController::LogicUpdate(float dt)
	{
    
	}

  void PersonalSlotController::BounceDown(float dt)
  {
    
  }

  void PersonalSlotController::SetSMTextures(int slotNum, int *spinTexID, int *stopTexID)
  {
    
  }

  void PersonalSlotController::SetSMResults(int slotNum, int *landResult)
  {
    
  }

  void PersonalSlotController::ReceiveSMResults(std::vector<int>* results)
  {
    
  }

  bool PersonalSlotController::CheckForJP(std::vector<int> results)
  {
    return true;
  }

}
