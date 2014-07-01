/*****************************************************************
Filename: SheepPhysics.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepPhysics.h"

namespace Framework
{
	SheepPhysics* PHYSICS = NULL;

	SheepPhysics::SheepPhysics()
	{
		// Initialize physics object list
		PHYSICS = this;
	}

	SheepPhysics::~SheepPhysics()
	{
    PHYSICS = nullptr;
	}

  void SheepPhysics::RegisterComponents()
  {
    REGISTER_COMPONENT(Transform);
    REGISTER_COMPONENT(Tester);
  }

	void SheepPhysics::Initialize()
	{

	}

	void SheepPhysics::Update(float dt)
	{

	}


}