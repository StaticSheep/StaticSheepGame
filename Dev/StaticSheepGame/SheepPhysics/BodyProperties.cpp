#pragma once
#include "BodyProperties.h"

namespace SheepFizz
{
	//determine which side player collided on and set flag
	void StickyProperty::PropertyCollision(unsigned int rectangleSide)
	{
		if(rectangleSide == 2)
			playerFace = 1;

		else
			playerFace = 0;
	}

	void StickyProperty::PropertyResolution(Vec3D& normal)
	{
		if(playerFace)
		{
			ActivateGravity();
			SetGravityNormal(normal);
		}
	}
}
