/*****************************************************************
Filename: CCoinBall.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CCoinBall.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"
#include "../Weapon_Scripts/CBullet_default.h"

namespace Framework
{
  CoinBall::CoinBall()
    :hp(60), lasthp(hp), alive(true)
	{
	}

  CoinBall::~CoinBall()
	{

	}

  static Transform *pt;
  static CircleCollider *pc;

  void CoinBall::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(CoinBall::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(CoinBall::OnCollision));

    cbTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    cbCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);
    pc = space->GetHandles().GetAs <CircleCollider>(cbCollider);
    pc->SetGravityOff();
    Vec3 startingVel((float)GetRandom(-100, 100), (float)GetRandom(-100, 100), 0.0f);
    pc->SetVelocity(startingVel);
	}

  void CoinBall::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void CoinBall::LogicUpdate(float dt)
	{
    pt = space->GetHandles().GetAs<Transform>(cbTransfrom);
    pc = space->GetHandles().GetAs <CircleCollider>(cbCollider);

    if (hp < lasthp)
    {
      space->hooks.Call("SpawnCoinsEx", pt->GetTranslation(),
        (lasthp - hp) / 12);

      lasthp = hp;
      if (hp < 0)
        alive = false;
    }
    if (!alive)
      DestroyBall();

    if (pt->GetTranslation().z > 0)
      pt->SetTranslation(pt->GetTranslation() + Vec3(0.0f, 0.0f, -2.0f));

    if (pt->GetTranslation().x > 1010 || pt->GetTranslation().x < -1010 || pt->GetTranslation().y > 600 || pt->GetTranslation().y < -600)
      space->GetGameObject(owner)->Destroy();

	}

  void CoinBall::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
  {
    pt = space->GetHandles().GetAs<Transform>(cbTransfrom);
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    Bullet_Default* bd = OtherObject->GetComponent<Bullet_Default>(eBullet_Default);



    if (bd)
    {
      hp -= bd->damage;
    }
    
  }

  void CoinBall::DestroyBall()
  {
    space->hooks.Call("SpawnCoinsEx", pt->GetTranslation(),
      10);

    space->GetGameObject(owner)->Destroy();
  }

}
