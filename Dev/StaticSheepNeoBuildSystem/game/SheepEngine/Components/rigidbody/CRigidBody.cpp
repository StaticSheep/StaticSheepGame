#pragma once
#include "pch/precompiled.h"


#include "CRigidBody.h"
#include "components/transform/CTransform.h"

#include "systems/physics/SheepPhysics.h"

namespace Framework
{
	//constructor
	RigidBody::RigidBody(SheepFizz::Shapes shape) :
		m_shape(shape), m_width(12), m_height(12), m_materialName("Wood"),
		m_material(nullptr)
	{

	}

	RigidBody::~RigidBody()
	{

	}
	
	//initialize the rigidbody
	void RigidBody::Initialize(void)
	{
		//get a pointer to the transform component
		Transform* trans = (this->space->GetHandles().GetAs<GameObject>(this->owner))->GetComponent<Transform>(eTransform);

		m_material = PHYSICS->GetMaterial(m_materialName);
		
		//temp vec for holding position
		Vec3 position = trans->GetTranslation();
		float rotation = trans->GetRotation();

			//check if the shape is a circle or rectangle
			//if so, add a body and return the pointer to the component
		if(m_shape == SheepFizz::Cir || m_shape == SheepFizz::Rec)
		  m_handle = PHYSICS->AddBodies(space->GetHandles().GetAs<GameObject>(owner), m_shape, *m_material, position, 
		  m_radius, m_height, rotation);

		 trans->SetPhysicsBody(m_handle);
	}

	//remove the body from the space
	void RigidBody::Remove()
	{
    if (space->m_edit || space->Paused())
    {
      Transform* trans = (this->space->GetHandles().GetAs<GameObject>(this->owner))->GetComponent<Transform>(eTransform);
      Vec3 position = trans->GetTranslation();
      float rotation = trans->GetRotation();

      trans->SetPhysicsBody(Handle::null);

      trans->SetTranslation(position);
      trans->SetRotation(rotation);
    }

		PHYSICS->RemoveBodies(space, m_handle);
	}

	void RigidBody::SetVelocity(Vec3D& velocity)
	{
		PHYSICS->SetBodyVelocity(space, m_handle, velocity);
	}

	void AddToVelocity(Vec3D& velocity)
	{
		PHYSICS->AddToBodyVelocity(space, m_handle, velocity);
	}

	void AddToForce(Vec3D& force)
	{
		PHYSICS->AddToBodyVelocity(space, m_handle, force);
	}

	void AddToAngVelocity(float angularvelocity)
	{
		PHYSICS->AddToBodyVelocity(space, m_handle, angularvelocity);
	}

	void AddToTorque(float torque)
	{
		PHYSICS->AddToBodyVelocity(space, m_handle, torque);
	}
}