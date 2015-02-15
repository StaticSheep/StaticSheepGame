#include "pch/precompiled.h"
#include "WLaser.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CBoxCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "Matrix3D.h"


namespace Framework
{
  Laser::Laser()
  {
    delay = 0.0f;
    damage = 1;
    knockback = 0;
    semi = false;
  }

  Laser::~Laser()
  {

  }

  void Laser::Fire(GameObject *player)
  {
    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;
    
    /*if (AimDir.x < 0.0f)
      arrowTransform->SetRotation((float)atan(aimDir.y / aimDir.x));
    else
      arrowTransform->SetRotation((float)atan(aimDir.y / aimDir.x) + PI);*/


    Handle playerCollider = player->GetComponentHandle(eBoxCollider);
    BoxCollider *bc = player->space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetRayCast(playerTrans->GetTranslation(), AimDir, player->archetype);
    //bool death = bc->ComplexRayCast();
    bool death = bc->SimpleRayCast();
    if (death)
      bc->RayDestruction();
    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void Laser::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void Laser::ResetDelay()
  {
    delay = 0;
  }
}