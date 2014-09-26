#pragma once

#include "physics\Handle.h"
#include "physics\Shape.h"

#include "SheepPhysics.h"

namespace SheepFizz
{
	class Material;
}

namespace Framework
{

	class Transform;

	class RigidBody: public GameComponent
	{
	
		public:
			//rigid body constructor
			RigidBody(SheepFizz::Shapes shape = SheepFizz::Cir,	//shape - default is circle
			SheepFizz::Material* material = PHYSICS->GetMaterial("Wood"),	//material
			float radius = 1,							//default radius one
			float y = 0,								//no height because it is a circle
			float orientation = 0);						//orientation of 0

			//destructor
			~RigidBody();

			//initialize adds the values from the component
			virtual void Initialize();

			//removes the component
			virtual void Remove();

		private:

			//these values are all temp storage for initialization
			SheepFizz::Material* m_material;
			SheepFizz::Shapes m_shape;
			float m_xradius;
			float m_yval;
			float m_orientation;

			SheepFizz::Handle m_handle;
	};
	
}