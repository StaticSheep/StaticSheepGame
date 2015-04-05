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
  static float weapDelay = 0.85f;

  Shotgun::Shotgun()
  {
    delay = 0.0f;
    damage = 18;
    knockback = 600;
    semi = true;
    explosive_ = false;

    fireSound = "";
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
    Bullet_Default* bd;

    int dealDamage = damage;
    bool setExplosive = false;
    
    if (explosive_)
    {
      dealDamage += 20;
      setExplosive = true;
    }

    //middle shot - first shot
    bullet = CreateBullet(player, "Bullet_shot");

    float centerTheta = atan2f(AimDir.y, AimDir.x) - (PI / 2.0f);

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
        bullet = CreateBullet(player, "Bullet_shot");

        float theta = atan2f(bulletDir.y, bulletDir.x) - (PI / 2.0f);
        Mat3D rotation(centerTheta - theta);
        ParticleSystem* part = bullet->GetComponent<ParticleSystem>(eParticleSystem);
        part->direction.m_startMin = rotation * part->direction.m_startMin;
        part->direction.m_startMax = rotation * part->direction.m_startMax;

        //bullet fire velocity and collision group
        bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
        bulletC->SetVelocity(bulletDir * 1000);
      }   
    }

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Shotgun_Shot", &SoundInstance(0.8f));
  }

  void Shotgun::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void Shotgun::Update(float dt)
  {
    DelayUpdate(dt);
  }

  void Shotgun::ResetDelay()
  {
    delay = weapDelay;
  }
}