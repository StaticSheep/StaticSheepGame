#pragma once

#ifdef DLL_PHYEXPORT
#include "Manifold.h"
#endif

#include "Material.h"
#include "Shape.h"
#include "Vec3.h"
#include <vector>



namespace SheepFizz
{

	#define GRAVITY 10.0f

	class PhysicsSpace
	{
		public:
		PHY_API static PhysicsSpace* Allocate(float dt);


		//body settors
		PHY_API void SetBodyPos(Handle handle, SheepMath::Vec3D position);
		PHY_API void SetBodyVeloc(Handle handle, SheepMath::Vec3D velocity);
		PHY_API void SetBodyForce(Handle handle, SheepMath::Vec3D force);
		PHY_API void SetBodyRot(Handle handle, float rot);
		PHY_API void SetBodyAngVeloc(Handle handle, float angveloc);
		PHY_API void SetBodyTorque(Handle handle, float torque);

		//change the dt
		PHY_API void SetTime(float dt);

		//get values for engine
		PHY_API Vec3D GetBodyPos(Handle handle);
		PHY_API Vec3D GetBodyVeloc(Handle handle);
		PHY_API Vec3D GetBodyForce(Handle handle);
		PHY_API float GetBodyRot(Handle handle);
		PHY_API float GetBodyAngVeloc(Handle handle);
		PHY_API float GetBodyTorque(Handle handle);

		//get the time for the engine
		PHY_API float GetTime(void);

		//add bodies to the body vector
		PHY_API void AddBody(Shapes shape, Material& material, SheepMath::Vec3D position, float xradius, float yval = 0);

		//perform the timestep - perform all comparisons
		//and resolve all forces
		//applied in physics update
		PHY_API void Step(void);

		//application of forces and velocities to each body
		//applied in physics update

		#ifdef DLL_PHYEXPORT

		private:
			//timestep value
			float dt_;

			//vector of bodies in engine
			ObjectAllocator bodies_;
			ObjectAllocator shapes_[Count];
			HandleManager handles_;

			std::vector<Manifold> manifolds_;

		#endif

	};

}
