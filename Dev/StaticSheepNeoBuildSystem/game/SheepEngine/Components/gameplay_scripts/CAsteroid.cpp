/*****************************************************************
Filename: CAsteroid.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CAsteroid.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CCircleCollider.h"

namespace Framework
{
  Asteroid::Asteroid()
	{

	}

  Asteroid::~Asteroid()
	{

	}

  void Asteroid::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Asteroid::LogicUpdate));

    aTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    aCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);
	}

  void Asteroid::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void Asteroid::LogicUpdate(float dt)
	{
    Transform *at = space->GetHandles().GetAs<Transform>(aTransfrom);
    CircleCollider *ac = space->GetHandles().GetAs <CircleCollider>(aCollider);

    at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -3.0f));
    ac->AddToAngVelocity(20.0f);
    if (at->GetTranslation().z <= -5.0f)
    {
      space->GetGameObject(owner)->Destroy();
    }
	}


}
