/*****************************************************************
Filename: PExplosive.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PExplosive.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"
#include "Components/gameplay_scripts/FX_Scripts/CFollowTarget.h"

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
    PlayerController *pc = player->GetComponent<PlayerController>(ePlayerController);
    pc->Combat()->GiveExplosiveBullets();
    uses -= 1;

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Pickup_Explosive", &SoundInstance(1.0f));

    //(FACTORY->LoadObjectFromArchetype(player->space, "powerFX_explosive"))->GetComponent<FollowTarget>(eFollowTarget)->pTransform =
    //  player->GetComponentHandle(eTransform);
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