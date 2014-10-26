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