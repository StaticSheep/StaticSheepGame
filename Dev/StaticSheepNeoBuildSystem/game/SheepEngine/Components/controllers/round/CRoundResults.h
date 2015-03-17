/*****************************************************************
Filename: CWarningText.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../../gameplay_scripts/Level_Scripts/CLevel1_Logic.h"

namespace Framework
{
	class RoundResults : public GameComponent
	{
	public:
    RoundResults();
    ~RoundResults();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();
    void BounceDown(float dt);

    void Draw();
    void DrawKills();
    void DrawDeaths();
    void DrawJuggTime();
    void DrawChipsAwarded();
    void DrawTotalChips();

		//member variables
    Handle rTransfrom;
    Handle roundSprite;
    Handle ChipCont;
    float bounceDownTimer, timeToLive;
    bool bounceDownDone, startDrawing;
    GameTypes mode_;
	};
}