/*****************************************************************
Filename: CSlotController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "CLevel1_Logic.h"

namespace Framework
{
  enum SlotType
  {
    GOLD,
    JACKPOT,
    INDIVIDUAL
  };

	class SlotController : public GameComponent
	{
	public:
    SlotController();
    ~SlotController();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();

    void SetSMTextures(int, int*, int*);
    void SetSMResults(int, int*);
    void ReceiveSMResults(std::vector<int>*);

    bool CheckForJP(std::vector<int>);
    GameObject* SpawnChildSM(SlotType type);
		//member variables
    float levelTimer; //keeps track of overall time in the level for slot machine switches
    SlotType Stype;
    int StypeInt;
    bool done; 
    GameObject *spawnedSM;
    Handle spawnedSMHandle;

    GameTypes mode;
    GameMods mod1, mod2;
	};
}