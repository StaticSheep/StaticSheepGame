/*****************************************************************
Filename: CDashEffect.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class DashEffect : public GameComponent
	{
	public:
    DashEffect();
    ~DashEffect();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle effectTransform;
    Handle pTransform;
    float lifeTime;
    bool alive;
    float deathTimer;
	};
}