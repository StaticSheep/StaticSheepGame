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

    gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
	}

  void Grinder::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void Grinder::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(gTransfrom);
    BoxCollider *pc = space->GetHandles().GetAs <BoxCollider>(gCollider);

    //pc->AddToAngVelocity(100);
    pt->SetRotation(pt->GetRotation() + (dt * 10));

	}


}
