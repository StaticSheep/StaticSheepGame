#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Level1_Logic : public GameComponent
	{
	public:
    Level1_Logic();
    ~Level1_Logic();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    //Handle bTransfrom;
    float timeLimit;
    float spawnTimer;
	};
}