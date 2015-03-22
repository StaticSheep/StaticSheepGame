/*****************************************************************
Filename: CWarningText.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CJoinText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"

namespace Framework
{
  JoinText::JoinText()
	{
    timer = 3.0f;
	}

  JoinText::~JoinText()
	{

	}

  void JoinText::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(JoinText::LogicUpdate));

    transfrom_ = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    sprite_ = space->GetGameObject(owner)->GetComponentHandle(eSprite);

	}

  void JoinText::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("AnimEnd", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void JoinText::LogicUpdate(float dt)
  {
    Transform *rt = space->GetHandles().GetAs<Transform>(transfrom_);
    Vec3 currPos = rt->GetTranslation();
    

	}

  void JoinText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

}
