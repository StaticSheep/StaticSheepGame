/*****************************************************************
Filename: CGrinder.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CGrinder.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"

namespace Framework
{
  Grinder::Grinder()
	{
   
	}

  Grinder::~Grinder()
	{

	}

  void Grinder::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Grinder::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(Grinder::OnCollision));

    gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
	}

  void Grinder::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  static Transform *pt;
  static BoxCollider *pc;

  void Grinder::LogicUpdate(float dt)
	{
    pt = space->GetHandles().GetAs<Transform>(gTransfrom);
    pc = space->GetHandles().GetAs <BoxCollider>(gCollider);

    pt->SetRotation(pt->GetRotation() + (dt * 10));
    if (pt->GetRotation() > 2 * PI)
      pt->SetRotation(0);

    if (pt->GetTranslation().x > 1010 || pt->GetTranslation().x < -1010 || pt->GetTranslation().y > 600 || pt->GetTranslation().y < -600)
      space->GetGameObject(owner)->Destroy();

	}

  void Grinder::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
  {
    pt = space->GetHandles().GetAs<Transform>(gTransfrom);
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    if (OtherObject->name == "Player")
    {
      Handle pCollider = space->GetGameObject(otherObject)->GetComponentHandle(eBoxCollider);
      BoxCollider *playT = space->GetHandles().GetAs<BoxCollider>(pCollider);
      float xDir = (float)GetRandom(-500, 500);
      if (pt->GetTranslation().y <= 0)
      {
        playT->SetVelocity(playT->GetCurrentVelocity() + Vec3(xDir, 500.0f, 0.0f));
      }
      else
      {
        playT->SetVelocity(playT->GetCurrentVelocity() + Vec3(xDir, -500.0f, 0.0f));
      }
    }
  }

}
