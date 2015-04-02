/*****************************************************************
Filename: CWarningText.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CTutorialText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../gamepad/CGamePad.h"
#include "../../sprites/CSprite.h"
#include "CLobbyController.h"

namespace Framework
{
  TutorialText::TutorialText()
	{

	}

  TutorialText::~TutorialText()
	{

	}

  void TutorialText::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(TutorialText::LogicUpdate));
    space->hooks.Add("GameStart", self, BUILD_FUNCTION(TutorialText::DestroySelf));

	}

  void TutorialText::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("GameStart", self);
	}

  void TutorialText::LogicUpdate(float dt)
  {
    
	}

  void TutorialText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

}
