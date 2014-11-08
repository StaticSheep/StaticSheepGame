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
    { ePistol, "Pistol" }
  };

  WeaponPickup::WeaponPickup()
	{

	}

  WeaponPickup::~WeaponPickup()
	{

	}

  void WeaponPickup::Initialize()
	{
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
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eCircleCollider))->SetGravityOff();
	}

  void WeaponPickup::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  void WeaponPickup::LogicUpdate(float dt)
	{
    Transform *bt = space->GetHandles().GetAs<Transform>(wpTransfrom);

    if (bt->GetTranslation().x > 700 || bt->GetTranslation().x < -700 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
      space->GetGameObject(owner)->Destroy();
	}

  void WeaponPickup::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    if (OtherObject->name == "Player" || "KillBox")
    {
      space->GetHandles().GetAs<PlayerController>(otherObject)->weapon = (Weapon *)weaponType->New();
      space->GetGameObject(owner)->Destroy();
    }
		

	}


}
