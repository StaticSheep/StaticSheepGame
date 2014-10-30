#pragma once

#ifdef SHEEPPHYSICS
#include "Manifold.h"
#endif

#include <vector>
#include <unordered_map>
#include <utility>
#include <array>
#include "Material.h"
#include "Shape.h"
#include "Vec3D.h"

namespace SheepFizz
{
	//collision callback function pointer
  PHY_API typedef void* ExternalManifold;
  PHY_API typedef void(*CollisionCB)(void*, void*, void*, ExternalManifold);
 

	#define GRAVITY -100.0f

	class PhysicsSpace
	{
		public:
		PHY_API static PhysicsSpace* Allocate(float dt, float meterScale);
		PHY_API static void Delete(PhysicsSpace* space);


    //engine only functions, not exposed
    
		//body settors
		PHY_API void SetBodyPos(Handle handle, Vec3D position);
		PHY_API void SetBodyVeloc(Handle handle, Vec3D velocity);
		PHY_API void SetBodyForce(Handle handle, Vec3D force);
		PHY_API void SetBodyRot(Handle handle, float rot);
		PHY_API void SetBodyAngVeloc(Handle handle, float angveloc);
		PHY_API void SetBodyTorque(Handle handle, float torque);

    //set collision groups
    PHY_API void SetCollisionString(Handle handle, std::string value);
    PHY_API void SetBodyCollisionCallback(Handle handle, bool collisionCallback);

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

    //gravity gettors
    PHY_API Vec3D GetBodyGravityNormal(Handle handle);

		//get the time for the engine
		PHY_API float GetTime(void);

    //collision functions
    PHY_API Vec3D GetCollisionNorm(void* handle, ExternalManifold manifold);
    PHY_API std::string GetCollisionString(Handle handle);

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

      //tracks the manifolds that run the system
			std::vector<Manifold> manifolds_;

      //used for collision groups
      std::unordered_map< std::string, unsigned int> collisionGroups_;
      
		#endif

	};

}
