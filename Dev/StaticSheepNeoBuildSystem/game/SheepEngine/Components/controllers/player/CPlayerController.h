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
		bool hasFired;
		Handle playerGamePad;
		Handle playerCollider;
	};
}