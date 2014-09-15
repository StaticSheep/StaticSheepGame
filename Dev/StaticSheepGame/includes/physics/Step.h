#include "Manifold.h"
#include <vector>

namespace PHYSICS
{

#define GRAVITY 10.0f

struct TimeStep


	//perform the timestep - perform all comparisons
	//and resolve all forces

	//application of forces and velocities to each body

	//timestep value
	float dt_;

	//vector of bodies in engine
	std::vector<Body*> bodies_;
	std::vector<Manifold*> manifolds_;

};

}
