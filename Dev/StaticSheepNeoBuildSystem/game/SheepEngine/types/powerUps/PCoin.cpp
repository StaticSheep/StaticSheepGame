/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PCoin.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  Coin::Coin()
  {
    timer = -1;
    uses = 1;
    inUse = true;
  }

  Coin::~Coin()
  {

  }

  void Coin::Use(GameObject *player)
  {
    PlayerController *playerController = player->GetComponent<PlayerController>(ePlayerController);
    int coinAmount = GetRandom(100, 125);

    player->space->hooks.Call("GivePlayerCoins", playerController->playerNum, coinAmount);

    //SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    //se->Play("Laser_Shot", &SoundInstance(1.0f));
  }

  void Coin::Update(float dt)
  {
    if (uses <= 0)
    {
      //turn powerup off
      inUse = false;
      return;
    }
  }

}