/*****************************************************************
Filename: CSaucerMissile.cpp
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CSaucerMissile.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"
#include "systems/audio/SheepSoundEvent.h"
#include "../../sprites/CSprite.h"

namespace Framework
{
  SaucerMissile::SaucerMissile()
  {

  }

  SaucerMissile::~SaucerMissile()
  {

  }

  void SaucerMissile::LogicUpdate(float dt)
  {
    Transform *at = space->GetHandles().GetAs<Transform>(mTransform);
    CircleCollider *ac = space->GetHandles().GetAs <CircleCollider>(mCollider);

    if (at->GetTranslation().z < 50.0f)
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -1.0f));
    else
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -1.5f));

    float currZ = at->GetTranslation().z;

    Vec3 scale(0.0f, 0.0f, 0.0f);

    if (currZ >= 0)
      scale = Vec3(1, 1, 1);
    else
      scale = Vec3(10 / (currZ), 10 / (currZ), 1.0f);

    at->SetScale(scale);

    if (currZ > 5.0f)
      ac->SetBodyCollisionGroup("NonCollide");
    else
      ac->SetBodyCollisionGroup("Resolve");

    if (at->GetTranslation().z <= 0.0f)
    {
      Handle explosion = (FACTORY->LoadObjectFromArchetype(space, "explosion"))->self;
      Transform *exT = space->GetGameObject(explosion)->GetComponent<Transform>(eTransform);
      exT->SetTranslation(at->GetTranslation());
      space->GetGameObject(owner)->Destroy();
    }
  }

  void SaucerMissile::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SaucerMissile::LogicUpdate));

    mTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);

    mCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);
  }

  void SaucerMissile::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}