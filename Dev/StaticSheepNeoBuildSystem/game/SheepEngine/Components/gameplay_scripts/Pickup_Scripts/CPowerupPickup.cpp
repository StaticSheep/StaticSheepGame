/*****************************************************************
Filename: CPowerupPickup.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CPowerupPickup.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../controllers/player/CPlayerController.h"
#include "../../colliders/CCircleCollider.h"
#include "../../sprites/CSprite.h"
#include "../../sprites/CAniSprite.h"

namespace Framework
{
  static std::unordered_map<EPowerUps, const char*> PowerUpMap =
  {
    { eShield, "Shield" },
    { ePower, "DamageBoost" },
    { eCoin, "Coin" },
    { eExplosive, "Explosive" },
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
    else if (powerNum == 2)
      powerUpID = eCoin;
    else if (powerNum == 3)
      powerUpID = eExplosive;

    if (powerUpID == eNoPowerUp)
    {
      int default_ = 2;
      powerUpID = (EPowerUps)default_;
      powerUpType = GET_STR_TYPE(PowerUpMap[powerUpID]);
    }
    else
    {
      powerUpType = GET_STR_TYPE(PowerUpMap[powerUpID]);
    }
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PowerupPickup::LogicUpdate));
    space->hooks.Add("DestroyPickups", self, BUILD_FUNCTION(PowerupPickup::DestroySelf));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PowerupPickup::OnCollision));

    puTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    if (powerUpID == eCoin)
    {
      float ranXvel = (float)GetRandom(-500, 500);
      float ranYvel = (float)GetRandom(-500, 500);
      space->GetHandles().GetAs<CircleCollider>(space->GetGameObject(owner)->GetComponentHandle(eCircleCollider))->SetVelocity(Vec3(ranXvel, ranYvel, 0.0f));
      space->GetHandles().GetAs<CircleCollider>(space->GetGameObject(owner)->GetComponentHandle(eCircleCollider))->SetGravityOff();
    }
    else
      space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eBoxCollider))->SetGravityOff();

    if (space->GetGameObject(owner)->name == "CoinPickup")
      puSprite = space->GetGameObject(owner)->GetComponentHandle(eAniSprite);
    else
      puSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

    timeToLive = 5.0f;
    blink = false;
    respawnTimer = 2.0f;

	}

  void PowerupPickup::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("DestroyPickups", self);

	}

  void PowerupPickup::LogicUpdate(float dt)
	{
    timeToLive -= dt;
    if (timeToLive <= 2.0f)
    {
      //blinking
      RespawnBlink(dt);
    }
    if (timeToLive <= 0)
      space->GetGameObject(owner)->Destroy();

    Transform *bt = space->GetHandles().GetAs<Transform>(puTransfrom);

    if (bt->GetTranslation().z > 0)
    {
      bt->SetTranslation(bt->GetTranslation() + Vec3(0.0f, 0.0f, -2.0f));
    }
    else
    {
      if (powerUpID == eCoin)
        space->GetGameObject(owner)->GetComponent<CircleCollider>(eCircleCollider)
        ->SetBodyCollisionGroup("Item");
      else
        space->GetGameObject(owner)->GetComponent<BoxCollider>(eBoxCollider)
        ->SetBodyCollisionGroup("Collide");
    }

    if (bt->GetTranslation().x > 1000 || bt->GetTranslation().x < -1000 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
      space->GetGameObject(owner)->Destroy();
	}

  void PowerupPickup::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetGameObject(otherObject);

    PlayerController *playerController = 
      OtherObject->GetComponent<PlayerController>(ePlayerController);

    

    if (playerController)
    {
      if (playerController->powerUp != nullptr)
        delete playerController->powerUp;

      playerController->powerUp = (PowerUp *)powerUpType->New();
      playerController->powerUp->Use(OtherObject);
      space->GetGameObject(owner)->Destroy();
    }

	}

  void PowerupPickup::RespawnBlink(float dt)
  {
    Sprite *ps = space->GetHandles().GetAs<AniSprite>(puSprite);

    if (!ps)
      ps = space->GetHandles().GetAs<Sprite>(puSprite);

    if (!ps)
      return;

    if (respawnTimer > 0.0f)
    {
      if (!blink)
        ps->Color.A -= dt * 1.0f;
      else
        ps->Color.A += dt * 1.0f;

      respawnTimer -= dt;

      if (ps->Color.A <= 0.7f)
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

  void PowerupPickup::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }
}
