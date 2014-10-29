
#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "components/transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"

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

		//left stick move
		if (gp->LeftStick_X() > 0.2)
		{
			bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
			bc->AddToVelocity(Vec3(150.0f, 0.0f, 0.0f));
		}
		else if (gp->LeftStick_X() < -0.2)
		{
			bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
			bc->AddToVelocity(Vec3(-150.0f, 0.0f, 0.0f));
		}

		//jump
		if (gp->ButtonPressed(XButtons.A) && isSnapped)
		{
			bc->AddToVelocity(Vec3(0.0f, 100.0f, 0.0f));
			isSnapped = false;
		}
		else if (!gp->ButtonPressed(XButtons.A))
		{
			//isSnapped = true;
		}
		//melee
		if (gp->ButtonPressed(XButtons.B))
		{

		}

		if (gp->ButtonPressed(XButtons.X))
		{
			//bc->AddToAngVelocity(.5f);
		}


		
	}

	void PlayerController::OnCollision(Handle otherObject)
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
			BoxCollider *OOBc = space->GetHandles().GetAs<BoxCollider>(otherObject);


		}
		else if (OtherObject->HasComponent(eBoxCollider))
		{
			CircleCollider *OOCc = space->GetHandles().GetAs<CircleCollider>(otherObject);

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
		returnVec.x = gp->RightStick_X();
		returnVec.y = gp->RightStick_Y();

		return returnVec;

	}
}