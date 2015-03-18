/*****************************************************************
Filename: CAsteroid.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CAsteroid.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"
#include "systems/audio/SheepSoundEvent.h"
#include "../../sprites/CSprite.h"

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
    //space->hooks.Add("CallingSM", self, BUILD_FUNCTION(Asteroid::CallingSM));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(Asteroid::DestroySelf));

    aTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    aCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);
    warn_circle_handle = (FACTORY->LoadObjectFromArchetype(space, "red_circle_warning"))->self;
    warn_circle = space->GetGameObject(warn_circle_handle);
	}

  void Asteroid::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("RoundOver", self);
	}

  void Asteroid::LogicUpdate(float dt)
	{
    Transform *at = space->GetHandles().GetAs<Transform>(aTransfrom);
    CircleCollider *ac = space->GetHandles().GetAs <CircleCollider>(aCollider);
    warn_circle = space->GetGameObject(warn_circle_handle);
    warn_circle->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(at->GetTranslation().x, at->GetTranslation().y, 0.0f));
    //warn_circle->GetComponent<Transform>(eTransform)->SetScale(at->GetScale());

    if (at->GetTranslation().z < 50.0f)
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -1.0f));
    else
      at->SetTranslation(at->GetTranslation() + Vec3(0.0f, 0.0f, -1.5f));

    float currZ = at->GetTranslation().z;

    Vec3 scale(0.0f, 0.0f, 0.0f);
    if (currZ <= 5.0f)
      scale = Vec3(2.5f, 2.5f, 2.0f);
    else
      scale = Vec3(10/(currZ), 10/(currZ), 1.0f);

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
      if (!GetRandom(0, 2))
        space->hooks.Call("SpawnItemSet", at->GetTranslation());
      space->GetGameObject(warn_circle_handle)->Destroy();
      space->GetGameObject(owner)->Destroy();
    }
	}

  void Asteroid::CallingSM()
  {
    /*if (owner != Handle::null)
      space->GetGameObject(owner)->Destroy();*/
  }

  void Asteroid::DestroySelf()
  {
    space->GetGameObject(warn_circle_handle)->Destroy();
    space->GetGameObject(owner)->Destroy();
  }
}
