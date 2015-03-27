/*****************************************************************
Filename: CRigidBody.h
Project:
Author(s): Jon Sourbeer(primary), Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

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

      //forces and velocities
			virtual void SetVelocity(Vec3D& velocity);
      virtual void SetAngVelocity(float angularvelocity);
			virtual void AddToVelocity(Vec3D& velocity);
			virtual void AddToForce(Vec3D& force);
			virtual void AddToAngVelocity(float angularvelocity);
			virtual void AddToTorque(float torque);
     
      //gravity
      virtual void SetGravityOn(void);
      virtual void SetGravityOff(void);
      virtual void SetGravityNormal(Vec3D);

      //collision
      virtual void SetBodyCollisionCallback(bool collisionCallback);
      virtual void SetBodyCollisionGroup(std::string group);

      virtual Vec3D GetCollisionNormals(SheepFizz::ExternalManifold manifold);
      virtual Vec3D GetCollisionPoint(SheepFizz::ExternalManifold manifold);
      virtual std::string GetBodyCollisionGroup(void);

      //raycasting
      virtual void SetRayCast(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name);
      virtual bool SimpleRayCast();
      virtual bool ComplexRayCast();
      virtual void RayDestruction();

      //settors
      virtual void SetBodyRotation(Vec3D direction);
      virtual void SetBodyRotation(float direction);
      virtual void SetBodyRadius(float radius);
      virtual void SetBodyPosition(Vec3D& position);
      virtual void SetBodyFrictionMod(float frictionMod);

      //gettors
      virtual Vec3D GetCurrentVelocity(void);
      virtual Vec3D GetGravityNormal(void);
      virtual Vec3D GetBodyPosition(void);
      virtual Vec3D GetBodyUpNormal(void);
      virtual Vec3D GetBodyRotationAsVector(void);
      virtual float GetBodyRotation(void);
      virtual float GetBodyAngVelocity(void);

      //debug
      virtual unsigned int GetBodyVertexNumber(void);
      virtual Vec3D GetBodyVertex(unsigned int vertex);
      void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
      

      virtual void UpdateWidth(const void* value);
      virtual void UpdateHeight(const void* value);

		// Properties
		union
		{
		float m_width;
		float m_radius;
		};
      
		float m_height; // For boxes

		std::string m_materialName;

    bool m_hasCollisionCallback;

    std::string m_CollisionGroup;

    //debug - stores normal as first index, collision point as second
    //normals_[0] - normal, normals_[1] - point, normals_[2] - normal, etc.
    std::vector<Vec3D> normals_;

		protected:

			//these values are all temp storage for initialization
			SheepFizz::Material* m_material;
      
			// Shape of the object
			SheepFizz::Shapes m_shape; 
			
			// Handle to created physics
      SheepFizz::Handle m_handle = SheepFizz::Handle::null;

	};
	
}