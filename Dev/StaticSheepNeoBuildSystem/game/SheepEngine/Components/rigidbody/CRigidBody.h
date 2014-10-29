#pragma once

#include "SheepPhysics\Handle.h"
#include "SheepPhysics\Shape.h"

#include "systems/physics/SheepPhysics.h"

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
			
			//RigidBody(); // Default Constructor

			RigidBody(SheepFizz::Shapes shape);

			//destructor
			~RigidBody();

			//initialize adds the values from the component
			virtual void Initialize();

			//removes the component
			virtual void Remove();

			virtual void SetVelocity(Vec3D& velocity);
      virtual void SetAngVelocity(float angularvelocity);
      void SetBodyCollisionCallback(bool collisionCallback);
			virtual void AddToVelocity(Vec3D& velocity);
			virtual void AddToForce(Vec3D& force);
			virtual void AddToAngVelocity(float angularvelocity);
			virtual void AddToTorque(float torque);

		// Properties
		union
		{
		float m_width;
		float m_radius;
		};
      
		float m_height; // For boxes

		std::string m_materialName;

    bool m_hasCollisionCallback;

		protected:

			//these values are all temp storage for initialization
			SheepFizz::Material* m_material;
      
			// Shape of the object
			SheepFizz::Shapes m_shape; 
			
			// Handle to created physics
			SheepFizz::Handle m_handle;

	};
	
}