/*****************************************************************
Filename: CAimingArrow.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CAimingArrow.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"
#include "../../controllers/player/CPlayerController.h"


namespace Framework
{
  AimingArrow::AimingArrow()
	{
   
	}

  AimingArrow::~AimingArrow()
	{

	}

  void AimingArrow::Initialize()
	{
    arrowSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite); //gets the handle to the gamepad
    arrowTrans = space->GetGameObject(owner)->GetComponentHandle(eTransform);
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(AimingArrow::LogicUpdate));

    //arrowTrans = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    //arrowSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

	}

  static GamePad *gp;           //players controller
  static Transform *arrowTransform, *pt;
  static Sprite *as;

  void AimingArrow::LogicUpdate(float dt)
  {
    gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
    as = space->GetHandles().GetAs<Sprite>(arrowSprite);
    arrowTransform = space->GetHandles().GetAs<Transform>(arrowTrans);
    pt = space->GetHandles().GetAs<Transform>(playerTransform);
    //arrowTransform = space->GetHandles().GetAs<Transform>(arrowTrans);

    //Transform *pt = space->GetHandles().GetAs<Transform>(eTransfrom);

    if (pt != nullptr && gp->RStick_InDeadZone() == false)     //if the right stick is NOT inside of its dead zone
    {
      Vec3 aimDir = aimingDirection(gp, 'R');           //get the direction the player is currently aiming;

      if (aimDir.x > 0.0f)
        arrowTransform->SetRotation((float)atan(aimDir.y / aimDir.x));
      else
        arrowTransform->SetRotation((float)atan(aimDir.y / aimDir.x) + (float)PI);

      Vec3 arrowOffset = ((pt->GetTranslation()) + (aimDir * 50));
      arrowOffset += (space->GetGameObject(playerController)->GetComponent<BoxCollider>(eBoxCollider)->GetBodyUpNormal() * 25);
      arrowTransform->SetTranslation(arrowOffset);
      
    }
    else
      DestroySelf();
  }

  void AimingArrow::Remove()
  {
		space->hooks.Remove("LogicUpdate", self);
	}

  void AimingArrow::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  //************************************
  // Method:    aimingDirection
  // FullName:  Framework::PlayerController::aimingDirection
  // Access:    public 
  // Returns:   Framework::Vec3
  // Qualifier:
  // Parameter: GamePad * gp
  //************************************
  Vec3 AimingArrow::aimingDirection(GamePad *gp, char stick)
  {
    Vec3 returnVec;

    if (stick == 'L')
    {
      returnVec.x = gp->LeftStick_X();
      returnVec.y = gp->LeftStick_Y();
    }
    else
    {
      //here I'm grabbing where the right stick's x and y is and saving that to a vector
      returnVec.x = gp->RightStick_X();
      returnVec.y = gp->RightStick_Y();
    }
    //then I normalize that vector and multiply it by a constant (1.5)
    returnVec.Normalize();
    returnVec *= 1.5;

    return returnVec;
  }

}
