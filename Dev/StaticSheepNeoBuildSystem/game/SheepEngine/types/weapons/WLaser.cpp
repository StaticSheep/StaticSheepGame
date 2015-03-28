#include "pch/precompiled.h"
#include "WLaser.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CBoxCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "Matrix3D.h"


namespace Framework
{
  static float weaponDelay = 0.050f;
  static float lifeTime = 0.3f;

  WLaser::WLaser()
  {
    delay = 0.0f;
    damage = 1;
    knockback = 0;
    life = 0;
    semi = false;
  }

  WLaser::~WLaser()
  {

  }

  void WLaser::Fire(GameObject *player)
  {
    life = lifeTime;

    Transform *playerTrans = player->GetComponent <Transform>(eTransform);
    Vec3 AimDir = player->GetComponent<PlayerController>(ePlayerController)->aimDir;

    


    Handle playerCollider = player->GetComponentHandle(eBoxCollider);
    BoxCollider *bc = player->space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetRayCast(playerTrans->GetTranslation(), AimDir, player->archetype);
    //bool death = bc->ComplexRayCast();
    bool death = bc->ComplexRayCast();
    if (death)
      bc->RayDestruction();
    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void WLaser::Update(float dt)
  {
    DelayUpdate(dt);

    if (life < 0)
    {

    }
    else
      life -= dt;
  }

  void WLaser::DelayUpdate(float dt)
  {
    delay -= dt;
    if (delay < -100)
      delay = 0;
  }

  void WLaser::ResetDelay()
  {
    delay = 1;
  }
}