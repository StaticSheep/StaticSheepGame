/*****************************************************************
Filename: CCircleCollider.h
Project:
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "components/rigidbody/CRigidBody.h"

namespace Framework
{

	class CircleCollider : public RigidBody
	{
		public:
		// Default constructor
		CircleCollider();

		// Deconstructor
		~CircleCollider();

		// Initialization routine
		void Initialize();

		// Removal function
		void Remove();

		void SetVelocity(Vec3D& velocity)
		{
		  RigidBody::SetVelocity(velocity);
		}
	};
}