#include "pch/precompiled.h"
#include "CExplosion.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../sprites/CAniSprite.h"

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
    space->GetGameObject(owner)->hooks.Add("AnimEnd", self, BUILD_FUNCTION(Explosion::DestroySelf));

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
    Transform *pt = space->GetHandles().GetAs<Transform>(eTransfrom);
    AniSprite *ea = space->GetHandles().GetAs <AniSprite>(eAnSprite);

	}

  void Explosion::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }
}