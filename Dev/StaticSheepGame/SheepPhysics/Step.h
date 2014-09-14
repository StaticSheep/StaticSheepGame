#include "Manifold.h"
#include <vector>

namespace PHYSICS
{

#define GRAVITY 10.0f

struct TimeStep
{
	TimeStep(float dt) : dt_(dt) {}
	
	~TimeStep()
	{
		//remove all dynamic allocations
		for(int i = 0; i < bodies_.size(); ++i)
		{
			delete bodies_[i]->shape_;
			delete bodies_[i];
		}
	}

	//add bodies to the body vector
	void AddBody(Shapes shape, Material& material, Vec3D position, float xradius, float yval = 0)
	{	
		Body* body;

		switch(shape)
		{
			case Rec:
				{
				Rectangle* rec = new Rectangle(xradius, yval);
				body = new Body(rec, material, position, Vec3D(), Vec3D(), 0);
				bodies_.push_back(body);
				break;
				}

			case Cir:
				{
				Circle* cir = new Circle(xradius);
				body = new Body(cir, material, position, Vec3D(), Vec3D());
				bodies_.push_back(body);
				break;
				}

			default:
				break;

		}		
	}

	//perform the timestep - perform all comparisons
	//and resolve all forces
	void Step(void);

	//application of forces and velocities to each body
	void SymplecticEuler(Body& body);

	//timestep value
	float dt_;

	//vector of bodies in engine
	std::vector<Body*> bodies_;
	std::vector<Manifold*> manifolds_;

};

}