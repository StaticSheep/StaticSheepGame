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
#include "../../basicps/CBasicPSystem.h"
#include "../../lights/CPointLight.h"

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

  void Explosion::PrepRemoval()
  {
    ParticleCircleEmitter* psc = (ParticleCircleEmitter*)space->GetComponent
      (eParticleCircleEmitter, owner);

    if (psc)
      psc->Toggle(false);

    BasicParticleSystem* bsc = (BasicParticleSystem*)space->GetComponent
      (eBasicParticleSystem, owner);

    if (bsc)
      bsc->Toggle(false);

    
    AniSprite* ans = (AniSprite*)space->GetComponent(eAniSprite, owner);

    if (ans)
      ans->Color.a = 0;


    removal = true;
    waitForAnim = false;
    timer = lightFadeTime + 0.02f;

    PointLight* pl = (PointLight*)space->GetComponent
      (ePointLight, owner);

    if (pl)
      lightDropStep = pl->m_brightness.a / lightFadeTime;
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
        PrepRemoval();
      }
    }
    else
    {
      PointLight* pl = (PointLight*)space->GetComponent
        (ePointLight, owner);

      if (pl)
      {
        pl->m_brightness.a -= lightDropStep * dt;
        if (pl->m_brightness.a < 0.0f)
          pl->m_brightness.a = 0;
      }
        
    }

    //Transform *pt = space->GetHandles().GetAs<Transform>(eTransfrom);
    //AniSprite *ea = space->GetHandles().GetAs <AniSprite>(eAnSprite);

	}

  void Explosion::AnimEnd()
  {
    PrepRemoval();
  }

  void Explosion::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }
}
