/*****************************************************************
Filename: CLightPatternController.h
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
  class LightPatternController : public GameComponent
  {
  public:
    enum PatternType
    {
      SLOTSPIN,
      PLAYERSLOT,
      ROUNDINPRO,
      ROUNDWIN,
      GAMEWIN,

    };
    LightPatternController();
    ~LightPatternController();
    void Initialize();
    void LogicUpdate(float dt);
    void Remove();

    void UpdatePattern(float dt);
    void SlotSpin(float dt);
    void PlayerSlot(float dt);
    void RoundInProgress(float dt);
    void RoundWinner(float dt);
    void GameWinner(float dt);
    void SetPatternType(PatternType);
    void GetWeights();

    PatternType currPat_;
    float timer_, delay_;
    bool patternSet, swapFlag;
    float currWeights[4];
    int currWinner;
  };
}