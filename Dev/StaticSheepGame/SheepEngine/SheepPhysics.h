/*****************************************************************
Filename: SheepPhysics.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"
#include "PhysicsObject.h"

namespace Framework
{
	class SheepPhysics : public ISystem
	{
	public:
		// Sheep graphics interface constructor
		SheepPhysics();
		// Sheep graphics interface deconstructor
		~SheepPhysics();

		virtual std::string GetName() {return "SheepPhysics";};

		// Initialization call
		void Initialize(void);

		// Update per frame
		void Update(float dt);

	private:
		void PhysicsStep(float dt);

		void RegisterObject(PhysicsObject& object);


	};

	extern SheepPhysics* PHYSICS;
}