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
		void OnCollision(Handle otherObject);
		void Initialize();
		void Remove();
		void onFire();
		Vec3 aimingDirection(GamePad *gp);

		//member variables
		int playerNum; //the player number, i.e. 1, 2, 3, 4
		bool hasFired, isSnapped; //has fired is a flag to prevent fully auto fire, is snapped is a bool to see if the player is snapped to something
		Vec3 aimDir;  //the direction the player is currently aiming
		Handle playerGamePad;
		Handle playerCollider;
		Handle playerTransform;
		Handle snappedTo; //the thing we are currently "snapped" to
	};
}