#include "Step.h"

namespace PHYSICS
{

//this function advances the game forward one dt
void TimeStep::Step(void)
{
	//iterate through list of bodies
	for(int i = 0; i < bodies_.size(); ++i)
	{
		//move one body in vector forward to start
		for(int j = i + 1; j < bodies_.size(); ++j)
		{

			//create a manifold and see if there is any interaction
			Manifold m(bodies_[i], bodies_[j]);
			m.Initialize();
			m.ManifoldInteraction();

			//if there is an interaction, add the manifold to the list
			//to iterate through
			if(m.contactCount > 0)
				manifolds_.push_back(m);
		}
	}

	//apply forces for all manifolds - positional correction first
	for(int i = 0; i < manifolds_.size(); ++i)
	{
		manifolds_[i].PositionalCorrection();
		manifolds_[i].ApplyForces();
		
	}

	//empty manifold list;
	manifolds_.clear();

	//apply forces and velocity to all bodies
	for(int i = 0; i < bodies_.size(); ++i)
		SymplecticEuler(*(bodies_[i]));

	//clean up forces so no interference with next loop
	for(int i = 0; i < bodies_.size(); ++i)
	{
		bodies_[i]->force_.x_ = 0.0f;
		bodies_[i]->force_.y_ = 0.0f;
		bodies_[i]->torque_ = 0.0f;
		bodies_[i]->torque_ = 0.0f;
	}

}//end of Step


void TimeStep::SymplecticEuler(Body& body)
{
	if(body.massData_.mass == 0)
		return;

	//calculate the angular velocity
	body.angularVelocity_ += (body.torque_ * body.massData_.inverseInertia) * dt_;

	//then determine the orientation of the object
	body.orientation_ += body.angularVelocity_ * dt_;

	//Velocity can be calculated after adjusting force
	body.velocity_ += ((body.force_ *= body.massData_.inverseMass) 
		+ Vec3D(0,GRAVITY) * body.gravityScale_ * body.gravityOn_) * dt_;

	//Position changed last
	body.position_ += body.velocity_ * dt_;

}//end of SymplecticEuler

}