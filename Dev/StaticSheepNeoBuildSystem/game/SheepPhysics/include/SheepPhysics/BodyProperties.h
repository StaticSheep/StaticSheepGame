/*****************************************************************
Filename: BodyProperties.h
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
	};

	class StickyProperty: public BodyProperties
	{
		public:

		private:
			unsigned int stickOn;		//whether to stick
			unsigned int stickySide;    //what side to stick with
	};

}
