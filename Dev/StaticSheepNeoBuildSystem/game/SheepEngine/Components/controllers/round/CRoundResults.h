/*****************************************************************
Filename: CWarningText.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
    void AwardChip(float dt);
    void DetermineWinner();

    void Draw();
    void DrawKills();
    void DrawDeaths();
    void DrawJuggTime();
    void DrawLMSTime();
    void DrawChipsAwarded();
    void DrawTotalChips();
    void DrawTotalKills();
    void DrawTotalDeaths();
    void DraowTotalJuggTime();

		//member variables
    Handle rTransfrom;
    Handle roundSprite;
    Handle ChipCont;
    Handle winner_chip[4];
    float bounceDownTimer, timeToLive;
    int winner;
    bool winners_[4];
    bool chip_spawned[4];
    bool bounceDownDone, startDrawing, tie;
    GameTypes mode_;
    bool chipAwarded;

    int wordFontIndex;
    int numberFontIndex;
	};
}