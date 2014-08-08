/*****************************************************************
Filename: SheepPhysics.h
Project: 

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
		// Sheep graphics interface deconstructor



		// Initialization call

		// Update per frame


		SlotArray<PhysicsObject> m_objectList;

	private:

	};

	extern SheepPhysics* PHYSICS;
}
