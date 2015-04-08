/*****************************************************************
Filename: WMissile.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
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
  static const float fireDelay = 0.8f;

  Missile::Missile()
  {
    delay = 0.0f;
    damage = 150;
    knockback = 800;
    semi = true;
    explosive_ = false;

    fireSound = "weapon_rocket";
  }

  Missile::~Missile()
  {

  }

  void Missile::Fire(GameObject *player)
  {

    GameObject *bullet = CreateBullet(player, "Missile", 850.0f);
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