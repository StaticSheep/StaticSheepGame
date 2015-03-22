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
  enum PersonalSlotOwner
  {
    P1,
    P2,
    P3,
    P4
  };

	class PersonalSlotController : public GameComponent
	{
	public:
    PersonalSlotController(PersonalSlotOwner owner = P1);
    ~PersonalSlotController();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void BounceDown(float dt);

    void SetSMTextures(int, int*, int*);
    void SetSMResults(int, int*);
    void ReceiveSMResults(std::vector<int>*);

    bool CheckForJP(std::vector<int>);
		//member variables
    float timer;
    float bounceDownTimer;
    bool bounceDownDone;
    PersonalSlotOwner owner_;
    int StypeInt;
    bool done; 
    GameObject *spawnedSM;
    Handle spawnedSMHandle;
    int roundNum;
    GameTypes mode;
    GameMods mod1, mod2;
	};
}