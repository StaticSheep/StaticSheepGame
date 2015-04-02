/*****************************************************************
Filename: CSlotController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../Level_Scripts/CLevel1_Logic.h"

namespace Framework
{

	class PersonalSlotSpawner : public GameComponent
	{
	public:
    PersonalSlotSpawner();
    ~PersonalSlotSpawner();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void BounceDown(float dt);
    void SpawnPersonalSM(int num);
    void Draw();
    void AllSlotsDone();

		//member variables
    float timer;
    float bounceDownTimer;
    bool bounceDownDone;
    bool coinsSpawned;
    bool stopEffects, done_, playing, stop_;

    int playerNum;
    int playerCoinTotal;
    int totalPSM;
    Handle spawnedPSM[3];
    Handle level_logic;
    Handle spawnedCoins;
	};
}