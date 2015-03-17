/*****************************************************************
Filename: CWarningText.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundResults.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"
#include "../chip/CChipController.h"

namespace Framework
{
  RoundResults::RoundResults()
	{

	}

  RoundResults::~RoundResults()
	{

	}

  void RoundResults::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(RoundResults::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(RoundResults::Draw));

    rTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    roundSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    bounceDownTimer = .5f;
    timeToLive = 6.0f;
    bounceDownDone = false;
    startDrawing = false;
	}

  void RoundResults::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
	}

  void RoundResults::LogicUpdate(float dt)
  {
    
    //Vec3 currPos = rt->GetTranslation();

    if (!bounceDownDone)
      BounceDown(dt);
    else
      startDrawing = true;

    timeToLive -= dt;
    if (timeToLive <= 0)
    {
      DestroySelf();
    }

	}

  void RoundResults::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  void RoundResults::BounceDown(float dt)
  {
    Transform *rt = space->GetHandles().GetAs<Transform>(rTransfrom);
    if (bounceDownTimer >= 0.3f)
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -80.0f, 0.0f));
    else if (bounceDownTimer >= 0.2)
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 40.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() + 0.035f);
    }
    else
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -20.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() - 0.017f);
    }


    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      rt->SetRotation(0.0f);
      bounceDownDone = true;
    }
  }

  void RoundResults::Draw()
  {
    if (startDrawing)
    {
      Vec3 pos;
      Vec2D scale(50, 50);
      char playerString[10];
      switch (mode_)
      {
      case FFA:
        DrawKills();
        DrawDeaths();
        //DrawChipsAwarded();
        DrawTotalChips();
        break;
      case JUGGERNAUT:
        DrawJuggTime();
        //DrawChipsAwarded();
        DrawTotalChips();
        break;
      case SUDDENDEATH:
        DrawLMSTime();
        //DrawChipsAwarded();
        DrawTotalChips();
        break;
      }
    }
  }

  void RoundResults::DrawKills()
  {
    Vec2D scale(40, 40);
    Draw::SetPosition(-300.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Kills", scale, 1);
    Vec3 pos;
    char playerString[10];
    scale = Vec2D(50, 50);
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-300.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(-300.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(-300.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-300.0f, -172.0f, 0.0f);

      itoa(space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundPlayerKills[i], playerString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);
    }
  }

  void RoundResults::DrawDeaths()
  {
    Vec2D scale(40, 40);
    Draw::SetPosition(0.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Deaths", scale, 1);
    Vec3 pos;
    char playerString[10];
    scale = Vec2D(50, 50);
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(0.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(0.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(0.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(0.0f, -172.0f, 0.0f);

      itoa(space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundPlayerDeaths[i], playerString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);
    }
  }

  void RoundResults::DrawJuggTime()
  {
    Vec2D scale(40, 40);
    Draw::SetPosition(-100.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Time Being IT", scale, 1);
    Vec3 pos;
    char playerString[10];
    scale = Vec2D(50, 50);
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-100.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(-100.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(-100.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-100.0f, -172.0f, 0.0f);

      sprintf(playerString, "%5.2f", (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i]));
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);
    }
  }

  void RoundResults::DrawLMSTime()
  {
    Vec2D scale(40, 40);
    Draw::SetPosition(-100.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Time Alive", scale, 1);
    Vec3 pos;
    char playerString[10];
    scale = Vec2D(50, 50);
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-100.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(-100.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(-100.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-100.0f, -172.0f, 0.0f);

      sprintf(playerString, "%5.2f", (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->LMSTimeAlive[i]));
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);
    }
  }

  void RoundResults::DrawChipsAwarded()
  {

  }

  void RoundResults::DrawTotalChips()
  {
    Vec2D scale(40, 40);
    Draw::SetPosition(300.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Total Chips", scale, 1);
    Vec3 pos;
    char playerString[10];
    scale = Vec2D(50, 50);
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(300.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(300.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(300.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(300.0f, -172.0f, 0.0f);

      itoa(space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->playerChips[i], playerString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, scale, 1);
    }
  }

}
