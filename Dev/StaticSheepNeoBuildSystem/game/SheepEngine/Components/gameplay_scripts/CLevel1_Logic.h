/*****************************************************************
Filename: CLevel1_Logic.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Level1_Logic : public GameComponent
	{
    enum GameTypes
    {
      FFA,
      TEAM,
      JUGGERNAUT,
      MINIME,
      SUMO
    };

	public:
    Level1_Logic();
    ~Level1_Logic();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void SpawnPlayers(float dt);
    void PlayerDied(int ply);
    void CameraShake(float dt, float shakeDuration, float magnitude);
		//member variables
    //Handle bTransfrom;
    float timeLimit;
    float spawnTimer;
    bool playing;
    bool startFlag;
    Handle levelSound;
    Handle levelCamera;
    Handle levelTransform;
    Handle levelEmitter;
    Handle levelSprite;
    Vec3 spawnPos[6];
    Handle Players[4];
    float spawnTimers[4];
    int playerLives[4];
    int deadPlayers;
    int numOfPlayers;
    bool camShake, shake;
    bool countDownDone;
    float shakeTime;
    float countDownTimer;

    int GetPlayerHealth(int);
    int GetPlayerLives(int);
    int GetWinner();
    void EndMatch();
    void CheatWin();

	};
}