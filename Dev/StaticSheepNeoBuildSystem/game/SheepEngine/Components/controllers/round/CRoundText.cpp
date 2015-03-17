/*****************************************************************
Filename: CWarningText.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"

namespace Framework
{
  RoundText::RoundText()
	{
    text = true;
    roundNum = 1;
	}

  RoundText::~RoundText()
	{

	}

  void RoundText::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(RoundText::LogicUpdate));

    rTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    roundSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    timer = 3.0f;
	}

  void RoundText::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("AnimEnd", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void RoundText::LogicUpdate(float dt)
  {
    Transform *rt = space->GetHandles().GetAs<Transform>(rTransfrom);
    Vec3 currPos = rt->GetTranslation();
    if (text)
    {
      if (currPos.x > -100.0f && currPos.x < 100.0f)
      {
        rt->SetTranslation(currPos + Vec3(2.0f, 0.0f, 0.0f));
      }
      else
      {
        rt->SetTranslation(currPos + Vec3(50.0f, 0.0f, 0.0f));
      }

    }
    else
    {
      AniSprite *numSprite = space->GetGameObject(owner)->GetComponent<AniSprite>(eAniSprite);
      if (numSprite != nullptr)
      {
        numSprite->SetRange(Vec2((float)roundNum, (float)roundNum));
      }
      if (currPos.x > -100.0f && currPos.x < 100.0f)
      {
        rt->SetTranslation(currPos - Vec3(2.0f, 0.0f, 0.0f));
      }
      else
      {
        rt->SetTranslation(currPos - Vec3(50.0f, 0.0f, 0.0f));
      }
    }

    if (currPos.x < -1000 || currPos.x > 1000)
    {
      DestroySelf();
    }

	}

  void RoundText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

}
