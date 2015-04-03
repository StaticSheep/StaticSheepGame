/*****************************************************************
Filename: PShield.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PShield.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  Shield::Shield()
  {
    timer = -1;
    uses = 1;
    inUse = true;
  }

  Shield::~Shield()
  {

  }

  void Shield::Use(GameObject *player)
  {
    PlayerController *playerController = player->GetComponent<PlayerController>(ePlayerController);
    playerController->health += 100;

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Pickup_Shield", &SoundInstance(1.0f));
  }

  void Shield::Update(float dt)
  {
    if (uses <= 0)
    {
      //turn powerup off
      inUse = false;
      return;
    }
  }

}