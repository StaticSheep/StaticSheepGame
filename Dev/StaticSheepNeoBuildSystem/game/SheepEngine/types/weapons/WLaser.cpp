#include "pch/precompiled.h"
#include "WLaser.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CBoxCollider.h"
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
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    
    Handle playerCollider = player->GetComponentHandle(eBoxCollider);
    BoxCollider *bc = player->space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetRayCast(playerTrans->GetTranslation(), AimDir, player->archetype);
    bool death = bc->ComplexRayCast();
    if (death)
      bc->RayDestruction();
    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void Laser::DelayUpdate(float dt)
  {

  }

  void Laser::ResetDelay()
  {
    delay = 0;
  }
}