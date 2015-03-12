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
    void RespawnBlink(float dt);

		//member variables
    Handle wpTransfrom;
    Handle wpCollider;
    Handle wpSprite;
    EWeapons weaponID = eNoWeapon;
    int weaponNum;
    float timeToLive, respawnTimer;
    bool blink;
  private:
    const TypeInfo *weaponType = nullptr;
	};
}