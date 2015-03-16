/*****************************************************************
Filename: CChipController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"

namespace Framework
{
  class ChipController : public GameComponent
  {
  public:
    ChipController();
    ~ChipController();
    void Initialize();
    void LogicUpdate(float dt);
    void Remove();

    void GivePlayerChip(int player, int chips);
    void DrawChips();
    void PlayerDied(int ply, int who_killed_them);
    void JuggDied(int jugg, float timeAsJugg);
    void ResetRoundStats();

    void FFAAwards();
    void JuggAwards();
    void SDAwards();

    void Draw();

    Handle LevelLogic;
    int playerChips[4];

    int totalPlayerKills[4], roundPlayerKills[4];
    int totalPlayerDeaths[4], roundPlayerDeaths[4];
    float totalTimeAsJugg[4], roundTimeAsJugg[4];
    int totalPlayerLMS[4];
    int LMSThisRound;
  };
}