/*****************************************************************
Filename: CBonusSlotController.h
Project:  Gam250
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CBonusSlotController.h"
#include "../../slotmachine/slotmachine.h"
#include "../../transform/CTransform.h"
#include "SheepMath.h"
#include "CBonusSlotManager.h"

namespace Framework
{

  void BonusSlotController::Initialize()
  {
    SlotMachine* sm = GetOwner()->GetComponent<SlotMachine>(eSlotMachine);

    sm->SetTextureCB(self, BUILD_FUNCTION(BonusSlotController::SetSMTextures));
    sm->SetFinishedCB(self, BUILD_FUNCTION(BonusSlotController::ReceiveSMResults));
    sm->SetSelectionCB(self, BUILD_FUNCTION(BonusSlotController::SetSMResults));

    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(
      BonusSlotController::LogicUpdate));

    bounceDownTimer = 0.5f;
  }

  void BonusSlotController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void BonusSlotController::BounceDown(float dt)
  {
    Transform* tr = GetOwner()->GetComponent<Transform>(eTransform);

    if (bounceDownTimer >= 0.3f)
    {
      if (leftHand)
        tr->SetTranslation(tr->GetTranslation() + Vec3(-50.0f, 0.0f, 0.0f));
      else
        tr->SetTranslation(tr->GetTranslation() + Vec3(50.0f, 0.0f, 0.0f));
    }
    else if (bounceDownTimer >= 0.2)
    {
      if (leftHand)
        tr->SetTranslation(tr->GetTranslation() + Vec3(20.0f, 0.0f, 0.0f));
      else
        tr->SetTranslation(tr->GetTranslation() + Vec3(-20.0f, 0.0f, 0.0f));
    }
    else
    {
      if (leftHand)
        tr->SetTranslation(tr->GetTranslation() + Vec3(-10.0f, 0.0f, 0.0f));
      else
        tr->SetTranslation(tr->GetTranslation() + Vec3(10.0f, 0.0f, 0.0f));
    }

    bounceDownTimer -= dt;
  }

  void BonusSlotController::LogicUpdate(float dt)
  {
    if (!done && bounceDownTimer > 0.0f)
      BounceDown(dt);



    if (done)
    {
      bounceDownTimer -= dt;

      if (bounceDownTimer <= 0)
      {
        Transform* tr = GetOwner()->GetComponent<Transform>(eTransform);

        if (leftHand)
          tr->SetTranslation(tr->GetTranslation() + Vec3(60.0f, 0.0f, 0.0f));
        else
          tr->SetTranslation(tr->GetTranslation() + Vec3(-60.0f, 0.0f, 0.0f));

        if (tr->GetTranslation().x <= 40.0f && tr->GetTranslation().x >= -40.0f)
          space->GetGameObject(owner)->Destroy();
      }
      
    }

  }

  void BonusSlotController::SetSMTextures(int slotNum, int* spinTexID, int* stopTexID)
  {
    if (slotNum == 0)
    {
      *spinTexID = Draw::GetTextureID("slot_coin_blur.png");
      *stopTexID = Draw::GetTextureID("slot_coin.png");
    }
    else if (slotNum == 1)
    {
      *spinTexID = Draw::GetTextureID("slot_coin_blur.png");
      *stopTexID = Draw::GetTextureID("slot_coin.png");
    }
    else if (slotNum == 2)
    {
      *spinTexID = Draw::GetTextureID("slot_coin_blur.png");
      *stopTexID = Draw::GetTextureID("slot_coin.png");
    }
  }

  void BonusSlotController::ReceiveSMResults(std::vector<int>* results)
  {
    BonusSlotManager::BResultData resData;
    resData.mod = (*results)[0] == 0 ? BonusSlotManager::MOST : BonusSlotManager::LEAST;
    resData.type = (BonusSlotManager::BONUSTYPES)(*results)[1];
    resData.prize = (BonusSlotManager::BONUSPRIZE)(*results)[2];
    resData.left = leftHand;

    space->hooks.Call("BonusResult", resData);

    bounceDownTimer = 1.0f;

    done = true;
  }

  void BonusSlotController::SetSMResults(int slotNum, int* res)
  {
    int randNumber = GetRandom(1, 100);

    if (slotNum == 0)
    {
      if (randNumber < 50)
        *res = 0; // Most
      else
        *res = 1; // Least
    }
    else if (slotNum == 1)
    {
      if (randNumber < 40)
        *res = 0;
      else if (randNumber < 60)
        *res = 1;
      else
        *res = 2;
    }
    else if (slotNum == 2)
    {
      if (randNumber < 50)
        *res = 0;
      else if (randNumber < 60)
        *res = 1;
      else
        *res = 2;
    }

    
  }



}