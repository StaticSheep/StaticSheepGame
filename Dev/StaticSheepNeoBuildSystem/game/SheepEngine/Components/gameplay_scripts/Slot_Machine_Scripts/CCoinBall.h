/*****************************************************************
Filename: CCoinBall.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class CoinBall : public GameComponent
	{
	public:
    CoinBall();
    ~CoinBall();
		void LogicUpdate(float dt);
		void Initialize();
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Remove();
    void NormalizeSpeed();
    void DestroyBall();
		//member variables
    Handle cbTransfrom;
    Handle cbCollider;
    int hp, lasthp;
    bool alive;
	};
}