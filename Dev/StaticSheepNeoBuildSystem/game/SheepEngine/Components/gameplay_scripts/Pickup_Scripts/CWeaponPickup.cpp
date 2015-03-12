/*****************************************************************
Filename: CWeaponPickup.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CWeaponPickup.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "types/weapons/WPistol.h"
#include "../../controllers/player/CPlayerController.h"
#include "../../sprites/CSprite.h"

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
    { eShotgun, "Shotgun" },
    { eWLaser, "WLaser" },
    { eMissile, "Missile" }
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
    else if (weaponNum == 3)
      weaponID = eWLaser;
    else if (weaponNum == 4)
      weaponID = eMissile;

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
    wpSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eBoxCollider))->SetGravityOff();
    timeToLive = 5.0f;
    blink = false;
    respawnTimer = 2.0f;
	}

  void WeaponPickup::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  void WeaponPickup::LogicUpdate(float dt)
	{
    timeToLive -= dt;
    if (timeToLive <= 2.0f)
    {
      //blinking
      RespawnBlink(dt);
    }
    if (timeToLive <= 0)
      space->GetGameObject(owner)->Destroy();

    Transform *bt = space->GetHandles().GetAs<Transform>(wpTransfrom);

    if (bt->GetTranslation().z > 0)
      bt->SetTranslation(bt->GetTranslation() + Vec3(0.0f, 0.0f, -2.0f));
    else
      space->GetGameObject(owner)->GetComponent<BoxCollider>(eBoxCollider)->SetBodyCollisionGroup("Collide");

    if (bt->GetTranslation().x > 1000 || bt->GetTranslation().x < -1000 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
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

      //metric shit
      MetricInfo metricData;
      metricData.mt = WEAP_PICKUP;
      metricData.playerNum = playerController->playerNum;
      metricData.x = 0;
      metricData.y = 0;
      switch (weaponID)
      {
      case ePistol:
        metricData.weapon = PISTOL;
        break;
      case eAutomatic:
        metricData.weapon = AUTO;
        break;
      case eShotgun:
        metricData.weapon = SHOTGUN;
        break;
      case eMissile:
        metricData.weapon = MISSILE;
        break;
      case eWLaser:
        metricData.weapon = LASER;
        break;
      }
      ENGINE->SystemMessage(MetricsMessage(&metricData));
    }

    if (OtherObject->archetype == "KillBox" || 
      OtherObject->archetype == "Grinder" || OtherObject->archetype == "KillBoxBig")
      space->GetGameObject(owner)->Destroy();

	}

  void WeaponPickup::RespawnBlink(float dt)
  {
    Sprite *ps = space->GetHandles().GetAs<Sprite>(wpSprite);

    if (respawnTimer > 0.0f)
    {
      if (!blink)
        ps->Color.A -= dt * 10.0f;
      else
        ps->Color.A += dt * 10.0f;

      respawnTimer -= dt;

      if (ps->Color.A <= 0.0f)
        blink = true;

      if (ps->Color.A >= 1.0f)
        blink = false;
    }
    else
    {
      ps->Color.A = 255.0f;
      respawnTimer = 2.0f;
    }
  }
}
