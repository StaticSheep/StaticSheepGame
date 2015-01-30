/*****************************************************************
Filename: CAimingArrow.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../gamepad/CGamePad.h"

namespace Framework
{
	class AimingArrow : public GameComponent
	{
	public:
    AimingArrow();
    ~AimingArrow();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();
    Vec3 aimingDirection(GamePad *gp, char stick);
		//member variables
    Handle arrowTrans;
    Handle arrowSprite;
    Handle playerGamePad;
    Vec4 arrowColor;

	};
}