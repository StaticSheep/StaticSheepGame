/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PDamage.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  DamageBoost::DamageBoost()
  {
    timer = 5.0f;
    uses = -1;
  }

  DamageBoost::~DamageBoost()
  {

  }

  void DamageBoost::Use(GameObject *player)
  {
    PlayerController *playerController = player->GetComponent<PlayerController>(ePlayerController);
    playerController->weapon->damage *= 2;

    //SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    //se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void DamageBoost::Update(float dt)
  {
    if (timer <= 0)
    {
      //turn powerup off
      inUse = false;
      return;
    }
    timer -= dt;
  }

}