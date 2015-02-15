/*****************************************************************
Filename: CAsteroid.h
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../transform/CTransform.h"

namespace Framework
{
	class Asteroid : public GameComponent
	{
	public:
    Asteroid();
    ~Asteroid();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

		//member variables
    Handle aTransfrom;
    Handle aCollider;
	};
}