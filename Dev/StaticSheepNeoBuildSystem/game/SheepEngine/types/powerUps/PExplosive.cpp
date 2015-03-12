/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PExplosive.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  Explosive::Explosive()
  {
    timer = -1;
    uses = 1;
    inUse = true;
  }

  Explosive::~Explosive()
  {

  }

  void Explosive::Use(GameObject *player)
  {
    PlayerController *playerController = player->GetComponent<PlayerController>(ePlayerController);
    playerController->weapon->explosive_ = true;
    uses -= 1;

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Pickup_Explosive", &SoundInstance(1.0f));
  }

  void Explosive::Update(float dt)
  {
    if (uses <= 0)
    {
      //turn powerup off
      inUse = false;
      return;
    }
  }

}