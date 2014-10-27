

#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/gamepad/CGamePad.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "components/transform/CTransform.h"

namespace Framework
{
	PlayerController::PlayerController() //1
	{
		//set defaults
		playerNum = 0;
		playerGamePad = Handle::null;
	}

	PlayerController::~PlayerController() //4
	{
		//release dynamic mem
	}


	void PlayerController::Initialize() //2
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PlayerController::LogicUpdate));

		playerGamePad = space->GetGameObject(owner)->GetComponentHandle(eGamePad); //gets the handle to the gamepad
		playerCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);

		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad); //actually gets the gamepad
		gp->SetPad(playerNum); //setting pad number

		
	}

	void PlayerController::LogicUpdate(float dt)
	{
		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
		BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);

		//fire
		if (gp->RightTrigger())
		{
			onFire();
		}

		//left stick move
		if (gp->LeftStick_X() > 0.5)
		{
			bc->SetVelocity(Vec3(100.0f, 0.0f, 0.0f));
		}
		else if (gp->LeftStick_X() < 0)
		{
			bc->SetVelocity(Vec3(-100.0f, 0.0f, 0.0f));
		}

		//jump
		if (gp->ButtonPressed(XButtons.A))
		{
			bc->SetVelocity(Vec3(0.0f, 100.0f, 0.0f));
		}
		//melee
		if (gp->ButtonPressed(XButtons.B))
		{

		}

	}

	void PlayerController::Remove() //3
	{
		//opposite of init
<<<<<<< HEAD
		space->hooks.Remove("LogicUpdate", self);
=======
    space->hooks.Remove("LogicUpdate", self);
>>>>>>> origin/EditorFixes
	}

	void PlayerController::onFire()
	{
		GameObject *bullet = (FACTORY->LoadObjectFromArchetype(space, "Bullet"));
		Transform *BT = bullet->GetComponent<Transform>(eTransform);
		BT->SetTranslation(Vec3(1.0, 0.0, 0.0));
	}
}