/*****************************************************************
Filename: CExplosion.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Explosion : public GameComponent
	{
	public:
    Explosion();
    ~Explosion();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();

		//member variables
    Handle eTransfrom;
    Handle eAnSprite;
	};
}