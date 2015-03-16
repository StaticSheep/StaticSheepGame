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

    void Impact();

		//member variables
    Handle bTransfrom;
    GameObject *bGameObject;
    int damage;

    

    /* Bullet dies after a period of time */
    bool limitedLife;

    /* Time until the bullet dies */
    float ttl;

    /* Time to wait after bullet impact to delete the game object.
    The object's sprite and collider are removed upon impact. */
    float fadeTime;
    float lightFade;

    bool fading;

    bool explosive_;
	};
}