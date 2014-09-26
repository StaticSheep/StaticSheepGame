/*****************************************************************
Filename: SheepPhysics.cpp
Project: 
Author(s): Zachary Nawar, Jon Sourbeer(primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepPhysics.h"

namespace Framework
{
	SheepPhysics* PHYSICS = NULL;

	SheepPhysics::SheepPhysics()
	{
		// Initialize physics object list
		PHYSICS = this;
	}

	SheepPhysics::~SheepPhysics()
	{
		PHYSICS = nullptr;
	}

	void SheepPhysics::Shutdown()
	{

	}

	void SheepPhysics::RegisterComponents()
	{
		REGISTER_COMPONENT(Transform);
		REGISTER_COMPONENT(Tester);
		//REGISTER_COMPONENT(RigidBody);

	 }

	static void CollisionCallback(void* A_userData, void* B_userData, void* Space_userData)
	{
		Handle A_object = (unsigned)A_userData;
		Handle B_object = (unsigned)B_userData;
		GameSpace* space = (GameSpace*)Space_userData;

		GameObject* AObj = space->GetHandles().GetAs<GameObject>(A_object);
		GameObject* BObj = space->GetHandles().GetAs<GameObject>(B_object);

		AObj->m_hooks.Call("Collision",B_object);
		BObj->m_hooks.Call("Collision",A_object);
	}

	void* SheepPhysics::CreateSpace(GameSpace* gameSpace)
	{
		SheepFizz::PhysicsSpace* newSpace = SheepFizz::PhysicsSpace::Allocate(0.0167f);
		
		newSpace->SetCollisionCallback(CollisionCallback);
		newSpace->SetUserData(gameSpace);
		gameSpace->m_pSpace = newSpace;

		return newSpace;
	}

	void SheepPhysics::DeleteSpace(void* p_Space)
	{
		SheepFizz::PhysicsSpace::Delete((SheepFizz::PhysicsSpace*)p_Space);
	}

	void SheepPhysics::Initialize()
	{
		
		//add materials
		SheepFizz::Material Wood(.8f, .5f, .8f, .4f);
		SheepFizz::Material Iron(1.5f, .8f, .6f, .3f);
		SheepFizz::Material Steel(1.3f, .8f, .3f, .15f);
		SheepFizz::Material Fluff(.3f, .2f, .4f, .1f);
		SheepFizz::Material Bounce(.8f, 2.0f, .3f, .15f);
		SheepFizz::Material Static(.0f,.2f,.5f,.3f);

		//insert materials into the unordered map
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Wood", Wood));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Iron", Iron));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Steel", Steel));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Fluff", Fluff));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Bounce", Bounce));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Static", Static));


	}

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
	}

	void SheepPhysics::RemoveBodies(GameSpace* space, SheepFizz::Handle handle)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->RemoveBody(handle);
	}

	SheepFizz::Material* SheepPhysics::GetMaterial(std::string name)
	{
		//find the material and return it
		SheepFizz::Material* material = &m_materials[name];
		return material;
	}

	void SheepPhysics::Update(float dt)
	{
		std::vector<GameSpace*>& gSpaces = ENGINE->Spaces();
		for(size_t i = 0; i < gSpaces.size(); ++i)
		{
			((SheepFizz::PhysicsSpace*)(gSpaces[i]->m_pSpace))->Step();
		}
	}

}