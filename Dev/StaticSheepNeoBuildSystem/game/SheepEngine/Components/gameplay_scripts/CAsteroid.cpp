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
    if (at->GetTranslation().z < 50.0f)
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -1.5f));
    else
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -2.0f));

    float currZ = at->GetTranslation().z;

    Vec3 scale(0.0f, 0.0f, 0.0f);
    if (currZ <= 5.0f)
      scale = Vec3(0.75f, 0.75f, 0.75f);
    else
      scale = Vec3(1/(currZ * 0.10f), 1/(currZ * 0.10f), 1.0f);

    at->SetScale(scale);

    if (currZ > 5.0f)
      ac->SetBodyCollisionGroup("NonCollide");
    else
      ac->SetBodyCollisionGroup("Resolve");

    if (at->GetTranslation().z <= 0.0f)
    {
      space->GetGameObject(owner)->Destroy();
    }
	}


}
