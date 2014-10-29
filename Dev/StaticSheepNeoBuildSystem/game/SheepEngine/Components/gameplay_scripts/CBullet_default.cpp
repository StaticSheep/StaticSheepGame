#include "pch/precompiled.h"
#include "CBullet_default.h"
#include "types/space/Space.h"

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
	}

	void Bullet_Default::Remove()
	{

	}

	void Bullet_Default::LogicUpdate(float dt)
	{

	}

	void Bullet_Default::OnCollision(Handle otherObject)
	{

	}


}
