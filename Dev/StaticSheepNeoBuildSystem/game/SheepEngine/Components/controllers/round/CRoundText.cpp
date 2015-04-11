/*****************************************************************
Filename: CRoundText.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"
#include "../../sound/CSoundEmitter.h"

namespace Framework
{
  RoundText::RoundText()
	{
    text = true;
    number = 1;
    timer = 3.0f;
    LeftToRight = false;
    initialSpeed = 80.0f;
    middleSpeed = 2.5f;
    killRange = 1000.0f;
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

    if (!text)
    {
      AniSprite *numSprite = space->GetGameObject(owner)->GetComponent<AniSprite>(eAniSprite);

      if (numSprite != nullptr)
        numSprite->SetRange(Vec2((float)number, (float)number));
    }

    swooshedIn = false;
    swooshedOut = true;
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
    
    //play swoosh in
    if (!swooshedIn)
      PlaySwooshIn();
    if (!text)
    {
      AniSprite *numSprite = space->GetGameObject(owner)->GetComponent<AniSprite>(eAniSprite);

      if (numSprite != nullptr)
        numSprite->SetRange(Vec2((float)number, (float)number));
    }

    if (LeftToRight)
    {
      if (currPos.x > -100.0f && currPos.x < 100.0f)
      {
        rt->SetTranslation(currPos + Vec3(middleSpeed, 0.0f, 0.0f));
        swooshedOut = false;
      }
      else
      {
        //play swoosh out
        if (!swooshedOut)
          PlaySwooshOut();
        rt->SetTranslation(currPos + Vec3(initialSpeed, 0.0f, 0.0f));
      }
    }
    else
    {
      if (currPos.x > -100.0f && currPos.x < 100.0f)
      {
        rt->SetTranslation(currPos + Vec3(-middleSpeed, 0.0f, 0.0f));
        swooshedOut = false;
      }
      else
      {
        //play swoosh out
        if (!swooshedOut)
          PlaySwooshOut();
        rt->SetTranslation(currPos + Vec3(-initialSpeed, 0.0f, 0.0f));
      }
    }


    if (currPos.x < -killRange || currPos.x > killRange)
    {
      DestroySelf();
    }

	}

  void RoundText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  void RoundText::PlaySwooshIn()
  {
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    if (text)
      se->Play("swoosh_low", &SoundInstance(1.0f));
    else
    {
      //se->Play("swooesh_med_high", &SoundInstance(1.0f));
    }

    swooshedIn = true;
  }

  void RoundText::PlaySwooshOut()
  {
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    if (text)
      se->Play("swoosh_medium", &SoundInstance(1.0f));
    else
    {
      //se->Play("swoosh_high", &SoundInstance(1.0f));
    }

    swooshedOut = true;
  }
}
