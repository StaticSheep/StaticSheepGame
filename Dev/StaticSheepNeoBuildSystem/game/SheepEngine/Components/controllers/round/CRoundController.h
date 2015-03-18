/*****************************************************************
Filename: CStarController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"
#include "../../gameplay_scripts/Level_Scripts/CLevel1_Logic.h"

namespace Framework
{
  class RoundController : public GameComponent
  {
  public:
    enum RoundState
    {
      ROUNDSTART,
      ROUNDINPRO,
      ROUNDOVER,
      GAMEOVER
    };
    RoundController();
    ~RoundController();
    void Initialize();
    void LogicUpdate(float dt);
    void Remove();
    void Draw();

    void GoToState(float dt);
    void RoundStart(float dt);
    void RoundInProgress(float dt);
    void RoundOver(float dt);
    void GameOver(float dt);
    void SlotMachineDone(GameTypes mode);
    void RoundCountDown();
    void AwardEndOfRoundChips();

    Handle LevelLogic, ChipController_;
    int current_round, max_rounds;
    RoundState state_;
    float round_state_timer;
    float timeOfRound;
    bool spawned_round_start, slotMachineDone, roundUp_spawned, EORAwarded, ResultsSpawned;
    bool num_spawned[5];
    char round_timer[10];
    int font_index;
    GameTypes mode_;
  };
}