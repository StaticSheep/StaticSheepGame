#pragma once
#include "precompiled.h"

namespace SheepFizz
{

	enum BodyProperties{

		Sticky,
		GravityWell,
		BodyPropertiesLength
	};

	class BodyProperties 
	{
		public:
			virtual void PropertyCollision(unsigned int specifier) {}
			virtual void PropertyResolution(Vec3D& vector) {}
	};

	class StickyProperty
	{
		public:
			virtual void PropertyCollision(unsigned int specifier);
			virtual void PropertyResolution(Vec3D& vector);

		private:
			unsigned int stickySide;
	};

}