#include "Step.h"

namespace SheepFizz
{

	PhysicsSpace* PhysicsSpace::Allocate(float dt)
	{
		return new PhysicsSpace(dt);
	}//end of Allocate

	void PhysicsSpace::Delete(PhysicsSpace* space)
	{
		delete space;
	}//end of Delete

	//body settors//*************
	void PhysicsSpace::SetBodyPos(Handle handle, Vec3D position)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->position_ = position;
	}//end of SetBodyPos

	void PhysicsSpace::SetBodyVeloc(Handle handle, Vec3D velocity)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->velocity_ = velocity;
	}//end of SetBodyVeloc
	
	void PhysicsSpace::SetBodyForce(Handle handle, Vec3D force)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->force_ = force;
	}//end of SetBodyForce
	
	void PhysicsSpace::SetBodyRot(Handle handle, float rot)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->orientation_ = rot;
	}//end of SetBodyRot
	
	void PhysicsSpace::SetBodyAngVeloc(Handle handle, float angveloc)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->angularVelocity_ = angveloc;
	}//end of SetBodyAngVeloc
	
	void PhysicsSpace::SetBodyTorque(Handle handle, float torque)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->torque_ = torque;
	}//end of SetBodyTorque
	
	//change the dt
	void PhysicsSpace::SetTime(float dt) {dt_ = dt;}
	//end of SetTime
	//*************end of settors


	//get values for engine
	Vec3D PhysicsSpace::GetBodyPos(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->position_;
	}//end of GetBodyPos
	
	Vec3D PhysicsSpace::GetBodyVeloc(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->velocity_;
	}//end of GetBodyVeloc
	
	Vec3D PhysicsSpace::GetBodyForce(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->force_;
	}//end of GetBodyForce

	float PhysicsSpace::GetBodyRot(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->orientation_;
	}//end of GetBodyRot

	float PhysicsSpace::GetBodyAngVeloc(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->angularVelocity_;
	}//end of GetBodyAngVeloc

	float PhysicsSpace::GetBodyTorque(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->torque_;
	}//end of GetBodyTorque

	//get the time for the engine
	float PhysicsSpace::GetTime(void) {return dt_;}
	//end of GetTime
	//*************end of gettors


	//add bodies to the body vector
	Handle PhysicsSpace::AddBody(Shapes shape, Material& material, Vec3D position, 
		float xradius, float yval, float orientation, void* userData)
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
					new (body) Body(rec, material, position, Vec3D(), Vec3D(), userData, orientation);
					body->self = handles_.Insert(body);
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
					new (body) Body(cir, material, position, Vec3D(), Vec3D(), userData, orientation);
					body->self = handles_.Insert(body);
					handles_.SyncHandles<Body>(bodies_);

					return body->self;

					break;
				}

			default:
				return NULL;
				break;

		}		
	}//end of AddBody
	//*************

	//change bodies
	void PhysicsSpace::ChangeBodies(Handle handle, float xradius, float y)
	{
		Body* body = (Body*)handles_.Get(handle);
		body->ChangeBody(xradius, y);
	}//end of ChangeBodies

	//change materials
	void PhysicsSpace::ChangeMaterials(Handle handle, Material& material)
	{
		Body* body = (Body*)handles_.Get(handle);
		body->ChangeMaterial(material);
	}//end of ChangeMaterials

	//remove bodies
	void PhysicsSpace::RemoveBody(Handle handle)
	{
		//find body point from handle, then identify shape handle and pointer
		Body* body = (Body*)handles_.Get(handle);
		Shape* shape = (Shape*)handles_.Get(body->shape_->self);
		
		//free shape and release handle
		Shape* shapeRemoved = (Shape*)shapes_[body->shape_->GetShape()].Free(shape);
		if(shapeRemoved)
			handles_.Update(shapeRemoved, shapeRemoved->self);
		
		//free body and release handle
		Body* bodyRemoved = (Body*)bodies_.Free(body);
		if(bodyRemoved)
			handles_.Update(bodyRemoved, bodyRemoved->self);
	}//end of RemoveBody
	//*************


	//this function advances the game forward one dt
	void PhysicsSpace::Step(void)
	{
		//iterate through list of bodies
		for(unsigned i = 0; i < bodies_.Size(); ++i)
		{
			//move one body in vector forward to start
			for(unsigned j = i + 1; j < bodies_.Size(); ++j)
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
		for(unsigned i = 0; i < manifolds_.size(); ++i)
		{
			manifolds_[i].PositionalCorrection();
			manifolds_[i].ApplyForces();
		}

		//apply forces and velocity to all bodies
		for(unsigned i = 0; i < bodies_.Size(); ++i)
			SymplecticEuler(*((Body*)bodies_[i]));

		//clean up forces so no interference with next loop
		for(unsigned i = 0; i < bodies_.Size(); ++i)
		{
			((Body*)bodies_[i])->force_.x_ = 0.0f;
			((Body*)bodies_[i])->force_.y_ = 0.0f;
			((Body*)bodies_[i])->torque_ = 0.0f;
			((Body*)bodies_[i])->torque_ = 0.0f;
		}

		//send manifold data back to engine for game logic
		/*for(int i = 0; i < manifolds_.size(); ++i)
		{
			cb_(manifolds_[i].A->userData, manifolds_[i].B->userData, userData_);
		}*/

		//empty manifold list;
		manifolds_.clear();

	}//end of Step
	//*************

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
			+ Vec3D(0,GRAVITY) * (float)body.gravityScale_ * (float)body.gravityOn_) * dt_;

		//Position changed last
		body.position_ += body.velocity_ * dt_;

	}//end of SymplecticEuler
	//*************

	//Engine functionality
	void PhysicsSpace::SetUserData(void* userData)
	{
		userData_ = userData;
	}//end of SetUserData

	void PhysicsSpace::SetCollisionCallback(CollisionCB cb)
	{
		cb_ = cb;
	}//end of SetCollisionCallback
}