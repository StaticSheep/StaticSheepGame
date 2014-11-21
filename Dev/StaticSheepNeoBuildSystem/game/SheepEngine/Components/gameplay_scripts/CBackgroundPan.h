#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class BackgroundPan : public GameComponent
	{
	public:
    BackgroundPan();
    ~BackgroundPan();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle bTransfrom;
	};
}