/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PMiniMe.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  MiniMe::MiniMe()
  {
    timer = 5.0f;
    uses = -1;
    inUse = true;
  }

  MiniMe::~MiniMe()
  {

  }

  void MiniMe::Use(GameObject *player)
  {
    //PlayerController *playerController = player->GetComponent<PlayerController>(ePlayerController);
    //playerController->weapon->damage *= 2;

    //SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    //se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void MiniMe::Update(float dt)
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