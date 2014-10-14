#pragma once

#include "CRigidBody.h"

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

		void SetVelocity()
		{
		  RigidBody::SetVelocity();
		}
	};
}