/*****************************************************************
Filename: CBullet_default.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class Bullet_Default : public GameComponent
	{
	public:
		Bullet_Default();
		~Bullet_Default();
		void LogicUpdate(float dt);
    void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();

		//member variables
    Handle bTransfrom;
    GameObject *bGameObject;
    int damage;
    float ttl; //time to live
    bool explosive_;
	};
}