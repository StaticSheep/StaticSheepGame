/******************************************************************************
Filename: WLaser.cpp
Project:  GAM 250
Author(s): Zachary Nawar (primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "WLaser.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CBoxCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "Matrix3D.h"
#include "systems/physics/MultiRay.h"


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
    PlayerController* fpc = player->GetComponent<PlayerController>(ePlayerController);
    Vec3 AimDir = fpc->aimDir;

    Vec3 AimEnd = playerTrans->GetTranslation() + AimDir * 40.0f;
    Vec2 testStart = Draw::ToScreen(playerTrans->GetTranslation());
    Vec2 testEnd = Draw::ToScreen(AimEnd);

    Draw::DrawLine(testStart.x, testStart.y, testEnd.x, testEnd.y);

    std::vector<MCData> rayData;

    MultiRayCaster(playerTrans->GetTranslation(), AimDir,
      player->space, 20.0f, Player1Weapon, rayData, 4);

    if (rayData[0].obj != Handle::null)
    {
      GameObject* obj = player->space->GetGameObject(rayData[0].obj);

      if (obj)
      {
        PlayerController* pc = obj->GetComponent<PlayerController>
          (ePlayerController);

        if (pc)
        {
          // Kill me
          pc->Combat()->TakeDamage(500.0f, fpc->playerNum);
        }
      }
    }
    


    
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
    delay = 0;
  }
}