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
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/SheepMath.h"
#include "Matrix3D.h"

namespace Framework
{
  static float weapDelay = 0.09f;
  Automatic::Automatic()
  {
    delay = 0.0f;
    fireVolume = 0.15f;
    damage = 13;
    knockback = 60;
    semi = false;
    explosive_ = false;

    fireSound = "laser1";
  }

  Automatic::~Automatic()
  {

  }

  void Automatic::Fire(GameObject* player)
  {
    int randomNumber;

    randomNumber = GetRandom(0, 100);

    GameObject *bullet = CreateBullet(player, "Bullet");

    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    PlayerController* pc = player->GetComponent<PlayerController>(ePlayerController);
    
    Vec3 AimDir = pc->aimDir;
    
    //set the cone of 5 degrees for firing.
    float FireAngle = -7.0f + (randomNumber/100.0f) * 10.0f;
    AimDir = Mat3D((FireAngle * (float)PI) / 180) * AimDir;

    bulletC->SetVelocity(AimDir * 900);
  }

  void Automatic::Update(float dt)
  {
    DelayUpdate(dt);
  }

  void Automatic::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < 0)
      delay = 0;
  }

  void Automatic::ResetDelay()
  {
    delay = weapDelay;
  }
}