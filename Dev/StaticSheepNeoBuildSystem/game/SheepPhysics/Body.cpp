
#include "precompiled.h"
#include "Body.h"

namespace SheepFizz
{

	//body requires a shape* to be passed - all others default to base constructors
	Body::Body(Shape* shape, Material& material, Vec3D position, Vec3D velocity, 
		Vec3D force, void* userData, float orientation, float angularVelocity, float torque,
		CollisionGroup collisionGroup, unsigned int bodyGroup, float gravityScale, 
		unsigned int gravityOn, bool staticObject) :
		material_(material), shape_(shape), velocity_(velocity), position_(position), force_(force), userData(userData),
		orientation_(orientation), angularVelocity_(angularVelocity), torque_(torque),
		collisionGroup_(collisionGroup), bodyGroup_(bodyGroup), gravityScale_(gravityScale), gravityOn_(gravityOn),
		staticObject_(staticObject)
		{ 
			shape_->Initialize();
			ComputeMass();
		}

	void Body::ComputeMass(void)
	{

		//calculate mass based on area - if mass is 0, it is considered infinite
		massData_.mass = shape_->GetArea() * material_.GetMaterialDensity();

		//if mass is 0, set inverse mass to 0 as well
		massData_.inverseMass = massData_.mass ? 1 / massData_.mass : 0.0f;

		//calculate inertia for rotations
		massData_.inertia = shape_->GetMomentOfInertia() * massData_.mass;

		//check if inertia exists
		massData_.inverseInertia = massData_.inertia ? 1 / massData_.inertia : 0.0f;

	}//end of ComputeMass

	//change the body size and recompute the mass
	void Body::ChangeBody(float x, float y)
	{
		shape_->ChangeShape(x, y);
		ComputeMass();
	}//end of ChangeBody

	//change the material
	void Body::ChangeMaterial(Material& material)
	{ 
		material_ = material;
		ComputeMass();
	}//end of ChangeMaterial

	//apply forces directly to the body - an impulse is an
	//instantaneous force application, so no dt is applied
	void Body::ApplyImpulse(Vec3D& impulse, Vec3D& contactvector)
		{
			velocity_ += impulse * massData_.inverseMass;

			//takes the contact vector - which is the distance from center of mass to contact point
			//and crosses it with the actual impulse, creating angular rotation.  The further out
			//the contact point, the greater the rotation applied
			angularVelocity_ += (contactvector * impulse).z_ * massData_.inverseInertia;
		}	

}