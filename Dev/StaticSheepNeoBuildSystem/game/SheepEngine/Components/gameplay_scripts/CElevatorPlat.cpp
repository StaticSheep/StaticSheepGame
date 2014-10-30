#include "pch/precompiled.h"
#include "CElevatorPlat.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"

namespace Framework
{
  ElevatorPlat::ElevatorPlat()
	{
    direction = true;
	}

  ElevatorPlat::~ElevatorPlat()
	{

	}

  void ElevatorPlat::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ElevatorPlat::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(ElevatorPlat::OnCollision));

    pTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
	}

  void ElevatorPlat::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void ElevatorPlat::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(pTransfrom);

    if (pt->GetTranslation().y > 500 || pt->GetTranslation().y < -500)
      space->GetGameObject(owner)->Destroy();

    if (direction)
      pt->SetTranslation(pt->GetTranslation() + Vec3(0.0, 1.0, 0.0));
    else
      pt->SetTranslation(pt->GetTranslation() + Vec3(0.0, -1.0, 0.0));

	}

  void ElevatorPlat::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
		//space->GetGameObject(owner)->Destroy();
	}


}
