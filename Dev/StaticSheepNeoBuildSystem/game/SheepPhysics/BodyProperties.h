/*****************************************************************
Filename: BodyProperties.h
Project:
Author(s): Jon Sourbeer (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "precompiled.h"
#include "Vec3D.h"

namespace SheepFizz
{

	enum BodyProperty{

		Sticky,
		GravityWell,
		BodyPropertiesLength
	};

	class BodyProperties
	{
	public:
    virtual void PropertyCollision(unsigned int specifier);
    virtual void PropertyResolution(Framework::Vec3D& normal);
	};

	class StickyProperty: public BodyProperties
	{
		public:
			virtual void PropertyCollision(unsigned int specifier);
			virtual void PropertyResolution(Framework::Vec3D& normal);

		private:
			unsigned int stickOn;		//whether to stick
			unsigned int stickySide;    //what side to stick with
	};

}