/*****************************************************************
Filename: CCircleCollider.cpp
Project:
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

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