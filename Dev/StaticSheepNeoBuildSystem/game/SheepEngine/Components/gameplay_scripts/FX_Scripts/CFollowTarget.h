/*****************************************************************
Filename: CFollowTarget.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class FollowTarget : public GameComponent
	{
	public:
    FollowTarget();
    ~FollowTarget();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle effectTransform;
    Handle pTransform;
    float lifeTime;
    bool alive, timedLifetime;
    float deathTimer;
	};
}