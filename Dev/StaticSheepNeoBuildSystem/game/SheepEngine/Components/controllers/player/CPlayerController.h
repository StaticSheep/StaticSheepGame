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
		void Initialize();
		void Remove();
		void onFire();

		int playerNum;
		Handle playerGamePad;
		Handle playerCollider;
	};
}