#include "pch/precompiled.h"
#include "WShotgun.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "Matrix3D.h"

namespace Framework
{
  Shotgun::Shotgun()
  {
    delay = 20;
    damage = 20;
  }

  Shotgun::~Shotgun()
  {

  }

  void Shotgun::Fire(GameObject *player)
  {
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 PlayerAimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    Vec3 AimDir = PlayerAimDir;
    Vec3 NegAimDir = PlayerAimDir;
    GameObject *bullet;
    Transform *BT;
    CircleCollider *bulletC;

    for (int i = 0; i < 2; ++i)
    {
      if (i != 0)
      {
        AimDir = Mat3D(PI / 12) * AimDir;
        NegAimDir = (Mat3D(5.7596f)) * AimDir;
      }
      {
        bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet_shot"));
        playerTrans = player->GetComponent <Transform>(eTransform);
        bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
        BT = bullet->GetComponent<Transform>(eTransform);
        bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
        BT->SetTranslation(playerTrans->GetTranslation() + PlayerAimDir * 50);
        bulletC->AddToVelocity(AimDir * 1000);
      }
      if (i != 0)
      {
        bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet_shot"));
        playerTrans = player->GetComponent <Transform>(eTransform);
        bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
        BT = bullet->GetComponent<Transform>(eTransform);
        bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
        BT->SetTranslation(playerTrans->GetTranslation() + PlayerAimDir * 50);
        bulletC->AddToVelocity(NegAimDir * 1000);
      }
    }

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("gunshot", &SoundInstance(0.25f));
  }
}