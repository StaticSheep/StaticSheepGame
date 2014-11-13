#include "pch/precompiled.h"
#include "WPistol.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"

namespace Framework
{
  Pistol::Pistol()
  {
    delay = 10;
    damage = 10;
    semi = true;
  }

  Pistol::~Pistol()
  {

  }

  void Pistol::Fire(GameObject *player)
  {
    
    GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet"));
    bullet->GetComponent<Bullet_Default>(eBullet_Default)->damage = damage;
    Transform *BT = bullet->GetComponent<Transform>(eTransform);
    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("gunshot", &SoundInstance(1.0f));
  }
}