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
  Automatic::Automatic()
  {
    delay = 5;
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
    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 45);
    
    //set the cone of 5 degrees for firing.
    float FireAngle = -5.0f + (randomNumber/100.0f) * 10.0f;
    AimDir = Mat3D((FireAngle * PI) / 180) * AimDir;

    bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("laser1", &SoundInstance(0.25f));
  }
}