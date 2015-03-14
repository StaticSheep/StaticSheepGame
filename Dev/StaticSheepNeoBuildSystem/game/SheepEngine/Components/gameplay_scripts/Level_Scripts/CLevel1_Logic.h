/*****************************************************************
Filename: CLevel1_Logic.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../types/levelEvents/LEBase.h"

namespace Framework
{
  enum GameTypes
  {
    FFA,
    JUGGERNAUT,
    SUDDENDEATH,
    BONUSMODE,
    SLOTMACHINE
  };
  enum GameMods
  {
    LIGHTSOUT,
    BONUS,
    EXPLOSIVEROUNDS,
    SHOTGUNS,
    ROCKETS
  };


	class Level1_Logic : public GameComponent
	{
	public:
    Level1_Logic();
    ~Level1_Logic();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void SpawnPlayers(float dt);
    void PlayerDied(int ply, int who_killed_them);
    void CameraShake(float dt, float shakeDuration, float magnitude);
    bool LevelCountdown(float dt);
		//member variables
    //Handle bTransfrom;
    float eventTimer;
    float spawnTimer;
    float roundTimer;
    float juggernautTimer;
    bool playing;
    bool startFlag;
    Handle levelSound;
    Handle levelCamera;
    Handle levelTransform;
    Handle levelEmitter;
    Handle levelSprite;
    Handle levelSM;
    Vec3 spawnPos[6];
    Handle Players[4];
    float spawnTimers[4];
    size_t playerCoins[4];
    Handle playerCoinStack[4];
    Vec3 CoinStackPos[4];
    int playerCoinsThisFrame[4];
    std::deque<std::pair<int, float>> coinStringsAlive[4];
    bool juggernaut[4];
    int deadPlayers;
    int numOfPlayers;
    bool camShake, shake;
    bool countDownDone;
    bool slotFinished;
    float shakeTime;
    float countDownTimer;
    LevelEvent *LE;
    GameTypes mode;
    GameMods mod1, mod2;

    int GetPlayerHealth(int);
    int GetPlayerLives(int);
    int GetWinner();
    void EndMatch();
    void CheatWin();
    void ResetPlayers();
    void ResetSpawnTimers();
    void GivePlayerCoins(int player, int coins);
    void UpdateCoinStacks();

    void SpawnCoinStacks();
    void SpawnItem(const char *, Vec3);
    void SpawnItemSet(Vec3);
    void SpawnCoins(Vec3);
    void SpawnLevelEvent();

    void GoToGameMode(float dt);
    void FFAMode(float dt);
    void JuggernautMode(float dt);
    void MakeJuggernaut();
    void ResetJuggernaut();
    void SuddenDeathMode(float dt);
    void SlotMachineMode(float dt);
    void BonusMode(float dt);
    bool LastManStanding();

    void SlotFinished(GameTypes mode);
    void SetMods(GameMods mod1, GameMods mod2);

    void Draw();
	};
}