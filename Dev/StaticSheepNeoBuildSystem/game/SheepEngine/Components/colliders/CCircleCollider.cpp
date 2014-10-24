#include "pch/precompiled.h"
#include "components/rigidbody/CRigidBody.h"
#include "CCircleCollider.h"

namespace Framework
{
	CircleCollider::CircleCollider()
		:RigidBody(SheepFizz::Cir)
	{

	}

	CircleCollider::~CircleCollider()
	{

	}

	void CircleCollider::Initialize()
	{
		RigidBody::Initialize();
	}

	void CircleCollider::Remove()
	{
		RigidBody::Remove();
	}
}