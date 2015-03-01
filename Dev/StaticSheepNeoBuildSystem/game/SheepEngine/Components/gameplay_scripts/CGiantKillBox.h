/*****************************************************************
Filename: CGiantKillBox.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../transform/CTransform.h"

namespace Framework
{
	class GiantKillBox : public GameComponent
	{
	public:
    GiantKillBox();
    ~GiantKillBox();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void CallingSM();

		//member variables
    Handle Grinders[12]; //12 is the max amount of grinders to hold for this level event
    Transform *GT[12];
    Handle kbTransfrom;
    Handle kbCollider;
    bool direction, GrindSpawn;
    int numOfGrinders;
    float sizeOfGrinder;
	};
}