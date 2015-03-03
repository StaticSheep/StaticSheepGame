/*****************************************************************
Filename: CCoinBall.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CCoinBall.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CCircleCollider.h"

namespace Framework
{
  CoinBall::CoinBall()
	{
    hits = 0;
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
    hits = 0;
    beenHit = false;
    alive = true;
	}

  void CoinBall::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void CoinBall::LogicUpdate(float dt)
	{
    pt = space->GetHandles().GetAs<Transform>(cbTransfrom);
    pc = space->GetHandles().GetAs <CircleCollider>(cbCollider);
    if (beenHit)
    {
      beenHit = false;
      space->hooks.Call("SpawnCoins", pt->GetTranslation());
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
    if (OtherObject->name == "Bullet")
    {
      ++hits;
      beenHit = true;
      if (hits > 4)
        alive = false;
      Handle pCollider = space->GetGameObject(otherObject)->GetComponentHandle(eCircleCollider);
      CircleCollider *bullCollider = space->GetHandles().GetAs<CircleCollider>(pCollider);
      Vec3 BullDir = bullCollider->GetCurrentVelocity();

      pc->AddToVelocity(BullDir);
      NormalizeSpeed();
    }
    
  }

  void CoinBall::NormalizeSpeed()
  {
    int baseSpeed = 200;
    Vec3 unitVector = pc->GetCurrentVelocity().Normalize();
    Vec3 newVector = unitVector * (baseSpeed + (baseSpeed * hits));
    pc->SetVelocity(newVector);
  }

  void CoinBall::DestroyBall()
  {
    for (int i = 0; i < 5; ++i)
      space->hooks.Call("SpawnCoins", pt->GetTranslation());

    space->GetGameObject(owner)->Destroy();
  }

}
