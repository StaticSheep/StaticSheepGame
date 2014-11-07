/*****************************************************************
Filename: SheepPhysics.cpp
Project: 
Author(s): Zachary Nawar, Jon Sourbeer(primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "SheepPhysics.h"

#include "systems/graphics/DrawLib.h"

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
		REGISTER_COMPONENT(BoxCollider);
		REGISTER_COMPONENT(CircleCollider);

	 }

  
  void SheepPhysics::ReceiveMessage(Message& msg)
  {
    if (msg.MessageId == Message::PostDraw)
    {
      std::vector<GameSpace*>& gspaces = ENGINE->Spaces();

      for (int i = 0; i < gspaces.size(); ++i)
      {
        if (!gspaces[i]->Ready())
          return;

        ObjectAllocator* boxes = gspaces[i]->GetComponents(eBoxCollider);
        
        for (int j = 0; j < boxes->Size(); ++j)
        {
          Vec3D bodyPosition = ((BoxCollider*)(*boxes)[j])->GetBodyPosition();
          Vec3D bodyVelocity = ((BoxCollider*)(*boxes)[j])->GetCurrentVelocity();

          Draw::DrawLine(bodyPosition.x, bodyPosition.y,
            bodyPosition.x + bodyVelocity.x, bodyPosition.y + bodyVelocity.y);
        }

      }
    }
  }

	static void CollisionCallback(void* A_userData, void* B_userData,
    void* Space_userData, SheepFizz::ExternalManifold manifold)
	{
		Handle A_object = (unsigned)A_userData;
		Handle B_object = (unsigned)B_userData;
		GameSpace* space = (GameSpace*)Space_userData;

		GameObject* AObj = space->GetHandles().GetAs<GameObject>(A_object);
    AObj->hooks.Call("OnCollision", B_object, manifold);

		GameObject* BObj = space->GetHandles().GetAs<GameObject>(B_object);
		BObj->hooks.Call("OnCollision", A_object, manifold);
	}//end of CollisionCallback

	void* SheepPhysics::CreateSpace(GameSpace* gameSpace)
	{
		SheepFizz::PhysicsSpace* newSpace = SheepFizz::PhysicsSpace::Allocate(0.0167f, 32.0f);
		
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
	}

	// initialize materials
	void SheepPhysics::Initialize()
	{
		
		//add materials
		SheepFizz::Material Wood(3.0f, 2.5f, .8f, .4f);
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
    SheepFizz::Material& material, bool collisionCallback, Vec3D position,
		float xradius, float yval, float orientation)
	{
		GameSpace* space = obj->space;

		if(!space->m_pSpace)
			CreateSpace(space);

		//return a sheepfizz handle
		SheepFizz::Handle handle = ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddBody(shape, material, 
      collisionCallback, position, xradius, yval, orientation, (void*)((unsigned)obj->self));

    ErrorIf(handle == Handle::null, "Sheep Physics", 
      "Null handle returned from Physics! You're probably creating a physics body when you shouldn't be!");

		return handle;
	}//end of AddBodies

	//get and set values
	//********************
	//gettors
	Vec3D SheepPhysics::GetBodyPosition(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyPos(handle);
	}

	Vec3D SheepPhysics::GetBodyVelocity(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyVeloc(handle);
	}

	Vec3D SheepPhysics::GetBodyForce(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyForce(handle);
	}

	float SheepPhysics::GetBodyRotation(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyRot(handle);
	}

	float SheepPhysics::GetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyAngVeloc(handle);
	}

	float SheepPhysics::GetBodyTorques(GameSpace* space, SheepFizz::Handle handle)
	{
		return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyTorque(handle);
	}

  Vec3D SheepPhysics::GetCollisionNormal(GameSpace* space, Framework::Handle ownerHandle, SheepFizz::ExternalManifold manifold)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetCollisionNorm((void*)(unsigned int)ownerHandle, manifold);
  }

  Vec3D SheepPhysics::GetBodyGravityNormal(GameSpace* space, SheepFizz::Handle handle)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyGravityNormal(handle);
  }

	//settors
	void SheepPhysics::SetBodyPosition(GameSpace* space, SheepFizz::Handle handle, Vec3D position)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyPos(handle, position);
	}
	
	void SheepPhysics::SetBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyVeloc(handle, velocity);
	}

	void SheepPhysics::SetBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyForce(handle, force);
	}

	void SheepPhysics::SetBodyRotation(GameSpace* space, SheepFizz::Handle handle, float rotation)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyRot(handle, rotation);
	}

	void SheepPhysics::SetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyAngVeloc(handle, angveloc);
	}

	void SheepPhysics::SetBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyTorque(handle, torque);
	}

	void SheepPhysics::SetDT(GameSpace* space, float dt)
	{
	if (space->m_pSpace)
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetTime(dt);
	}

  void SheepPhysics::SetBodyCollisionCallback(GameSpace* space, SheepFizz::Handle handle, bool collisionCallback)
  {
    ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyCollisionCallback(handle, collisionCallback);
  }

  //gravity functions
  void SheepPhysics::SetBodyGravityOn(GameSpace* space, SheepFizz::Handle handle)
  {
    ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyGravityOn(handle);
  }

  void SheepPhysics::SetBodyGravityOff(GameSpace* space, SheepFizz::Handle handle)
  {
    ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyGravityOff(handle);
  }

  void SheepPhysics::SetBodyGravityNormal(GameSpace* space, SheepFizz::Handle handle, Vec3D normal)
  {
    ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyGravityNormal(handle, normal);
  }

	//additive functions
	void SheepPhysics::AddToBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyVeloc(handle, velocity);
	}

	void SheepPhysics::AddToBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyForce(handle, force);
	}

	void SheepPhysics::AddToBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyAngVeloc(handle, angveloc);
	}

	void SheepPhysics::AddToBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->AddToBodyTorque(handle, torque);
	}

	//end of gettors and settors
	//********************


	//change bodies
	//********************
	void SheepPhysics::ChangePhysBodies(GameSpace* space, SheepFizz::Handle handle, float xradius, float y)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->ChangeBodies(handle, xradius, y);
	}

	void SheepPhysics::ChangePhysMaterials(GameSpace* space, SheepFizz::Handle handle, SheepFizz::Material& material)
	{
		((SheepFizz::PhysicsSpace*)(space->m_pSpace))->ChangeMaterials(handle, material);
	}

	//remove bodies from space
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
	}

}