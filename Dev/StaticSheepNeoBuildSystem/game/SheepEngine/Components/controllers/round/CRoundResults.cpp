/*****************************************************************
Filename: CRoundResults.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CRoundResults.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sprites/CAniSprite.h"
#include "../chip/CChipController.h"
#include "../../lights/CSpriteLight.h"

namespace Framework
{
  static Vec2D wordSize(21.0f, 21.0f);
  static Vec2D numberSize(50.0f, 50.0f);
  static float playerHeadY_[4] {252.0f, 116.0f, -42.0f, -210.0f};

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
    space->hooks.Add("PostDraw", self, BUILD_FUNCTION(RoundResults::Draw));

    rTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    roundSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    bounceDownTimer = .5f;
    timeToLive = 6.0f;
    bounceDownDone = false;
    startDrawing = false;
    chipAwarded = false;
    tie = false;
    for (int i = 0; i < 4; ++i)
    {
      winner_chip[i] = Handle::null;
      winners_[i] = false;
    }
   
    winner = -1;

    wordFontIndex = Draw::GetFontIndex("aircruiser");
    numberFontIndex = Draw::GetFontIndex("BN_Jinx");
	}

  void RoundResults::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("PostDraw", self);
	}

  void RoundResults::LogicUpdate(float dt)
  {
    
    //Vec3 currPos = rt->GetTranslation();

    if (!bounceDownDone)
    {
      BounceDown(dt);
      if (mode_ == GAMEOVER)
        space->GetGameObject(owner)->GetComponent<Sprite>(eSprite)->SetTexture("screen_end.png");
    }
    else
      startDrawing = true;

    if (startDrawing)
    {
      DetermineWinner();
      AwardChip(dt);
    }

    if (mode_ == GAMEOVER)
      return;

    else if (timeToLive <= 0)
    {
      Transform *thisTrans = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
      thisTrans->SetTranslation(thisTrans->GetTranslation() + Vec3(0.0f, 70.0f, 0.0f));
      if (thisTrans->GetTranslation().y >= 950.0f)
      {
        for (int i = 0; i < 4; ++i)
        {
          if (winner_chip[i] != Handle::null)
            space->GetGameObject(winner_chip[i])->Destroy();
        }
        DestroySelf();
      }
    }
    timeToLive -= dt;
	}

  void RoundResults::DestroySelf()
  {
    space->GetGameObject(owner)->Destroy();
  }

  static bool soundFlag_ = false;
  void RoundResults::BounceDown(float dt)
  {
    Transform *rt = space->GetHandles().GetAs<Transform>(rTransfrom);
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);

    if (bounceDownTimer >= 0.3f)
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -80.0f, 0.0f));
    else if (bounceDownTimer >= 0.2)
    {
      if (!soundFlag_)
      {
        se->Play("impact1", &SoundInstance(1.0f));
        soundFlag_ = true;
      }
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

  void RoundResults::AwardChip(float dt)
  {
    for (int i = 0; i < 4; ++i)
    {
      if (winner_chip[i] == Handle::null && !chip_spawned[i] && winners_[i])
      {
        winner_chip[i] = (FACTORY->LoadObjectFromArchetype(space, "winner_chip"))->self;
        space->GetGameObject(winner_chip[i])->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, playerHeadY_[i], 0.0f));
        chip_spawned[i] = true;
        break;
      }

      if (winner_chip[i] != Handle::null)
      {
        Transform *ct = space->GetGameObject(winner_chip[i])->GetComponent<Transform>(eTransform);
        if (ct->GetTranslation().x < -700.0f)
          ct->SetTranslation(ct->GetTranslation() + Vec3(50.0f, 0.0f, 0.0f));

        if (timeToLive <= 0.75f)
        {
          ct->SetTranslation(ct->GetTranslation() + Vec3(-120.0f, 0.0f, 0.0f));
        }
      }
    }

    
    
  }

  void RoundResults::DetermineWinner()
  {
    int mostKills;
    int mostChips;
    int currWinner;
    float mostTime;
    tie = false;
    switch (mode_)
    {
    case FFA:
      mostKills = 0;
      currWinner = -1;
      for (int i = 0; i < 4; ++i)
      {
        int playKills = space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundPlayerKills[i];
        if (playKills > 0 && playKills > mostKills)
        {
          mostKills = playKills;
          currWinner = i;
        }
      }
      if (currWinner != -1)
      {
        for (int i = 0; i < 4; ++i)
        {
          if (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundPlayerKills[i] == mostKills)
          {
            tie = true;
            winners_[i] = true;
          }
        }
      }
      if (currWinner != -1 && !tie)
        winner = currWinner;
      break;
    case JUGGERNAUT:
      mostTime = 0.0f;
      currWinner = -1;
      for (int i = 0; i < 4; ++i)
      {
        float playTime = space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i];
        if (playTime > 0 && playTime > mostTime)
        {
          mostTime = playTime;
          currWinner = i;
        }
      }
      if (currWinner != -1)
      {
        for (int i = 0; i < 4; ++i)
        {
          if (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i] == mostTime)
          {
            tie = true;
            winners_[i] = true;
          }
        }
      }
      if (currWinner != -1 && !tie)
        winner = currWinner;
      break;
    case SUDDENDEATH:
      winner = space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->LMSThisRound;
      break;
    case GAMEOVER:
      mostChips = 0;
      currWinner = -1;
      for (int i = 0; i < 4; ++i)
      {
        float playChips = space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->playerChips[i];
        if (playChips > 0 && playChips > mostChips)
        {
          mostChips = playChips;
          currWinner = i;
        }
      }
      if (currWinner != -1)
      {
        for (int i = 0; i < 4; ++i)
        {
          if (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->playerChips[i] == mostChips)
          {
            tie = true;
            winners_[i] = true;
          }
        }
      }
      if (currWinner != -1 && !tie)
        winner = currWinner;
      break;
    }
  }

  void RoundResults::Draw()
  {
    if (startDrawing && timeToLive > 0)
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
      case GAMEOVER:
        DrawTotalKills();
        DrawTotalDeaths();
        DraowTotalJuggTime();
        DrawTotalChips();
        break;
      }
    }
  }

  void RoundResults::DrawKills()
  {
    Draw::SetPosition(-300.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Kills", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
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
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawDeaths()
  {
    Draw::SetPosition(0.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Deaths", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
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
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawJuggTime()
  {
    Draw::SetPosition(-300.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Time Being IT", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
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

      sprintf(playerString, "%5.2f", (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i]));
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawLMSTime()
  {
    Draw::SetPosition(-100.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Time Alive", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
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
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawChipsAwarded()
  {

  }

  void RoundResults::DrawTotalChips()
  {
    
    Draw::SetPosition(275.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Total Chips", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
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
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawTotalKills()
  {
    Draw::SetPosition(-425.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Total Kills", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-375.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(-375.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(-375.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-375.0f, -172.0f, 0.0f);

      itoa(space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->totalPlayerKills[i], playerString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DrawTotalDeaths()
  {
    Draw::SetPosition(-225.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Total Deaths", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(-175.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(-175.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(-175.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(-175.0f, -172.0f, 0.0f);

      itoa(space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->totalPlayerDeaths[i], playerString, 10);
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

  void RoundResults::DraowTotalJuggTime()
  {
    Draw::SetPosition(25.0f, 310.0f);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    Draw::DrawString("Tagged Time", wordSize, wordFontIndex);
    Vec3 pos;
    char playerString[10];
    for (int i = 0; i < 4; ++i)
    {
      //depending on the player, it draws the totals in the correct place
      if (i == 0)
        pos = Vec3(50.0f, 232.0f, 0.0f);
      if (i == 1)
        pos = Vec3(50.0f, 106.0f, 0.0f);
      if (i == 2)
        pos = Vec3(50.0f, -32.0f, 0.0f);
      if (i == 3)
        pos = Vec3(50.0f, -172.0f, 0.0f);

      sprintf(playerString, "%5.2f", (space->GetGameObject(ChipCont)->GetComponent<ChipController>(eChipController)->totalTimeAsJugg[i]));
      Draw::SetPosition(pos.x, pos.y);
      Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
      Draw::SetRotation(0);
      Draw::DrawString(playerString, numberSize, numberFontIndex);
    }
  }

}
