#include "pch/precompiled.h"
#include "CGiantKillBox.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"

namespace Framework
{
  GiantKillBox::GiantKillBox()
	{
    direction = true;
	}

  GiantKillBox::~GiantKillBox()
	{

	}

  void GiantKillBox::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(GiantKillBox::LogicUpdate));

    kbTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    kbCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
	}

  void GiantKillBox::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void GiantKillBox::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(kbTransfrom);
    BoxCollider *pc = space->GetHandles().GetAs <BoxCollider>(kbCollider);
    if (pt->GetTranslation().x > 500 || pt->GetTranslation().x < -500)
      space->GetGameObject(owner)->Destroy();

    if (direction)
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(-1.0, 0.0, 0.0));
    }
    else
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(1.0, 0.0, 0.0));
    }
	}


}
