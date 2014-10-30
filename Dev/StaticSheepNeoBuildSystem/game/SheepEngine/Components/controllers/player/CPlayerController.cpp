
#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "components/transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"


#include "systems/input/Input.h"

namespace Framework
{
	PlayerController::PlayerController() //1
	{
		//set defaults
		playerNum = 0;
		playerGamePad = Handle::null;
		isSnapped = true;
		hasFired = false;
		snappedTo = NULL;
	}

	PlayerController::~PlayerController() //4
	{
		//release dynamic memory
	}


	void PlayerController::Initialize() //2
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PlayerController::LogicUpdate));
		space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PlayerController::OnCollision));

		playerGamePad = space->GetGameObject(owner)->GetComponentHandle(eGamePad); //gets the handle to the gamepad
		playerCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
		playerTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);

		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad); //actually gets the gamepad
		gp->SetPad(playerNum); //setting pad number

		aimDir.x = 1;
		aimDir.y = 0;
	}

	void PlayerController::LogicUpdate(float dt)
	{
		//get the game pad
		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
		//get the box collider of player
		BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);

		if (gp->RStick_InDeadZone() == false)       //if the right stick is NOT inside of its dead zone
			aimDir = aimingDirection(gp); //get the direction the player is currently aiming;

		//fire on trigger pull
		if (gp->RightTrigger() /*&& hasFired == false*/)
		{
			hasFired = true;
			onFire();
		}
		//if the trigger is released, reset the bool
		if (!gp->RightTrigger())
			hasFired = false;
///////////////////////////////////////////////////////////////
    if (isSnapped)
    {
      bc->SetVelocity(snappedNormal * 25);
      //left stick move
      if (gp->LeftStick_X() > 0.2)
      {
        bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 150));

      }
      else if (gp->LeftStick_X() < -0.2)
      {
        bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        bc->AddToVelocity((snappedNormal.CalculateNormal() * 150));
      }

      //jump
      if (gp->ButtonPressed(XButtons.A) && isSnapped)
      {
        bc->AddToVelocity(-(snappedNormal * 300));
        isSnapped = false;
      }
      else if (!gp->ButtonPressed(XButtons.A))
      {
        //isSnapped = true;
      }
    }
    else
    {

    }
////////////////////////////////////////////////////////////////
		//melee
		if (gp->ButtonPressed(XButtons.B))
		{

		}

		if (gp->ButtonPressed(XButtons.X))
		{
			//bc->AddToAngVelocity(.5f);
		}


    if (SHEEPINPUT->Keyboard.KeyIsPressed('A'))
    {
      //ENGINE->ChangeLevel("Level2");
    }

		
	}

	void PlayerController::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
		isSnapped = true;
		//get the thing we are colliding with
		GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
		//get the transform of the thing we are colliding with
		Transform *OOT = OtherObject->GetComponent<Transform>(eTransform);
		//if that thing we collided with's transform is missing, get the fuck otta here, i mean what are you even doing?
		if (!OOT)
			return;

		if (OtherObject->HasComponent(eBoxCollider))
		{
      BoxCollider *OOBc = OtherObject->GetComponent<BoxCollider>(eBoxCollider);
      snappedNormal = OOBc->GetCollisionNormals(manifold);
		}
		else if (OtherObject->HasComponent(eBoxCollider))
		{
      CircleCollider *OOCc = OtherObject->GetComponent<CircleCollider>(eCircleCollider);
      snappedNormal = OOCc->GetCollisionNormals(manifold);
		}

	}

	void PlayerController::Remove() //3
	{
		//opposite of init
		space->hooks.Remove("LogicUpdate", self);
	}

	void PlayerController::onFire()
	{
		GameObject *bullet = (FACTORY->LoadObjectFromArchetype(space, "Bullet"));
		Transform *BT = bullet->GetComponent<Transform>(eTransform);
		CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
		Transform *playerTrans = space->GetHandles().GetAs<Transform>(playerTransform);
		BT->SetTranslation(playerTrans->GetTranslation() + aimDir * 25);
		bulletC->AddToVelocity(aimDir * 1000);
	}

	Vec3 PlayerController::aimingDirection(GamePad *gp)
	{
		Vec3 returnVec;
    float thresh = 0.7f; //the threshold minimum for aiming

		returnVec.x = gp->RightStick_X() * 2;
		returnVec.y = gp->RightStick_Y() * 2;
    //you're about to see a lot of if statements, just know that all of these are 
    //making sure that the default return vector is within a certain range so that
    //when bullets spawn using that return vector they don't spawn to close to the player.
    if (returnVec.x > 1.0)
      returnVec.x = 1.0;
    if (returnVec.y > 1.0)
      returnVec.y = 1.0;

    if (returnVec.x < -1.0)
      returnVec.x = -1.0;
    if (returnVec.y < -1.0)
      returnVec.y = -1.0;

    if (returnVec.x < thresh && returnVec.x > 0 && !(returnVec.y >= thresh || returnVec.y <= -thresh))
      returnVec.x = thresh;
    if (returnVec.y < thresh && returnVec.y > 0 && !(returnVec.x >= thresh || returnVec.x <= -thresh))
      returnVec.y = thresh;

    if (returnVec.x > -thresh && returnVec.x < 0)
      returnVec.x = -thresh;
    if (returnVec.y > -thresh && returnVec.y < 0)
      returnVec.y = -thresh;

		return returnVec;

	}
}