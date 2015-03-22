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

		//member variables
    Handle transfrom_;
    Handle sprite_;
    float timer;
	};
}