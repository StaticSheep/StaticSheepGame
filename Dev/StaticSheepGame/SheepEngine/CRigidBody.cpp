#pragma once
#include "CRigidBody.h"
#include "SheepPhysics.h"

namespace Framework
{
	//constructor
	RigidBody::RigidBody(
		SheepFizz::Shapes shape,
		SheepFizz::Material* material,
		float radius,
		float y,
		float orientation ) :
	m_shape(shape), m_material(material), m_xradius(radius), m_yval(y), m_orientation(orientation) {}

	RigidBody::~RigidBody()
	{

	}
		

	void RigidBody::Initialize(void)
	{
		//get a pointer to the transform component
		Transform* trans = (this->space->GetHandles().GetAs<GameObject>(this->owner))->GetComponent<Transform>(eTransform);
		
		//temp vec for holding position
		Vec3D position;

		//get the translation from the transform component
		position.x_ = trans->Translation.X;
		position.y_ = trans->Translation.Y;

		//check if the shape is a circle or rectangle
		//if so, add a body and return the pointer to the component
		if(m_shape == SheepFizz::Cir || m_shape == SheepFizz::Rec)
			m_handle = PHYSICS->AddBodies(m_shape, *m_material, position, 
			m_xradius, m_yval, m_orientation);
	}

	void RigidBody::Remove()
	{
		PHYSICS->RemoveBodies(m_handle);
	}
}