#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Level1_Logic : public GameComponent
	{
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
    Vec3 spawnPos[6];
    Handle Players[4];
    float spawnTimers[4];
    int playerLives[4];
    int deadPlayers;
    int numOfPlayers;
    bool camShake, shake;
    float shakeTime;

    int GetPlayerHealth(int);
    int GetPlayerLives(int);
    int GetWinner();
    void EndMatch();

	};
}