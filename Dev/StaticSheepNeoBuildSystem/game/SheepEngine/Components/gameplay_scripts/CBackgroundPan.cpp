/*****************************************************************
Filename: CBackgroundPan.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CBackgroundPan.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"

namespace Framework
{
  BackgroundPan::BackgroundPan()
	{

	}

  BackgroundPan::~BackgroundPan()
	{

	}

  void BackgroundPan::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(BackgroundPan::LogicUpdate));

    bTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
	}

  void BackgroundPan::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void BackgroundPan::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(bTransfrom);

    pt->SetRotation(pt->GetRotation() + (dt / 20));
    if (pt->GetRotation() > 2 * PI)
      pt->SetRotation(0);
	}

}
