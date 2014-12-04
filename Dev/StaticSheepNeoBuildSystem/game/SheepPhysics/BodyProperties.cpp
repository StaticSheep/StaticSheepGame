/*****************************************************************
Filename: BodyProperties.cpp
Project:
Author(s): Jon Sourbeer (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "precompiled.h"
#include "BodyProperties.h"

namespace SheepFizz
{
	//determine which side player collided on and set flag
	void StickyProperty::PropertyCollision(unsigned int rectangleSide)
	{
		if (rectangleSide == stickySide)
			stickOn = 1;

		else
			stickOn = 0;
	}

	void StickyProperty::PropertyResolution(Framework::Vec3D& normal)
	{
		if (stickOn)
		{
      Framework::Vec3D norm;
      norm = normal;
      norm = norm;
			//ActivateGravity();
			//SetGravityNormal(normal);
		}
	}
}