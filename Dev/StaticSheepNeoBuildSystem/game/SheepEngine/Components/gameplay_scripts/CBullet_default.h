#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Bullet_Default : public GameComponent
	{
	public:
		Bullet_Default();
		~Bullet_Default();
		void LogicUpdate(float dt);
		void OnCollision(Handle otherObject);
		void Initialize();
		void Remove();

		//member variables

	};
}