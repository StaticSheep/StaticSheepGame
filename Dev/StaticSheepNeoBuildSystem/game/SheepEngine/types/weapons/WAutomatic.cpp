/*****************************************************************
Filename: WAutomatic.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "WAutomatic.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "../SheepUtil/include/SheepMath.h"
#include "Matrix3D.h"

namespace Framework
{
  static float weapDelay = 0.075;
  Automatic::Automatic()
  {
    delay = 0.075f;
    damage = 10;
    knockback = 50;
    semi = false;
    
  }

  Automatic::~Automatic()
  {

  }

  void Automatic::Fire(GameObject* player)
  {
    int randomNumber;

    randomNumber = GetRandom(0, 100);

    GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet"));
    bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
    Transform *BT = bullet->GetComponent<Transform>(eTransform);
    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    bulletC->SetBodyCollisionGroup(player->archetype);
    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    
    //set the cone of 5 degrees for firing.
    float FireAngle = -5.0f + (randomNumber/100.0f) * 10.0f;
    AimDir = Mat3D((FireAngle * (float)PI) / 180) * AimDir;

    bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("laser1", &SoundInstance(0.25f));
  }

  void Automatic::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void Automatic::ResetDelay()
  {
    delay = weapDelay;
  }
}