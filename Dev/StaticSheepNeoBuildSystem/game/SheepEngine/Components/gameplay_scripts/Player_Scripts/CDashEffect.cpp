/*****************************************************************
Filename: CGrinder.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CDashEffect.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../particles/CParticleCircleEmitter.h"

namespace Framework
{
  DashEffect::DashEffect()
	{
    lifeTime = 0.0f;
    alive = true;
    deathTimer = 0.5f;
	}

  DashEffect::~DashEffect()
	{

	}

  void DashEffect::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(DashEffect::LogicUpdate));

    effectTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
	}

  void DashEffect::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  static Transform *et, *pt;

  void DashEffect::LogicUpdate(float dt)
	{
    if (!alive)
    {
      Handle peHandle = space->GetGameObject(owner)->GetComponentHandle(eParticleCircleEmitter);
      ParticleCircleEmitter *pe = space->GetHandles().GetAs<ParticleCircleEmitter>(peHandle);
      pe->spawning = false;
      deathTimer -= dt;
      if (deathTimer <= 0)
        space->GetGameObject(owner)->Destroy();
      return;
    }
    pt = space->GetHandles().GetAs<Transform>(pTransform);

    et = space->GetHandles().GetAs<Transform>(effectTransform);

    if(pt != nullptr)
      et->SetTranslation(pt->GetTranslation());

    lifeTime += dt;

    if (lifeTime >= 0.5f || pt == nullptr)
      alive = false;
	}


}
