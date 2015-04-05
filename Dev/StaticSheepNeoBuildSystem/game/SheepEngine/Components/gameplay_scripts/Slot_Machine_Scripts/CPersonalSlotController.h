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

	class PersonalSlotController : public GameComponent
	{
	public:
    PersonalSlotController();
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
    int playerNum, psmNum;
    float timer;
    float bounceDownTimer;
    bool bounceDownDone;
    bool done;
    bool gold_;
	};
}