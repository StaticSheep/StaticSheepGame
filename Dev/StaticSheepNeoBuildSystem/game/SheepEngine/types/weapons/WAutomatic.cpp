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
  static float weapDelay = 0.050f;
  Automatic::Automatic()
  {
    delay = 0.075f;
    damage = 10;
    knockback = 50;
    semi = false;
    explosive_ = false;
  }

  Automatic::~Automatic()
  {

  }

  void Automatic::Fire(GameObject* player)
  {
    int randomNumber;

    randomNumber = GetRandom(0, 100);

    GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet"));

    Bullet_Default* bd = bullet->GetComponent<Bullet_Default>(eBullet_Default);

    if (explosive_)
    {
      bd->damage = damage + 10;
      bd->explosive_ = true;
    }
    else
      bd->damage = damage;

    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->spawning = false;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timedSpawning = true;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timed = 0.001f;
    Transform *BT = bullet->GetComponent<Transform>(eTransform);
    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    PlayerController* pc = player->GetComponent<PlayerController>(ePlayerController);
    bd->playerOwner = pc->playerNum;
    
    Vec3 AimDir = pc->aimDir;

    float theta = atan2f(AimDir.y, AimDir.x) - (PI / 2.0f);
    Mat3D rotation(theta);

    ParticleSystem* part = bullet->GetComponent<ParticleSystem>(eParticleSystem);
    part->direction.m_startMin = rotation * part->direction.m_startMin;
    part->direction.m_startMax = rotation * part->direction.m_startMax;

    bulletC->SetBodyCollisionGroup(player->GetComponent<PlayerController>(ePlayerController)->weaponGroup);

    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    
    //set the cone of 5 degrees for firing.
    float FireAngle = -5.0f + (randomNumber/100.0f) * 10.0f;
    AimDir = Mat3D((FireAngle * (float)PI) / 180) * AimDir;

    bulletC->AddToVelocity(AimDir * 1000);
    

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("laser1", &SoundInstance(0.25f));
  }

  void Automatic::Update(float dt)
  {
    DelayUpdate(dt);
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