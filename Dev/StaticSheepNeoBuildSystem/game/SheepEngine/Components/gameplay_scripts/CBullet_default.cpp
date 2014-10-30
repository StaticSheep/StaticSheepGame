#include "pch/precompiled.h"
#include "CBullet_default.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"

namespace Framework
{
	Bullet_Default::Bullet_Default()
	{

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
	}

	void Bullet_Default::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

	void Bullet_Default::LogicUpdate(float dt)
	{
    Transform *bt = space->GetHandles().GetAs<Transform>(bTransfrom);

    if (bt->GetTranslation().x > 700 || bt->GetTranslation().x < -700 || bt->GetTranslation().y > 700 || bt->GetTranslation().y < -700)
      space->GetGameObject(owner)->Destroy();
	}

  void Bullet_Default::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
		space->GetGameObject(owner)->Destroy();
	}


}
