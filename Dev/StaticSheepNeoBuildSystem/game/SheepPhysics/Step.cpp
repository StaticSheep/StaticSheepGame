
#include "precompiled.h"

#include "Manifold.h"
#include "Handle.h"
#include "Body.h"
#include "Step.h"

namespace SheepFizz
{

	PhysicsSpace* PhysicsSpace::Allocate(float dt, float meterScale)
	{
		return new PhysicsSpace(dt, meterScale);
	}//end of Allocate

	void PhysicsSpace::Delete(PhysicsSpace* space)
	{
		delete space;
	}//end of Delete

	//body settors
	//*************
	void PhysicsSpace::SetBodyPos(Handle handle, Vec3D position)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->position_ = position / meterScale_;
	}//end of SetBodyPos

	void PhysicsSpace::SetBodyVeloc(Handle handle, Vec3D velocity)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->velocity_ = velocity / meterScale_;
	}//end of SetBodyVeloc
	
	void PhysicsSpace::SetBodyForce(Handle handle, Vec3D force)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->force_ = force / meterScale_;
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

  void PhysicsSpace::SetBodyCollisionCallback(Handle handle, bool collisionCallback)
  {
    Body* body = handles_.GetAs<Body>(handle);
    body->collisionCallback_ = collisionCallback;
  }//end of SetBodyCollisionCallback

  //gravity functions
  void PhysicsSpace::SetBodyGravityOn(Handle handle)
  {
    Body* body = handles_.GetAs<Body>(handle);
    body->gravityOn_ = 1;
  }//end of SetBodyGravityOn

  void PhysicsSpace::SetBodyGravityOff(Handle handle)
  {
    Body* body = handles_.GetAs<Body>(handle);
    body->gravityOn_ = 0;
  }//end of SetBodyGravityOff

  void PhysicsSpace::SetBodyGravityNormal(Handle handle, Vec3D normal)
  {
    Body* body = handles_.GetAs<Body>(handle);
    body->gravityNormal_ = normal;
  }//end of SetBodyGravityNormal

	//end of settors
	//*************


	//adders
	//*************
	void PhysicsSpace::AddToBodyVeloc(Handle handle, Vec3D velocity)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->velocity_ += velocity / meterScale_;
	}//end of AddToBodyVeloc

	void PhysicsSpace::AddToBodyForce(Handle handle, Vec3D force)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->force_ += force / meterScale_;
	}//end of AddToBodyForce

	void PhysicsSpace::AddToBodyAngVeloc(Handle handle, float angveloc)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->angularVelocity_ += angveloc;
	}//end of AddToBodyAngVeloc

	void PhysicsSpace::AddToBodyTorque(Handle handle, float torque)
	{
		Body* body = handles_.GetAs<Body>(handle);
		body->torque_ += torque;
	}//end of AddToBodyTorque

	//end of adders
	//*************

	//get values for engine
	Vec3D PhysicsSpace::GetBodyPos(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->position_ * meterScale_;
	}//end of GetBodyPos
	
	Vec3D PhysicsSpace::GetBodyVeloc(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->velocity_ * meterScale_;
	}//end of GetBodyVeloc
	
	Vec3D PhysicsSpace::GetBodyForce(Handle handle)
	{
		Body* body = handles_.GetAs<Body>(handle);
		return body->force_ * meterScale_;
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

  Vec3D PhysicsSpace::GetBodyGravityNormal(Handle handle)
  {
    Body* body = handles_.GetAs<Body>(handle);
    return body->gravityNormal_;
  }//end of GetBodyGravityNormal

	//get the time for the engine
	float PhysicsSpace::GetTime(void) {return dt_;}

  Vec3D PhysicsSpace::GetCollisionNorm(void* handle, ExternalManifold manifold)
  {
    if (handle == ((Manifold*)manifold)->A->userData)
      return -((Manifold*)manifold)->normal;

    if (handle == ((Manifold*)manifold)->B->userData)
      return (((Manifold*)manifold)->normal);

    return Vec3D();

  }//end of GetCollisionNormal

	//*************end of gettors


	//add bodies to the body vector
  Handle PhysicsSpace::AddBody(Shapes shape, Material& material, bool collisionCallback, Vec3D position,
		float xradius, float yval, float orientation, void* userData)
	{
    
    if (locked_)
      return Handle::null;

		switch(shape)
		{
    case Rec: {
                //create the rectangle shape
                Rectangle* rec = new Rectangle(xradius / meterScale_, yval / meterScale_);
                // (Rectangle*)shapes_[Rec].Allocate();
                //new (rec) Rectangle(xradius / meterScale_, yval / meterScale_);
                rec->self = handles_.Insert(rec);
                handles_.SyncHandles<Rectangle>(shapes_[Rec]);

                //then add the body
                Body* body = (Body*)bodies_.Allocate();
                new (body)Body(rec, material, collisionCallback, position / meterScale_, Vec3D(), Vec3D(), userData, orientation);
                body->self = handles_.Insert(body);
                handles_.SyncHandles<Body>(bodies_);

                return body->self;
        }
        break;

    case Cir: {
                //create the circle shape
                Circle* cir = new Circle(xradius / meterScale_);
                //Circle* cir = (Circle*)shapes_[Cir].Allocate();
                //new (cir) Circle(xradius / meterScale_);
                cir->self = handles_.Insert(cir);
                handles_.SyncHandles<Circle>(shapes_[Cir]);

                //then add the body
                Body* body = (Body*)bodies_.Allocate();
                new (body)Body(cir, material, collisionCallback, position / meterScale_, Vec3D(), Vec3D(), userData, orientation);
                body->self = handles_.Insert(body);
                handles_.SyncHandles<Body>(bodies_);

                return body->self;
        }
				break;

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
		body->ChangeBody(xradius / meterScale_, y / meterScale_);
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
		
    handles_.Remove(handle);
    handles_.Remove(shape->self);


		//free shape and release handle
		//Shape* shapeRemoved = (Shape*)shapes_[body->shape_->GetShape()].Free(shape);
		//if(shapeRemoved)
		//	handles_.Update(shapeRemoved, shapeRemoved->self);
		
		//free body and release handle
		Body* bodyRemoved = (Body*)bodies_.Free(body);
		if(bodyRemoved)
			handles_.Update(bodyRemoved, bodyRemoved->self);
	}//end of RemoveBody
	//*************

	//this function advances the game forward one dt
	void PhysicsSpace::Step(void)
	{

    locked_ = true;

		//iterate through list of bodies
		for(unsigned i = 0; i < bodies_.Size(); ++i)
		{
			//move one body in vector forward to start
			for(unsigned j = i + 1; j < bodies_.Size(); ++j)
			{
				//if the bodies are static, don't put it in a manifold
				if (((Body*)bodies_[i])->massData_.mass == 0 && ((Body*)bodies_[j])->massData_.mass == 0)
					continue;
				
				//check collision groups
				if (((Body*)bodies_[i])->collisionGroup_ != ((Body*)bodies_[j])->collisionGroup_)
					continue;

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
		for(unsigned int i = 0; i < manifolds_.size(); ++i)
		{
      if (manifolds_[i].A->collisionCallback_ || manifolds_[i].B->collisionCallback_)
			  cb_(manifolds_[i].A->userData, manifolds_[i].B->userData, userData_, &manifolds_[i]);
		}

		//empty manifold list;
		manifolds_.clear();

    locked_ = false;

	}//end of Step
	//*************

	void PhysicsSpace::SymplecticEuler(Body& body)
	{
    if (body.massData_.mass == 0)
    {
      body.position_ += body.velocity_ * dt_;
      return;
    }
			

		//calculate the angular velocity
		body.angularVelocity_ += (body.torque_ * body.massData_.inverseInertia) * dt_;

		//then determine the orientation of the object
		body.orientation_ += body.angularVelocity_ * dt_;

		//Velocity can be calculated after adjusting force
		body.velocity_ += ((body.force_ *= body.massData_.inverseMass) 
			+ Vec3D(0,modifiedGravity_) * (float)body.gravityScale_ * (float)body.gravityOn_) * dt_;

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