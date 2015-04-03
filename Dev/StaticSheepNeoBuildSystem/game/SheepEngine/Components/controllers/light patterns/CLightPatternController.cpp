/*****************************************************************
Filename: CLightPatternController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLightPatternController.h"
#include "types/space/Space.h"
#include "../round/CRoundController.h"
#include "../../gameplay_scripts/arena/CBlockLights.h"
#include "../chip/CChipController.h"
#include "SheepMath.h"

namespace Framework
{
#define NUMOFBLOCKS 33;
  static Vec4 blockColor[4] = {Vec4(0.1f, 1.0f, 0.1f, 0.8f), Vec4(1.0f, 0.1f, 0.1f, 0.8f),
                               Vec4(1.0f, 0.1f, 1.0f, 0.8f), Vec4(0.1f, 0.1f, 1.0f, 0.8f)};
  LightPatternController::LightPatternController()
  {
    
  }

  LightPatternController::~LightPatternController()
  {

  }

  void LightPatternController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(LightPatternController::LogicUpdate));
    space->hooks.Add("SetLightPattern", self, BUILD_FUNCTION(LightPatternController::SetPatternType));

    currPat_ = ROUNDINPRO;
    timer_ = 0.0f;
    delay_ = 0.0f;
    patternSet = false;
    swapFlag = false;
  }

  void LightPatternController::LogicUpdate(float dt)
  {
    UpdatePattern(dt);
  }

  void LightPatternController::UpdatePattern(float dt)
  {
    switch (currPat_)
    {
    case SLOTSPIN:
      SlotSpin(dt);
      break;
    case PLAYERSLOT:
      PlayerSlot(dt);
      break;
    case ROUNDINPRO:
      RoundInProgress(dt);
      break;
    case ROUNDWIN:
      RoundWinner(dt);
      break;
    case GAMEWIN:
      GameWinner(dt);
      break;
    }
  }

  void LightPatternController::SlotSpin(float dt)
  {
    timer_ -= dt;
    if (timer_ <= 0)
      return;

    delay_ -= dt;
    if (delay_ <= 0)
    {
      if (swapFlag)
      {
        BlockLights::EventData ed;
        ed.duration = 0.15f;
        ed.settings.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
        ed.settings.fx = BlockLights::NONE;
        ed.settings.useColor = true;
        ed.overrideDefault = true;
        ed.settings.customData.duration = 0.15f;
        space->hooks.Call("LightingEvent", (unsigned)0xAAAAAAAA, &ed);

        ed.settings.color = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
        space->hooks.Call("LightingEvent", (unsigned)0x55555555, &ed);

        swapFlag = false;
      }
      else
      {
        BlockLights::EventData ed;
        ed.duration = 0.15f;
        ed.settings.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
        ed.settings.fx = BlockLights::NONE;
        ed.settings.useColor = true;
        ed.overrideDefault = true;
        ed.settings.customData.duration = 0.15f;
        space->hooks.Call("LightingEvent", (unsigned)0x55555555, &ed);

        ed.settings.color = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
        space->hooks.Call("LightingEvent", (unsigned)0xAAAAAAAA, &ed);

        swapFlag = true;
      }

      delay_ = 0.15f;
    }
  }

  void LightPatternController::PlayerSlot(float dt)
  {
    timer_ -= dt;
    if (timer_ <= 0)
      return;

    delay_ -= dt;
    if (delay_ <= 0)
    {
      if (swapFlag)
      {
        BlockLights::EventData ed;
        ed.duration = 0.15f;
        ed.settings.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
        ed.settings.fx = BlockLights::NONE;
        ed.settings.useColor = true;
        ed.overrideDefault = true;
        ed.settings.customData.duration = 0.15f;
        space->hooks.Call("LightingEvent", (unsigned)0xAAAAAAAA, &ed);

        ed.settings.color = blockColor[GetRandom(0, 3)];
        space->hooks.Call("LightingEvent", (unsigned)0x55555555, &ed);

        swapFlag = false;
      }
      else
      {
        BlockLights::EventData ed;
        ed.duration = 0.15f;
        ed.settings.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
        ed.settings.fx = BlockLights::NONE;
        ed.settings.useColor = true;
        ed.overrideDefault = true;
        ed.settings.customData.duration = 0.15f;
        space->hooks.Call("LightingEvent", (unsigned)0x55555555, &ed);

        ed.settings.color = blockColor[GetRandom(0, 3)];
        space->hooks.Call("LightingEvent", (unsigned)0xAAAAAAAA, &ed);

        swapFlag = true;
      }

      delay_ = 0.15f;
    }
  }

  void LightPatternController::RoundInProgress(float dt)
  {
    timer_ -= dt;
    if (timer_ <= 0)
      return;

    delay_ -= dt;
    if (delay_ <= 0)
    {
      GetWeights();
      unsigned blocks = 0x00000000;
      unsigned currBlock = 0x00000000;

      for (int i = 0; i < 4; ++i)
      {
        int currWeight = currWeights[i] * NUMOFBLOCKS;
        for (int j = 0; j < currWeight; ++j)
        {
          blocks = blocks | currBlock;
          if (currBlock == 0)
            ++currBlock;
          else
            currBlock *= 2;
        }
        BlockLights::EventData ed;
        ed.duration = 1.0f;
        ed.settings.color = blockColor[i];
        ed.settings.fx = BlockLights::NONE;
        ed.settings.useColor = true;
        ed.overrideDefault = true;
        ed.settings.customData.duration = 0.25f;
        space->hooks.Call("LightingEvent", blocks, &ed);

        blocks = 0x00000000;
      }

      delay_ = 1.0f;
    }
  }

  void LightPatternController::RoundWinner(float dt)
  {

  }

  void LightPatternController::GameWinner(float dt)
  {

  }

  void LightPatternController::SetPatternType(PatternType pt)
  {
    currPat_ = pt;
    patternSet = false;
    if (pt == SLOTSPIN)
    {
      timer_ = 7.0f;
      delay_ = 0.15f;
      swapFlag = false;
    }
    else if (pt == PLAYERSLOT)
    {
      timer_ = 10.0f;
      delay_ = 0.15f;
      swapFlag = false;
    }
    else if (pt == ROUNDINPRO)
    {
      timer_ = 90.0f;
      delay_ = 1.0f;
      swapFlag = false;
    }
  }

  void LightPatternController::GetWeights()
  {
    int totalKills;
    float totalTime;
    switch (space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController)->mode_)
    {
    case GameTypes::FFA:
      totalKills = 0;
      for (int i = 0; i < 4; ++i)
        totalKills += space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->roundPlayerKills[i];

      if (totalKills != 0)
      {
        for (int i = 0; i < 4; ++i)
        {
          currWeights[i] = (float)(space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->roundPlayerKills[i]) / (float)totalKills;
        }
      }
      break;
    case GameTypes::JUGGERNAUT:
      totalTime = 0;
      for (int i = 0; i < 4; ++i)
        totalTime += space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i];

      if (totalTime != 0)
      {
        for (int i = 0; i < 4; ++i)
        {
          currWeights[i] = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i] / totalTime;
        }
      }
      break;
    case GameTypes::SUDDENDEATH:
      totalTime = 0;
      for (int i = 0; i < 4; ++i)
        totalTime += space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->LMSTimeAlive[i];

      for (int i = 0; i < 4; ++i)
      {
        currWeights[i] = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->LMSTimeAlive[i] / totalTime;
      }
      break;
    }
  }

  void LightPatternController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

}

/*
sample call: Low white light flickering for 2 seconds to ALL lights

BlockLights::EventData ed;

ed.duration = 2.0f;
ed.settings.color = Vec4(0.3f, 0.3f, 0.3f, 0.3f);
ed.settings.fx = BlockLights::FLICKER;
ed.settings.customData.duration = 2.0f;

space->hooks.Call("LightingEvent", (unsigned)0xFFFFFFFF, &ed);
*/