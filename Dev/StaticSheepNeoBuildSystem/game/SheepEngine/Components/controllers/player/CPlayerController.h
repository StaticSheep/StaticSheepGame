#pragma once

#include "components/base/Component.h"
#include "components/gamepad/CGamePad.h"

namespace Framework
{
	class PlayerController : public GameComponent
	{
	public:
		PlayerController();
		~PlayerController();
		void LogicUpdate(float dt);
		void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();
		void onFire();
		Vec3 aimingDirection(GamePad *gp);

		//member variables
		int playerNum; //the player number, i.e. 1, 2, 3, 4
		bool hasFired, isSnapped; //has fired is a flag to prevent fully auto fire, is snapped is a bool to see if the player is snapped to something
		Vec3 aimDir;  //the direction the player is currently aiming
    Vec3 snappedNormal; //the normal of the thing we are currently snapped to
		Handle playerGamePad; //the players game pad
		Handle playerCollider; //the players collider (box)
		Handle playerTransform; //the player's transform
		Handle snappedTo; //the thing we are currently "snapped" to
	};
}