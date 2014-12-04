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
	class WeaponPickup : public GameComponent
	{
	public:
    WeaponPickup();
    ~WeaponPickup();
		void LogicUpdate(float dt);
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();

		//member variables
    Handle wpTransfrom;
    Handle wpCollider;
    EWeapons weaponID = eNoWeapon;
    int weaponNum;
  private:
    const TypeInfo *weaponType = nullptr;
	};
}