/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "WMissile.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "Components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"
#include "Components/controllers/player/CPlayerController.h"

namespace Framework
{
  static const float fireDelay = 1.0f;

  Missile::Missile()
  {
    delay = 0.0f;
    damage = 90;
    knockback = 600;
    semi = true;
    explosive_ = false;

    fireSound = "weapon_rocket";
  }

  Missile::~Missile()
  {

  }

  void Missile::Fire(GameObject *player)
  {

    GameObject *bullet = CreateBullet(player, "Missile", 800.0f);
  }

  void Missile::Update(float dt)
  {
    DelayUpdate(dt);
  }

  void Missile::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < 0)
      delay = 0;
  }

  void Missile::ResetDelay()
  {
    delay = fireDelay;
  }
}