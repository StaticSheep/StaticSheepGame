#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Explosion : public GameComponent
	{
	public:
    Explosion();
    ~Explosion();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();

		//member variables
    Handle eTransfrom;
    Handle eAnSprite;
	};
}