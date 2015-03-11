/*****************************************************************
Filename: CWarningText.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CWarningText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"

namespace Framework
{
  WarningText::WarningText()
	{
   
	}

  WarningText::~WarningText()
	{

	}

  void WarningText::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(WarningText::LogicUpdate));

    eTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    warnSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    timer = 2.0f;
    blink = false;
	}

  void WarningText::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("AnimEnd", self);
		space->hooks.Remove("LogicUpdate", self);
	}

  void WarningText::LogicUpdate(float dt)
	{
    //Transform *pt = space->GetHandles().GetAs<Transform>(eTransfrom);
    RespawnBlink(dt);
	}

  void WarningText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  void WarningText::RespawnBlink(float dt)
  {
    Sprite *ws = space->GetHandles().GetAs <Sprite>(warnSprite);

    if (timer > 0.0f)
    {
      if (!blink)
        ws->Color.A -= dt * 10.0f;
      else
        ws->Color.A += dt * 10.0f;

      timer -= dt;

      if (ws->Color.A <= 0.0f)
        blink = true;

      if (ws->Color.A >= 1.0f)
        blink = false;
    }
    else
    {
      ws->Color.A = 255.0f;
      timer = 2.0f;
      DestroySelf();
    }
  }
}
