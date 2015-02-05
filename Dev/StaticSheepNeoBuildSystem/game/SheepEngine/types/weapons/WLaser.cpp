#include "pch/precompiled.h"
#include "WLaser.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"

namespace Framework
{
  Laser::Laser()
  {
    delay = 0;
    damage = 20;
    knockback = 60;
    semi = true;
  }

  Laser::~Laser()
  {

  }

  void Laser::Fire(GameObject *player)
  {

    //GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet"));
    //bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
    //Transform *BT = bullet->GetComponent<Transform>(eTransform);
    //CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    ((RigidBody*)player)->SetRayCast((Vec3D)(playerTrans->GetTranslation()), (Vec3D)AimDir, player->archetype);
    ((RigidBody*)player)->ComplexRayCast();
    //bulletC->SetBodyCollisionGroup(player->archetype);
    //BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    //bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Laser_Shot", &SoundInstance(1.0f));
  }
}