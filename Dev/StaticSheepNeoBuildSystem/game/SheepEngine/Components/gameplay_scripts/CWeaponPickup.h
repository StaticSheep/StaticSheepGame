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
  private:
    const TypeInfo *weaponType = nullptr;
	};
}