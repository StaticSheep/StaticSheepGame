/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "WPistol.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "Components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  Pistol::Pistol()
  {
    delay = 0;
    damage = 10;
    knockback = 60;
    semi = true;
    explosive_ = false;
    fireSound = "Laser_Shot";
  }

  Pistol::~Pistol()
  {

  }

  void Pistol::Fire(GameObject *player)
  {
    GameObject *bullet = CreateBullet(player, "Bullet");
  }

  void Pistol::Update(float dt)
  {

  }

  void Pistol::DelayUpdate(float dt)
  {

  }

  void Pistol::ResetDelay()
  {
    delay = 0;
  }
}