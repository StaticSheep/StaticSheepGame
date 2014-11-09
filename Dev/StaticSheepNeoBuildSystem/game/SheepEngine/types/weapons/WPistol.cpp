#include "pch/precompiled.h"
#include "WPistol.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"

namespace Framework
{
  Pistol::Pistol()
  {
    delay = 10;
    damage = 10;
  }

  Pistol::~Pistol()
  {

  }

  void Pistol::Fire(GameObject *player)
  {
    
    GameObject *bullet = (FACTORY->LoadObjectFromArchetype(player->space, "Bullet"));
    Transform *BT = bullet->GetComponent<Transform>(eTransform);
    CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    BT->SetTranslation(playerTrans->GetTranslation() + AimDir * 25);
    bulletC->AddToVelocity(AimDir * 1000);

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->PlayEx("gunshot", 0.125f);
  }
}