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
		virtual void PropertyCollision(unsigned int specifier) {}
		virtual void PropertyResolution() {}
	};

	class StickyProperty: public BodyProperties
	{
		public:
			virtual void PropertyCollision(unsigned int specifier);
			virtual void PropertyResolution();

		private:
			unsigned int stickyOn;		//whether to stick
			unsigned int stickySide;
	};

}