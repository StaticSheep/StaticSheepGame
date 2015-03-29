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
  class LobbyController : public GameComponent
  {
  public:
    enum LobbyState
    {
      SPAWN,
      JOIN,
      GAME_START
    };
    LobbyController();
    ~LobbyController();
    void Initialize();
    void LogicUpdate(float dt);
    void Remove();
    void PlayerDied(int player, int whoKilledThem);
    //void Draw();

    void GoToState(float dt);
    void Spawn(float dt);
    void Join(float dt);
    void GameStart(float dt);
    void BounceDown(float dt);
    void PlayPowerDown();

    Handle LevelLogic, ChipController_;
    LobbyState state_;
    Handle playerJoinText[4];
    Handle Players[4];
    Handle levelSound;
    int numOfPlayers;
    float spawnTimer, timer_;
    float bounceDownTimer;
    bool spawned, bounceDownDone, startPressed;
    bool playing;
    bool powerDownSound = false;
    Vec3 spawnPos[4];
  };
}