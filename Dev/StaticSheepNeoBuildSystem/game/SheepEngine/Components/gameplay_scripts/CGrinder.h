/*****************************************************************
Filename: CGrinder.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Remove();

		//member variables
    Handle gTransfrom;
    Handle gCollider;
	};
}