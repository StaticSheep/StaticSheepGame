#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Grinder : public GameComponent
	{
	public:
    Grinder();
    ~Grinder();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle gTransfrom;
    Handle gCollider;
	};
}