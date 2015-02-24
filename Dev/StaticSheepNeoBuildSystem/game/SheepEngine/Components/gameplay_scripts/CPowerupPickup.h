/*****************************************************************
Filename: CWeaponPickup.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class PowerupPickup : public GameComponent
	{
	public:
    PowerupPickup();
    ~PowerupPickup();
		void LogicUpdate(float dt);
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();

		//member variables
    EPowerUps powerUpID = eNoPowerUp;
    Handle puTransfrom;
    Handle puCollider;
    int powerNum;
  private:
    const TypeInfo *powerUpType = nullptr;
	};
}