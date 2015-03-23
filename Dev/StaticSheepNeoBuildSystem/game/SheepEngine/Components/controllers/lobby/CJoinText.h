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
	class JoinText : public GameComponent
	{
	public:
    JoinText();
    ~JoinText();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();

    void Blink(float dt);

		//member variables
    Handle transfrom_;
    Handle sprite_;
    Handle lobbyController_;
    float timer;
    int playerNum;
    bool blink;
	};
}