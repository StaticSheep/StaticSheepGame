/*****************************************************************
Filename: SheepPhysics.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"
#include "PhysicsObject.h"
#include "SlotArray.h"

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

    void RegisterComponents();

		// Initialization call
		void Initialize(void);

		// Update per frame
		void Update(float dt);

		PhysicsObject* RegisterObject(PhysicsObject& object);

		SlotArray<PhysicsObject> m_objectList;

	private:
		void PhysicsStep(float dt);

	};

	extern SheepPhysics* PHYSICS;
}