/*****************************************************************
Filename: SheepPhysics.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Handle.h"

namespace Framework
{
	class PhysicsObject
	{
	public:
		int* test1;
		int* test2;
		// Translation
		// Rotation
		// Scale
		Handle m_handle;

		PhysicsObject& operator=(PhysicsObject rhs);
		PhysicsObject();
		PhysicsObject(int* arg1, int* arg2);
	private:
		static int generic;

		
	};

	
}