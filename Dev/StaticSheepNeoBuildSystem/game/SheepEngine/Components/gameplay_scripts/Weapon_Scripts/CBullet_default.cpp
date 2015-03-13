/*****************************************************************
Filename: CBullet_default.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CBullet_default.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"

namespace Framework
{
	Bullet_Default::Bullet_Default()
	{
    damage = 10;
    ttl = 0.375f;
    explosive_ = false;
	}

	Bullet_Default::~Bullet_Default()
	{

	}

	void Bullet_Default::Initialize()
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Bullet_Default::LogicUpdate));
		space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(Bullet_Default::OnCollision));

    bTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    bGameObject = space->GetHandles().GetAs<GameObject>(owner);
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eCircleCollider))->SetGravityOff();
	}

	void Bullet_Default::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  static Transform *bt;
	void Bullet_Default::LogicUpdate(float dt)
	{
    bt = space->GetHandles().GetAs<Transform>(bTransfrom);
    bGameObject = space->GetHandles().GetAs<GameObject>(owner);

    if (bGameObject->archetype == "Bullet_shot")
    {
      ttl -= dt;
      if (ttl <= 0)
        space->GetGameObject(owner)->Destroy();
    }

    if (bt->GetTranslation().x > 1000 || bt->GetTranslation().x < -1000 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
      space->GetGameObject(owner)->Destroy();

	}

  void Bullet_Default::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    if (OtherObject->name != "Bullet" && OtherObject->name != "WeaponPickup" && OtherObject->name != "PowerUpPickup" 
        && OtherObject->name != "CoinPickup")
    {
      //if (explosive_)
      //{
      //  GameObject *temp = (FACTORY->LoadObjectFromArchetype(space, "explosion"));
      //  temp->GetComponent<Transform>(eTransform)->SetTranslation(bt->GetTranslation());
      //}
      space->GetGameObject(owner)->Destroy();
    }


    
	}


}
