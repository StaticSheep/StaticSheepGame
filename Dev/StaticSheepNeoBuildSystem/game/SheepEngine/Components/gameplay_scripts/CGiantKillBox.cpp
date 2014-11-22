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
    GrindSpawn = false;
	}

  void GiantKillBox::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);

	}

  void GiantKillBox::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(kbTransfrom);
    BoxCollider *pc = space->GetHandles().GetAs <BoxCollider>(kbCollider);
    if (pt->GetTranslation().x > 1010 || pt->GetTranslation().x < -1010)
    {
      for (int i = 7; i < 7; ++i)
        space->GetGameObject(Grinders[i])->Destroy();

      GrindSpawn = false;
      space->GetGameObject(owner)->Destroy();
    }
    if (!GrindSpawn)
    {
      for (int i = 0; i < 7; ++i)
      {
        Grinders[i] = (FACTORY->LoadObjectFromArchetype(space, "GrinderBig"))->self;
      }
      GrindSpawn = true;
    }
    if (direction)
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(-3.0, 0.0, 0.0));
      for (int i = 0; i < 7; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0, ((float)i * 64.0f -192.0f), 0.0));
      }
    }
    else
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(3.0, 0.0, 0.0));
      for (int i = 0; i < 7; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0, ((float)i * 64.0f - 192.0f), 0.0));
      }
    }
	}


}
