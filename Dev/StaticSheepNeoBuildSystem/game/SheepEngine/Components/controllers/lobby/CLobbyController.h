/*****************************************************************
Filename: CStarController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
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
    //void Draw();

    void GoToState(float dt);
    void Spawn(float dt);
    void Join(float dt);
    void GameStart(float dt);

    Handle LevelLogic, ChipController_;
    LobbyState state_;
    bool playerJoined[4];
    Handle playerJoinText[4];
    int numOfPlayers;
    float spawnTimer;
    bool welcomeSpawned;
  };
}