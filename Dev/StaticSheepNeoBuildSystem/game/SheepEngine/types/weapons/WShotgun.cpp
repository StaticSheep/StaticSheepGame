/*****************************************************************
Filename: WShotgun.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "WShotgun.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "Components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "Matrix3D.h"

namespace Framework
{
  static float weapDelay = 1.0f;
  Shotgun::Shotgun()
  {
    delay = 1.0f;
    damage = 20;
    knockback = 600;
    semi = true;
    explosive_ = false;
  }

  Shotgun::~Shotgun()
  {

  }

  void Shotgun::Fire(GameObject *player)
  {
      //player info
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 PlayerAimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    
      //aim direction
    Vec3 AimDir = PlayerAimDir;
    Vec3 NegAimDir = PlayerAimDir;
    Vec3 bulletDir = AimDir;

      //bullet pointers
    GameObject *bullet;
    Transform *BT;
    CircleCollider *bulletC;

    int dealDamage = damage;
    bool setExplosive = false;
    
    if (explosive_)
    {
      dealDamage += 20;
      setExplosive = true;
    }

    //middle shot - first shot
    bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet_shot"));

    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->spawning = false;
    bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timedSpawning = false;

    //bullet damage and explosive
    bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = dealDamage;
    bullet->GetComponent<Bullet_Default>(eBullet_Default)->explosive_ = setExplosive;

    //bullet fire velocity and collision group
    bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    bulletC->SetBodyCollisionGroup(player->GetComponent<PlayerController>(ePlayerController)->weaponGroup);
    bulletC->AddToVelocity(bulletDir * 1000);

    //bullet offset
    BT = bullet->GetComponent<Transform>(eTransform);
    BT->SetTranslation(playerTrans->GetTranslation() + PlayerAimDir * 25);

      //subsequent shots
    for (int i = 0; i < 4; ++i)
    {
        //modify aim direction for subsequent bullets
      AimDir = Mat3D((float)PI / 40.0f) * AimDir;
      NegAimDir = Mat3D(-(float)PI / 40.0f) * NegAimDir;
      bulletDir = AimDir;

        //create two bullets, one at each angle in opposite directions
      for (int j = 0; j < 2; ++j, bulletDir = NegAimDir)
      {
        //create bullet
        bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet_shot"));

        //particles
        bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->spawning = false;
        bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timedSpawning = true;
        bullet->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter)->timed = 0.001f;

        float theta = atan2f(bulletDir.y, bulletDir.x) - (PI / 2.0f);
        Mat3D rotation(theta);
        ParticleSystem* part = bullet->GetComponent<ParticleSystem>(eParticleSystem);
        part->direction.m_startMin = rotation * part->direction.m_startMin;
        part->direction.m_startMax = rotation * part->direction.m_startMax;

        //bullet damage and explosive
        bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = dealDamage;
        bullet->GetComponent<Bullet_Default>(eBullet_Default)->explosive_ = setExplosive;

        //bullet fire velocity and collision group
        bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
        bulletC->SetBodyCollisionGroup(player->GetComponent<PlayerController>(ePlayerController)->weaponGroup);
        bulletC->AddToVelocity(bulletDir * 1000);

        //bullet offset
        BT = bullet->GetComponent<Transform>(eTransform);
        BT->SetTranslation(playerTrans->GetTranslation() + PlayerAimDir * 25);
      }   
    }

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Shotgun_Shot", &SoundInstance(1.0f));
  }

  void Shotgun::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void Shotgun::ResetDelay()
  {
    delay = weapDelay;
  }
}