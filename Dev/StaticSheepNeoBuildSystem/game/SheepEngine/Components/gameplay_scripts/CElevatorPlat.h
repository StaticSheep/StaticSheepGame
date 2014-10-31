#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class ElevatorPlat : public GameComponent
	{
	public:
    ElevatorPlat();
    ~ElevatorPlat();
		void LogicUpdate(float dt);
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();

		//member variables
    Handle pTransfrom;
    Handle pCollider;
    bool direction;
	};
}