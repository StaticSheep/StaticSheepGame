/*****************************************************************
Filename: SheepPhysics.h
Project: 
Author(s): Zachary Nawar, Jon Sourbeer (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace SheepFizz
{
	using namespace SheepMath;
}

#include "physics\api.h"
#include "physics\Handle.h"
#include "physics\Step.h"

#include "System.h"



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

		void Shutdown(void);

	private:
		//void PhysicsStep(float dt);
		SheepFizz::PhysicsSpace* m_space;

	};

	extern SheepPhysics* PHYSICS;
}