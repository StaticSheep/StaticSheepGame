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
    Vec3 spawnPos[4];
    Handle Players[4];
    float spawnTimers[4];
    int numOfPlayers;
    bool camShake, shake;
    float shakeTime;
	};
}