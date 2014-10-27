#pragma once

#include "components/base/Component.h"

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

		//member variables
		int playerNum;
		bool hasFired, isSnapped;
		Handle playerGamePad;
		Handle playerCollider;
		Handle playerTransform;
		Handle snappedTo; //the thing we are currently "snapped" to
	};
}