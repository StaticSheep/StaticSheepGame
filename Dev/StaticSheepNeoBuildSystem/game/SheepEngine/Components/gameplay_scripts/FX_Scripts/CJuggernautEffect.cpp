/*****************************************************************
Filename: CGrinder.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CJuggernautEffect.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../particles/CParticleCircleEmitter.h"

namespace Framework
{
  JuggernautEffect::JuggernautEffect()
	{
    lifeTime = 0.0f;
    alive = true;
    deathTimer = 0.1f;
	}

  JuggernautEffect::~JuggernautEffect()
	{

	}

  void JuggernautEffect::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(JuggernautEffect::LogicUpdate));

    effectTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
	}

  void JuggernautEffect::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  static Transform *et, *pt;

  void JuggernautEffect::LogicUpdate(float dt)
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

    if (pt == nullptr)
      alive = false;
	}


}
