/*****************************************************************
Filename: SheepPhysics.cpp
Project: 
Author(s): Zachary Nawar, Jon Sourbeer(primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "SheepPhysics.h"

#include "components/transform/CTransform.h"
#include "components/colliders/CBoxCollider.h"
#include "components/colliders/CCircleCollider.h"

namespace Framework
{
	SheepPhysics* PHYSICS = NULL;

	SheepPhysics::SheepPhysics()
	{
		// Initialize physics object list
		PHYSICS = this;
	}//end of Constructor

	SheepPhysics::~SheepPhysics()
	{
		PHYSICS = nullptr;
	}//end of Destructor

	void SheepPhysics::Shutdown()
	{

	}//end of Shutdown

	void SheepPhysics::RegisterComponents()
	{
		REGISTER_COMPONENT(Transform);
		REGISTER_COMPONENT(BoxCollider);
		REGISTER_COMPONENT(CircleCollider);

	 }//end of RegisterComponents

	static void CollisionCallback(void* A_userData, void* B_userData, void* Space_userData)
	{
		Handle A_object = (unsigned)A_userData;
		Handle B_object = (unsigned)B_userData;
		GameSpace* space = (GameSpace*)Space_userData;

		GameObject* AObj = space->GetHandles().GetAs<GameObject>(A_object);
		GameObject* BObj = space->GetHandles().GetAs<GameObject>(B_object);

		AObj->m_hooks.Call("Collision",B_object);
		BObj->m_hooks.Call("Collision",A_object);
	}//end of CollisionCallback

	void* SheepPhysics::CreateSpace(GameSpace* gameSpace)
	{
		SheepFizz::PhysicsSpace* newSpace = SheepFizz::PhysicsSpace::Allocate(0.0167f);
		
		newSpace->SetCollisionCallback(CollisionCallback);
		newSpace->SetUserData(gameSpace);
		gameSpace->m_pSpace = newSpace;

    if (gameSpace->Paused())
      SetDT(gameSpace, 0.0f);

		return newSpace;
	}//end of CreateSpace

	void SheepPhysics::DeleteSpace(void* p_Space)
	{
		SheepFizz::PhysicsSpace::Delete((SheepFizz::PhysicsSpace*)p_Space);
	}//end of DeleteSpace

	//intialize materials
	void SheepPhysics::Initialize()
	{
		
		//add materials
		SheepFizz::Material Wood(.8f, .5f, .8f, .4f);
		SheepFizz::Material Iron(1.5f, .8f, .6f, .3f);
		SheepFizz::Material Steel(1.3f, .8f, .3f, .15f);
		SheepFizz::Material Fluff(.3f, .2f, .4f, .1f);
		SheepFizz::Material Bounce(.3f, 4.0f, .3f, .15f);
		SheepFizz::Material Static(.0f,.2f,.5f,.3f);

		//insert materials into the unordered map
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Wood", Wood));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Iron", Iron));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Steel", Steel));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Fluff", Fluff));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Bounce", Bounce));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Static", Static));
	}//end of Initialize

	//add circles or rectangles
	SheepFizz::Handle SheepPhysics::AddBodies(GameObject* obj, SheepFizz::Shapes shape, 
		SheepFizz::Material& material, Vec3D position,
		float xradius, float yval, float orientation)
	{
		GameSpace* space = obj->space;

		if(!space->m_pSpace)
			CreateSpace(space);

		//return a sheepfizz handle
		SheepFizz::Handle handle = ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddBody(shape, material, position, 
			xradius, yval, orientation, (void*)((unsigned)obj->self));
		return handle;
	}//end of AddBodies

	//get and set values
	//********************
	//gettors
	Vec3D SheepPhysics::GetBodyPosition(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyPos(handle);
	}//end of GetBodyPosition

	Vec3D SheepPhysics::GetBodyVelocity(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyVeloc(handle);
	}//end of GetBodyVelocity
	Vec3D SheepPhysics::GetBodyForce(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyForce(handle);
	}//end of GetBodyForce

	Vec3D SheepPhysics::GetBodyRotation(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyRot(handle);
	}//end of GetBodyRotation

	Vec3D SheepPhysics::GetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyAngVeloc(handle);
	}//end of GetBodyAngVeloc

	Vec3D SheepPhysics::GetBodyTorques(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyTorque(handle);
	}//end of GetBodyTorques

	//settors
	void SheepPhysics::SetBodyPosition(GameSpace* space, SheepFizz::Handle handle, Vec3D position)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyPos(handle, position);
	}//end of SetBodyPosition
	
	void SheepPhysics::SetBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyVeloc(handle, velocity);
	}//end of SetBodyVeloc

	void SheepPhysics::SetBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyForce(handle, force);
	}//end of SetBodyForce

	void SheepPhysics::SetBodyRotation(GameSpace* space, SheepFizz::Handle handle, float rotation)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyRot(handle, rotation);
	}//end of SetBodyRoation

	void SheepPhysics::SetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyAngVeloc(handle, angveloc);
	}//end of SetBodyAngVelocity

	void SheepPhysics::SetBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyTorque(handle, torque);
	}//end of SetBodyTorques

	void SheepPhysics::SetDT(GameSpace* space, float dt)
	{
	if (space->m_pSpace)
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetTime(dt);
	}

	//additive functions
	void SheepPhysics::AddToBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyVeloc(handle, velocity);
	}//end of AddToBodyVeloc

	void SheepPhysics::AddToBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyForce(handle, force);
	}//end of AddToBodyForce

	void SheepPhysics::AddToBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyAngVeloc(handle, angveloc);
	}//end of AddToBodyAngVelocity

	void SheepPhysics::AddToBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyTorque(handle, torque);
	}//end of AddToBodyTorques

	//end of gettors and settors
	//********************


	//change bodies
	//********************
	void SheepPhysics::ChangePhysBodies(GameSpace* space, SheepFizz::Handle handle, float xradius, float y)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->ChangeBodies(handle, xradius, y);
	}//end of ChangePhysBodies

	void SheepPhysics::ChangePhysMaterials(GameSpace* space, SheepFizz::Handle handle, SheepFizz::Material& material)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->ChangeMaterials(handle, material);
	}//end of ChangePhysMaterials

	//remove bodies from space
	void SheepPhysics::RemoveBodies(GameSpace* space, SheepFizz::Handle handle)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->RemoveBody(handle);
	}//end of RemoveBodies

	SheepFizz::Material* SheepPhysics::GetMaterial(std::string name)
	{
		//find the material and return it
		SheepFizz::Material* material = &m_materials[name];
		return material;
	}//end of GetMaterial
	//********************

	//update all gamespaces by dt
	void SheepPhysics::Update(float dt)
	{
		std::vector<GameSpace*>& gSpaces = ENGINE->Spaces();
		for(size_t i = 0; i < gSpaces.size(); ++i)
		{
			if (gSpaces[i]->m_pSpace && !gSpaces[i]->Paused())
				((SheepFizz::PhysicsSpace*)(gSpaces[i]->m_pSpace))->Step();
		}
	}//end of Update

}