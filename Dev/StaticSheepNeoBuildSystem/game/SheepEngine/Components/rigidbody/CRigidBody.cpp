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
		m_material(nullptr), m_hasCollisionCallback(false)
	{

	}

	RigidBody::~RigidBody()
	{

	}
	
  static Vec3D ScaleDown(Vec3D value)
  {
   return (value *= 1.0f / 32.0f);
  }

  static float ScaleDown(float value)
  {
    return (value *= 1.0f / 32.0f);
  }

  static Vec3D ScaleUp(Vec3D value)
  {
    return (value *= 1.0f / 32.0f);
  }

  static float ScaleUp(float value)
  {
    return (value *= 1.0f / 32.0f);
  }

	//initialize the rigid body
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
		  m_handle = PHYSICS->AddBodies(space->GetHandles().GetAs<GameObject>(owner), m_shape, *m_material, m_hasCollisionCallback,
      ScaleDown(position), ScaleDown(m_radius), ScaleDown(m_height), rotation);

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
		PHYSICS->SetBodyVelocity(space, m_handle, ScaleDown(velocity));
	}

  void RigidBody::SetAngVelocity(float angularvelocity)
  {
    PHYSICS->SetBodyAngVelocity(space, m_handle, angularvelocity);
  }

  void RigidBody::SetBodyCollisionCallback(bool collisionCallback)
  {
    PHYSICS->SetBodyCollisionCallback(space, m_handle, collisionCallback);
  }

	void RigidBody::AddToVelocity(Vec3D& velocity)
	{
    PHYSICS->AddToBodyVelocity(space, m_handle, ScaleDown(velocity));
	}

	void RigidBody::AddToForce(Vec3D& force)
	{
    PHYSICS->AddToBodyForce(space, m_handle, ScaleDown(force));
	}

	void RigidBody::AddToAngVelocity(float angularvelocity)
	{
    PHYSICS->AddToBodyAngVelocity(space, m_handle, angularvelocity);
	}

	void RigidBody::AddToTorque(float torque)
	{
		PHYSICS->AddToBodyTorques(space, m_handle, torque);
	}

  Vec3D RigidBody::GetCollisionNormals(void* manifold)
  {
   return ScaleUp(PHYSICS->GetCollisionNormal(space, m_handle, manifold));
  }

  Vec3D RigidBody::GetCurrentVelocity(void)
  {
    return ScaleUp(PHYSICS->GetBodyVelocity(space, m_handle));
  }
}