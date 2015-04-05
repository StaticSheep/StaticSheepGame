/*****************************************************************
Filename: CFollowTarget.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CFollowTarget.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../particles/CParticleCircleEmitter.h"
#include "../../basicps/CBasicPSystem.h"

namespace Framework
{
  FollowTarget::FollowTarget()
	{
    lifeTime = 5.0f;
    alive = true;
    deathTimer = 0.15f; //default values
    timedLifetime = true;
	}

  FollowTarget::~FollowTarget()
	{

	}

  void FollowTarget::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(FollowTarget::LogicUpdate));

    effectTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
	}

  void FollowTarget::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  static Transform *et, *pt;

  void FollowTarget::LogicUpdate(float dt)
	{
    if (!alive)
    {
      Handle peHandle = space->GetGameObject(owner)->GetComponentHandle(eBasicParticleSystem);
      BasicParticleSystem *pe = space->GetHandles().GetAs<BasicParticleSystem>(peHandle);
      pe->m_system.active = false;

      deathTimer -= dt;
      if (deathTimer <= 0)
        space->GetGameObject(owner)->Destroy();
      return;
    }
    if (timedLifetime)
    {
      lifeTime -= dt;
      if (lifeTime <= 0)
        alive = false;
    }
    pt = space->GetHandles().GetAs<Transform>(pTransform);

    et = space->GetHandles().GetAs<Transform>(effectTransform);

    if(pt != nullptr)
      et->SetTranslation(pt->GetTranslation());

    if (pt == nullptr)
      alive = false;
	}


}
