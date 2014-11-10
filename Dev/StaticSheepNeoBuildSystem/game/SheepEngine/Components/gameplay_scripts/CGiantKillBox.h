#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class GiantKillBox : public GameComponent
	{
	public:
    GiantKillBox();
    ~GiantKillBox();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle kbTransfrom;
    Handle kbCollider;
    bool direction;
	};
}