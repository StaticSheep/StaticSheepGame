/*****************************************************************
Filename: CExplosion.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CExplosion.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"
#include "../../particles/CParticleCircleEmitter.h"

namespace Framework
{
  Explosion::Explosion()
	{
   
	}

  Explosion::~Explosion()
	{

	}

  void Explosion::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Explosion::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("AnimEnd", self,
      BUILD_FUNCTION(Explosion::AnimEnd));

    eTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    eAnSprite = space->GetGameObject(owner)->GetComponentHandle(eAniSprite);
	}

  void Explosion::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("AnimEnd", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void Explosion::LogicUpdate(float dt)
	{
    if (waitForAnim)
      return;

    timer -= dt;

    if (timer <= 0.0f)
    {
      if (removal)
        DestroySelf();
      else
      {
        ParticleCircleEmitter* psc = (ParticleCircleEmitter*)space->GetComponent
          (eParticleCircleEmitter, owner);

        if (psc)
          psc->Toggle(false);


        removal = true;
        timer = 2.0f;
      }
    }

    //Transform *pt = space->GetHandles().GetAs<Transform>(eTransfrom);
    //AniSprite *ea = space->GetHandles().GetAs <AniSprite>(eAnSprite);

	}

  void Explosion::AnimEnd()
  {
    AniSprite *ea = space->GetHandles().GetAs <AniSprite>(eAnSprite);
    ea->m_hidden = true;

    ParticleCircleEmitter* psc = (ParticleCircleEmitter*)space->GetComponent
      (eParticleCircleEmitter, owner);

    if (psc)
      psc->Toggle(false);

    

    timer = 2.0f;
    waitForAnim = false;
    removal = true;
  }

  void Explosion::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }
}
