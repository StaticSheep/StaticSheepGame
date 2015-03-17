/*****************************************************************
Filename: CWarningText.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class RoundText : public GameComponent
	{
	public:
    RoundText();
    ~RoundText();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();

		//member variables
    Handle rTransfrom;
    Handle roundSprite;
    float timer;
    bool text; //is it text or a number?
    int number; //if it is a number, this is a 0-9 value
    bool LeftToRight; //if true goes left to right, otherwise it will go right to left
    //intitialSpeed is when it comes in and out. middle speed is when it passes through center
    float initialSpeed, middleSpeed, killRange; 
	};
}