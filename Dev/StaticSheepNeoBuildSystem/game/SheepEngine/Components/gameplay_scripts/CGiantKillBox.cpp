/*****************************************************************
Filename: CExplosion.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CGiantKillBox.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"

namespace Framework
{
  GiantKillBox::GiantKillBox()
	{
    //default
    direction = true;
    numOfGrinders = 11;
    sizeOfGrinder = 60.0f;
	}

  GiantKillBox::~GiantKillBox()
	{

	}

  void GiantKillBox::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(GiantKillBox::LogicUpdate));
    space->hooks.Add("CallingSM", self, BUILD_FUNCTION(GiantKillBox::CallingSM));

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

    //spawn grinders
    if (!GrindSpawn)
    {
      for (int i = 0; i < numOfGrinders; ++i)
      {
        Grinders[i] = (FACTORY->LoadObjectFromArchetype(space, "GrinderBig"))->self;
      }
      GrindSpawn = true;
    }
    //move the level event
    if (direction)
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(-5.0, 0.0, 0.0));
      for (int i = 0; i < numOfGrinders; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0, ((float)i * sizeOfGrinder - (sizeOfGrinder * (numOfGrinders / 2))), -0.1));
      }
    }
    else
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(5.0, 0.0, 0.0));
      for (int i = 0; i < numOfGrinders; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0, ((float)i * sizeOfGrinder - (sizeOfGrinder * (numOfGrinders / 2))), -0.1));
      }
    }

    //kill grinders that are out of bounds
    if (pt->GetTranslation().x > 1008 || pt->GetTranslation().x < -1008)
    {
      for (int i = 0; i < numOfGrinders; ++i)
        space->GetGameObject(Grinders[i])->Destroy();

      GrindSpawn = false;
      space->GetGameObject(owner)->Destroy();
    }
	}

  void GiantKillBox::CallingSM()
  {
    /*if (owner)
      space->GetGameObject(owner)->Destroy();*/
  }
}
