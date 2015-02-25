/*****************************************************************
Filename: CWeaponPickup.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CPowerupPickup.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../controllers/player/CPlayerController.h"

namespace Framework
{
  static std::unordered_map<EPowerUps, const char*> PowerUpMap =
  {
    { eShield, "Shield" },
    { ePower, "DamageBoost" },
    { eFans, "Fans" },
    { eMiniMe, "MiniMe" }
  };

  PowerupPickup::PowerupPickup()
	{

	}

  PowerupPickup::~PowerupPickup()
	{

	}

  void PowerupPickup::Initialize()
	{
    if (powerNum == 0)
      powerUpID = eShield;
    else if (powerNum == 1)
      powerUpID = ePower;

    if (powerUpID == eNoPowerUp)
    {
      int default_ = 1;
      powerUpID = (EPowerUps)default_;
      powerUpType = GET_STR_TYPE(PowerUpMap[powerUpID]);
    }
    else
    {
      powerUpType = GET_STR_TYPE(PowerUpMap[powerUpID]);
    }
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PowerupPickup::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PowerupPickup::OnCollision));

    puTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eBoxCollider))->SetGravityOff();
	}

  void PowerupPickup::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  void PowerupPickup::LogicUpdate(float dt)
	{
    Transform *bt = space->GetHandles().GetAs<Transform>(puTransfrom);

    if (bt->GetTranslation().x > 1000 || bt->GetTranslation().x < -1000 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
      space->GetGameObject(owner)->Destroy();
	}

  void PowerupPickup::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetGameObject(otherObject);
    if (OtherObject->name == "Player")
    {
      PlayerController *playerController = OtherObject->GetComponent<PlayerController>(ePlayerController);
      if (playerController->powerUp != nullptr)
        delete playerController->powerUp;

      playerController->powerUp = (PowerUp *)powerUpType->New();
      playerController->powerUp->Use(OtherObject);
      space->GetGameObject(owner)->Destroy();
    }

	}


}
