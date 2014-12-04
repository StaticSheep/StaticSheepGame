/*****************************************************************
Filename: CWeaponPickup.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CWeaponPickup.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "types/weapons/WPistol.h"
#include "../controllers/player/CPlayerController.h"

namespace Framework
{
  //static std::pair<EWeapons, const char*> Weapons[1] = 
  //{
  //  { ePistol, "Pistol" }
  //};

  static std::unordered_map<EWeapons, const char*> WeaponMap =
  {
    { ePistol, "Pistol" },
    { eAutomatic, "Automatic" },
    { eShotgun, "Shotgun" }
  };

  WeaponPickup::WeaponPickup()
	{

	}

  WeaponPickup::~WeaponPickup()
	{

	}

  void WeaponPickup::Initialize()
	{

    //weaponID = (EWeapons)weaponNum;
    if (weaponNum == 1)
      weaponID = eShotgun;
    else if (weaponNum == 2)
      weaponID = eAutomatic;

    if (weaponID == eNoWeapon)
    {
      int randomNumber = 0;
      weaponID = (EWeapons)randomNumber;
      weaponType = GET_STR_TYPE(WeaponMap[weaponID]);
    }
    else
    {
      weaponType = GET_STR_TYPE(WeaponMap[weaponID]);
    }

		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(WeaponPickup::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(WeaponPickup::OnCollision));

    wpTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eBoxCollider))->SetGravityOff();
	}

  void WeaponPickup::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  void WeaponPickup::LogicUpdate(float dt)
	{
    Transform *bt = space->GetHandles().GetAs<Transform>(wpTransfrom);

    if (bt->GetTranslation().x > 1000 || bt->GetTranslation().x < -1000 || bt->GetTranslation().y > 600 || bt->GetTranslation().y < -600)
      space->GetGameObject(owner)->Destroy();
	}

  void WeaponPickup::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetGameObject(otherObject);
    if (OtherObject->name == "Player")
    {
      PlayerController *playerController = OtherObject->GetComponent<PlayerController>(ePlayerController);
      if (playerController->weapon != nullptr)
        free(playerController->weapon);
      playerController->weapon = (Weapon *)weaponType->New();
      space->GetGameObject(owner)->Destroy();
    }

    if (OtherObject->archetype == "KillBox" || OtherObject->archetype == "Grinder" || OtherObject->archetype == "KillBoxBig")
      space->GetGameObject(owner)->Destroy();

	}


}
