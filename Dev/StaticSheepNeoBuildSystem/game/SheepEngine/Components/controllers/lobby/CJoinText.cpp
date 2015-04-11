/*****************************************************************
Filename: CJoinText.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CJoinText.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../gamepad/CGamePad.h"
#include "../../sprites/CSprite.h"
#include "CLobbyController.h"

namespace Framework
{
  JoinText::JoinText()
	{
    timer = 3.0f;
    playerNum = -1;
    Handle lobbyController_ = Handle::null;
    blink = false;
    activePlayer = false;
	}

  JoinText::~JoinText()
	{

	}

  void JoinText::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(JoinText::LogicUpdate));
    space->hooks.Add("GameStart", self, BUILD_FUNCTION(JoinText::DestroySelf));

    transfrom_ = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    sprite_ = space->GetGameObject(owner)->GetComponentHandle(eSprite);

	}

  void JoinText::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("GameStart", self);
	}

  void JoinText::LogicUpdate(float dt)
  {
    Transform *rt = space->GetHandles().GetAs<Transform>(transfrom_);
    Vec3 currPos = rt->GetTranslation();
    GamePad *playGP = space->GetGameObject(owner)->GetComponent<GamePad>(eGamePad);
    playGP->SetPad(playerNum);

    if (!activePlayer)
      Blink(dt);

    if (playGP->ButtonPressed(XButtons.A) && !activePlayer)
    {
      Framework::playerJoined[playerNum] = true;
      //DestroySelf();
      Sprite *textSprite = space->GetHandles().GetAs<Sprite>(sprite_);
      textSprite->Color.A = 0.0f;
      activePlayer = true;
      SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
      se->Play("select", &SoundInstance(1.0f));
    }
    if (playGP->ButtonPressed(XButtons.Start))
    {
      space->GetGameObject(lobbyController_)->GetComponent<LobbyController>(eLobbyController)->startPressed = true;
    }
	}

  void JoinText::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  void JoinText::Blink(float dt)
  {
    Sprite *textSprite = space->GetHandles().GetAs<Sprite>(sprite_);

    if (!blink)
      textSprite->Color.A -= dt * 5.0f;
    else
      textSprite->Color.A += dt * 5.0f;


    if (textSprite->Color.A <= 0.5f)
      blink = true;
    else if (textSprite->Color.A >= 1.0f)
      blink = false;
  }

}
