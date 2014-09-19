#include "Step.h"

namespace SheepFizz
{

	PhysicsSpace* PhysicsSpace::Allocate(float dt)
	{
		return new PhysicsSpace(dt);
	}

	//body settors
	void PhysicsSpace::SetBodyPos(Handle handle, Vec3D position)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->position_ = position;
	}

	void PhysicsSpace::SetBodyVeloc(Handle handle, Vec3D velocity)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->velocity_ = velocity;
	}
	
	void PhysicsSpace::SetBodyForce(Handle handle, Vec3D force)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->force_ = force;
	}
	
	void PhysicsSpace::SetBodyRot(Handle handle, float rot)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->orientation_ = rot;
	}
	
	void PhysicsSpace::SetBodyAngVeloc(Handle handle, float angveloc)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->angularVelocity_ = angveloc;
	}
	
	void PhysicsSpace::SetBodyTorque(Handle handle, float torque)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->torque_ = torque;
	}
	
	//change the dt
	void PhysicsSpace::SetTime(float dt) {dt_ = dt;}

	//get values for engine
	Vec3D PhysicsSpace::GetBodyPos(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->position_;
	}
	
	Vec3D PhysicsSpace::GetBodyVeloc(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->velocity_;
	}
	
	Vec3D PhysicsSpace::GetBodyForce(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->force_;
	}

	float PhysicsSpace::GetBodyRot(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->orientation_;
	}

	float PhysicsSpace::GetBodyAngVeloc(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->angularVelocity_;
	}

	float PhysicsSpace::GetBodyTorque(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->torque_;
	}

	//get the time for the engine
	float PhysicsSpace::GetTime(void) {return dt_;}

	//add bodies to the body vector
	Handle PhysicsSpace::AddBody(Shapes shape, Material& material, Vec3D position, 
		float xradius, float yval, float orientation)
	{	

		switch(shape)
		{
			case Rec:
				{
					//create the rectangle shape
					Rectangle* rec = (Rectangle*)shapes_[Rec].Allocate();
					new (rec) Rectangle(xradius, yval);
					rec->self = handles_.Insert(rec);
					handles_.SyncHandles<Rectangle>(shapes_[Rec]);
			
					//then add the body
					Body* body = (Body*)bodies_.Allocate();
					new (body) Body(rec, material, position, Vec3D(), Vec3D(), orientation);
					body->self;
					handles_.SyncHandles<Body>(bodies_);

					return body->self;

					break;
				}

			case Cir:
				{
					//create the circle shape
					Circle* cir = (Circle*)shapes_[Cir].Allocate();
					new (cir) Circle(xradius);
					cir->self = handles_.Insert(cir);
					handles_.SyncHandles<Circle>(shapes_[Cir]);
			
					//then add the body
					Body* body = (Body*)bodies_.Allocate();
					new (body) Body(cir, material, position, Vec3D(), Vec3D(), orientation);
					body->self;
					handles_.SyncHandles<Body>(bodies_);

					return body->self;

					break;
				}

			default:
				break;

		}		
	}

	//this function advances the game forward one dt
	void PhysicsSpace::Step(void)
	{
		//iterate through list of bodies
		for(int i = 0; i < bodies_.Size(); ++i)
		{
			//move one body in vector forward to start
			for(int j = i + 1; j < bodies_.Size(); ++j)
			{

				//create a manifold and see if there is any interaction
				Manifold m((Body*)bodies_[i], (Body*)bodies_[j]);
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
		for(int i = 0; i < bodies_.Size(); ++i)
			SymplecticEuler(*((Body*)bodies_[i]));

		//clean up forces so no interference with next loop
		for(int i = 0; i < bodies_.Size(); ++i)
		{
			((Body*)bodies_[i])->force_.x_ = 0.0f;
			((Body*)bodies_[i])->force_.y_ = 0.0f;
			((Body*)bodies_[i])->torque_ = 0.0f;
			((Body*)bodies_[i])->torque_ = 0.0f;
		}

	}//end of Step


	void PhysicsSpace::SymplecticEuler(Body& body)
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