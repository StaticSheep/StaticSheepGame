#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../transform/CTransform.h"

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
    Handle Grinders[7];
    Transform *GT[7];
    Handle kbTransfrom;
    Handle kbCollider;
    bool direction, GrindSpawn;
	};
}