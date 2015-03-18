/*****************************************************************
Filename: Step.h
*****************************************************************/

#pragma once

#ifdef SHEEPPHYSICS
#include "Manifold.h"
#include "RayCast.h"
#endif

#include "CollisionGroups.h"
#include "Material.h"
#include "Shape.h"
#include "Vec3D.h"
#include "RayConfig.h"
#include <vector>


namespace SheepFizz
{
	//collision callback function pointer
  PHY_API typedef void* ExternalManifold;
  PHY_API typedef void(*CollisionCB)(void*, void*, void*, ExternalManifold);
 

	#define GRAVITY 0.0f
  #define MAX_ITERATIONS 10
  #define SPEEDLIMIT 100

	class PhysicsSpace
	{
		public:
		PHY_API static PhysicsSpace* Allocate(float dt, float meterScale);
		PHY_API static void Delete(PhysicsSpace* space);


		//body settors
		PHY_API void SetBodyPos(Handle handle, Vec3D position);
		PHY_API void SetBodyVeloc(Handle handle, Vec3D velocity);
		PHY_API void SetBodyForce(Handle handle, Vec3D force);
		PHY_API void SetBodyRot(Handle handle, float rot);
    PHY_API void SetBodyRot(Handle handle, Vec3D direction);
		PHY_API void SetBodyAngVeloc(Handle handle, float angveloc);
		PHY_API void SetBodyTorque(Handle handle, float torque);
    PHY_API void SetBodyCollisionGroup(Handle handle, CollisionGroup group);

    //gravity
    PHY_API void SetBodyGravityOn(Handle handle);
    PHY_API void SetBodyGravityOff(Handle handle);
    PHY_API void SetBodyGravityNormal(Handle handle, Vec3D normal);

		//adders
		PHY_API void AddToBodyVeloc(Handle handle, Vec3D velocity);
		PHY_API void AddToBodyForce(Handle handle, Vec3D force);
		PHY_API void AddToBodyAngVeloc(Handle handle, float angveloc);
		PHY_API void AddToBodyTorque(Handle handle, float torque);

		//change the dt
		PHY_API void SetTime(float dt);

		//get values for engine
		PHY_API Vec3D GetBodyPos(Handle handle);
		PHY_API Vec3D GetBodyVeloc(Handle handle);
		PHY_API Vec3D GetBodyForce(Handle handle);
		PHY_API float GetBodyRot(Handle handle);
		PHY_API float GetBodyAngVeloc(Handle handle);
		PHY_API float GetBodyTorque(Handle handle);

    PHY_API Vec3D GetBodyUpNormal(Handle handle);
    PHY_API CollisionGroup GetBodyCollisionGroup(Handle handle);

    //gravity gettors
    PHY_API Vec3D GetBodyGravityNormal(Handle handle);

		//get the time for the engine
		PHY_API float GetTime(void);

    //collision functions
    PHY_API void SetBodyCollisionCallback(Handle handle, bool collisionCallback);
    PHY_API Vec3D GetCollisionNorm(void* handle, ExternalManifold manifold);
    PHY_API Vec3D GetCollisionPoint(ExternalManifold manifold);

    //shape functions
    PHY_API unsigned int GetBodyVertexNumber(Handle handle);
    PHY_API Vec3D GetBodyVertex(Handle handle, unsigned int vertex);

		//add bodies to the body vector
    PHY_API Handle AddBody(
      Shapes shape,				        //shape of the object
      Material& material,			    //the material ref
      bool collisionCallback,     //determines if collision callback occurs
      Framework::Vec3D position,	//the position of the transform
      float xradius,				      //the radius of circle or width
      float yval = 0,				      //the height - if a rec
      float orientation = 0,		  //the orientation
			void* userData = NULL);		

    //raycasting
    PHY_API bool RayCaster(RayConfig* ray);

		//change a specific body's attributes
		PHY_API void ChangeBodies(Handle handle, float xradius, float y = 0);
		PHY_API void ChangeMaterials(Handle handle, Material& material);

		//remove bodies and their shapes from the vector
		PHY_API void RemoveBody(Handle handle);
	
		//perform the timestep - perform all comparisons
		//and resolve all forces
		//applied in physics update
		PHY_API void Step(void);

		//application of forces and velocities to each body
		//applied in physics update

		//engine functions for messaging
    PHY_API void SetCollisionCallback(CollisionCB cb);
		PHY_API void SetUserData(void* userData);

    //debug
    PHY_API unsigned int PhysicsSpace::GetDebugBodyNumber(void);
    PHY_API unsigned int PhysicsSpace::GetDebugManifoldNumber(void);

		#ifdef SHEEPPHYSICS

		private:
			//timestep value
			float dt_;

      //scale value
      float meterScale_;
      float modifiedGravity_;

      bool locked_ = false;

			//values tied to engine messaging
			CollisionCB cb_;	//callback function pointer
			void* userData_;	//pointer to engine space

			//vector of bodies in engine
			ObjectAllocator bodies_;
			ObjectAllocator shapes_[Count];
			HandleManager handles_;

			std::vector<Manifold> manifolds_;

      //raycast is constructed once
      RayCast rayCast_;

      //debug info
      unsigned int manifoldNumber_;

		#endif

	};

}
