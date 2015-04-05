/*****************************************************************
Filename: WPistol.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "PDamage.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "../SheepUtil/include/Matrix3D.h"
#include "Components/gameplay_scripts/FX_Scripts/CFollowTarget.h"

namespace Framework
{
  DamageBoost::DamageBoost()
  {
    timer = 5.0f;
    uses = -1;
    inUse = true;
  }

  DamageBoost::~DamageBoost()
  {

  }

  void DamageBoost::Use(GameObject *player)
  {
    PlayerController *pc = player->GetComponent<PlayerController>(ePlayerController);
    pc->Combat()->GiveDoubleDamage();

    SoundEmitter *se = player->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("Pickup_Damage", &SoundInstance(1.0f));

    (FACTORY->LoadObjectFromArchetype(player->space, "powerFX_DD"))->GetComponent<FollowTarget>(eFollowTarget)->pTransform =
      player->GetComponentHandle(eTransform);
  }

  void DamageBoost::Remove(GameObject *player)
  {

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