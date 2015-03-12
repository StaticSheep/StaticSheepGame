/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "WMissile.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  Missile::Missile()
  {
    delay = 1.0f;
    damage = 90;
    knockback = 600;
    semi = true;
    explosive_ = true;
  }

  Missile::~Missile()
  {

  }

  void Missile::Fire(GameObject *player)
  {

    GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Missile"));
    if (explosive_)
    {
      bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage + 10;
      bullet->GetComponent<Bullet_Default>(eBullet_Default)->explosive_ = true;
    }
    else
      bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->spawning = false;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timedSpawning = true;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timed = 0.001f;
    Transform *BT = bullet->GetComponent<Transform>(eTransform);
    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;

    float theta = atan2f(AimDir.y, AimDir.x) - (PI / 2.0f);

    Mat3D rotation(theta);

    ParticleSystem* part = bullet->GetComponent<ParticleSystem>(eParticleSystem);
    part->direction.m_startMin = rotation * part->direction.m_startMin;
    part->direction.m_startMax = rotation * part->direction.m_startMax;

    bulletC->SetBodyCollisionGroup(player->archetype);
    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("dash", &SoundInstance(1.0f));
  }

  void Missile::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void Missile::ResetDelay()
  {
    delay = 1.0f;
  }
}