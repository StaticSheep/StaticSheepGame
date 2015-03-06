/*****************************************************************
Filename: SheepPhysics.cpp
Project: 
Author(s): Jon Sourbeer(primary), Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "SheepPhysics.h"

#include "systems/graphics/DrawLib.h"           //Draw::     
#include "systems/graphics/SheepGraphics.h"     //Wireframe()
#include "systems/debug/Debug.h"                //Debug Struct
#include "systems/input/Input.h"                //input SHEEPINPUT
#include "components/transform/CTransform.h"
#include "components/colliders/CBoxCollider.h"
#include "components/colliders/CCircleCollider.h"

#pragma once
#include "components/controllers/player/CPlayerController.h"

namespace Framework
{
	SheepPhysics* PHYSICS = NULL;

	SheepPhysics::SheepPhysics()
	{
		// Initialize physics object list
		PHYSICS = this;

    //initialize debug info
    debugPhys.bodies = 0;
    debugPhys.manifolds = 0;
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

  //debug draw after postdraw message received
  void SheepPhysics::ReceiveMessage(Message& msg)
  {
    
    if (msg.MessageId == Message::PostDraw)
    {
      if (!debugOn)
        return;

      //go through all the game spaces
      std::vector<GameSpace*>& gspaces = ENGINE->Spaces();

      for (unsigned int i = 0; i < gspaces.size(); ++i)
      {
        //skip if the gamespace is not ready for some reason
        if (!gspaces[i]->Ready())
          return;

        debugPhys.bodies = ((SheepFizz::PhysicsSpace*)(gspaces[i]->m_pSpace))->GetDebugBodyNumber();
        debugPhys.manifolds = ((SheepFizz::PhysicsSpace*)(gspaces[i]->m_pSpace))->GetDebugManifoldNumber();

        //******************************
        //go through box colliders first
        ObjectAllocator* boxes = gspaces[i]->GetComponents(eBoxCollider);
        
        for (unsigned int j = 0; j < boxes->Size(); ++j)
        {
          Vec3D bodyPosition = ((BoxCollider*)(*boxes)[j])->GetBodyPosition();
          Vec3D bodyVelocity = ((BoxCollider*)(*boxes)[j])->GetCurrentVelocity();

          //draw velocity line
          Draw::SetColor(1, 0, 0, 1);
          Draw::DrawLine(bodyPosition.x, bodyPosition.y,
            bodyPosition.x + bodyVelocity.x * .5f, bodyPosition.y + bodyVelocity.y * .5f);

          //draw normals
          Vec3D normal;
          Vec3D location;

          for (unsigned int k = 0; k < ((BoxCollider*)(*boxes)[j])->normals_.size(); k += 2)
          {
            normal = ((BoxCollider*)(*boxes)[j])->normals_[k];
            location = ((BoxCollider*)(*boxes)[j])->normals_[k + 1];

            //modify by size modifer
            normal *= -20;
            location *= 32;

            Draw::SetColor(0, 1, 0, 1);
            Draw::DrawLine(location.x, location.y,
              location.x + normal.x, location.y + normal.y);

          }

          ((BoxCollider*)(*boxes)[j])->normals_.clear();

          //determine the number of vertices - should work for polygons, as well
          unsigned int vertexTotal = ((BoxCollider*)(*boxes)[j])->GetBodyVertexNumber();

          unsigned int nextVertex;
          float rotation = ((BoxCollider*)(*boxes)[j])->GetBodyRotation();

          //set up vec3s for vertices
          Vec3D vertexPos1;
          Vec3D vertexPos2;

          Vec3D vertex1 = ((BoxCollider*)(*boxes)[j])->GetBodyVertex(i);
          Vec3D vertex0 = vertex1;
          Vec3D vertex2;

          //draw outside edge of box
          for (unsigned int i = 0; i < vertexTotal; ++i)
          {
            nextVertex = ((i + 1) < vertexTotal) ? (i + 1) : 0;

            if (nextVertex)
              vertex2 = ((BoxCollider*)(*boxes)[j])->GetBodyVertex(nextVertex);
            else
              vertex2 = vertex0;

            //move the two vertices
            vertexPos1.x = vertex1.x * cos(rotation) - vertex1.y * sin(rotation);
            vertexPos1.y = vertex1.x * sin(rotation) + vertex1.y * cos(rotation);
            vertexPos2.x = vertex2.x * cos(rotation) - vertex2.y * sin(rotation);
            vertexPos2.y = vertex2.x * sin(rotation) + vertex2.y * cos(rotation);

            Draw::SetColor(1, 1, 0, 1);
            Draw::DrawLine(bodyPosition.x + vertexPos1.x, bodyPosition.y + vertexPos1.y, 
              bodyPosition.x + vertexPos2.x, bodyPosition.y + vertexPos2.y);

            vertex1 = vertex2;
          }

        }//end of box collider area

        //******************************
        //now go through circle colliders
        ObjectAllocator* circles = gspaces[i]->GetComponents(eCircleCollider);

        for (unsigned int j = 0; j < circles->Size(); ++j)
        {
          Vec3D bodyPosition = ((CircleCollider*)(*circles)[j])->GetBodyPosition();
          Vec3D bodyVelocity = ((CircleCollider*)(*circles)[j])->GetCurrentVelocity();

          //draw velocity line
          Draw::SetColor(1, 0, 0, 1);
          Draw::DrawLine(bodyPosition.x, bodyPosition.y,
            bodyPosition.x + bodyVelocity.x * .5f, bodyPosition.y + bodyVelocity.y * .5f);

          Draw::DrawCircle(bodyPosition.x, bodyPosition.y, ((CircleCollider*)(*circles)[j])->m_radius);
        }
      
        //go through rays
        for (int i = 0; i < rayComplexDraws.size(); ++i)
          Draw::DrawLine(rayComplexDraws[i].first.x, rayComplexDraws[i].first.y, 
            rayComplexDraws[i].second.x, rayComplexDraws[i].second.y);         
          
        for (int i = 0; i < raySimpleDraws.size(); ++i)
          Draw::DrawBeam(raySimpleDraws[i].first, raySimpleDraws[i].second);

        Vec2 vertex1(10, 10);
        Vec2 vertex2(-10, 10);
        Vec2 vertex3(-10, -10);
        Vec2 vertex4(10, -10);
        Vec2 vertexPos1, vertexPos2, vertexPos3, vertexPos4;
        for (int i = 0; i < debugRayBodyCollisions_.size(); ++i)
        {
          Vec3D position = debugRayBodyCollisions_[i];
          //move the two vertices
          vertexPos1 = vertex1 + debugRayBodyCollisions_[i];
          vertexPos2 = vertex2 + debugRayBodyCollisions_[i];
          vertexPos3 = vertex3 + debugRayBodyCollisions_[i];
          vertexPos4 = vertex4 + debugRayBodyCollisions_[i];

          Draw::SetColor(1, 1, 0, 1);
          Draw::DrawLine(vertexPos1.x, vertexPos1.y, vertexPos2.x, vertexPos2.y);
          Draw::DrawLine(vertexPos2.x, vertexPos2.y, vertexPos3.x, vertexPos3.y);
          Draw::DrawLine(vertexPos3.x, vertexPos3.y, vertexPos4.x, vertexPos4.y);
          Draw::DrawLine(vertexPos4.x, vertexPos4.y, vertexPos1.x, vertexPos1.y);
          //Handle box = ((unsigned)debugRayBodyCollisions_[i]);

          //GameSpace* space = (GameSpace*)(ray.gameSpace);
          //GameObject* boxDraw = space->GetHandles().GetAs<GameObject>(box);
          //boxDraw->GetComponent<BoxCollider>()
        }

        rayComplexDraws.clear();
        raySimpleDraws.clear();
        debugRayBodyCollisions_.clear();
      }//end of the space loop

    }//end of the if check

  }//end of RecieveMsg - DebugDraw

  void SheepPhysics::SetDebug(bool on)
  {
    debugOn = on;
    GRAPHICS->SetWireframe(on);
  }

  bool SheepPhysics::IsDebugOn(void)
  {
    return debugOn;
  }

  //debug information struct
  const void* SheepPhysics::GetDebugData(void)
  {
    return (void*)(&debugPhys);
  }

  //end of Debug
  //************

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
    SheepFizz::Material NoBounce(100.0f, 0.0f, 1.0f, 1.0f);

		//insert materials into the unordered map
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Wood", Wood));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Iron", Iron));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Steel", Steel));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Fluff", Fluff));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Bounce", Bounce));
		m_materials.insert(std::pair<std::string, SheepFizz::Material>("Static", Static));
    m_materials.insert(std::pair<std::string, SheepFizz::Material>("NoBounce", NoBounce));

    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("NonCollide", NonCollide));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Collide", Collide));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Resolve", Resolve));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Player1", Player1));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Player2", Player2));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Player3", Player3));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Player4", Player4));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("Static", CollisionGroup::Static));
    m_collisionGroup.insert(std::pair<std::string, CollisionGroup>("RayCast", RayCast));

    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(NonCollide, "NonCollide"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Collide, "Collide"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Resolve, "Resolve"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Player1, "Player1"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Player2, "Player2"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Player3, "Player3"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(Player4, "Player4"));
    m_collisionGroupNames.insert(std::pair<CollisionGroup, std::string>(RayCast, "RayCast"));

    ray.collisionGroup = Collide;
    ray.rayDirection = Vec3D(1, 0, 0);
    ray.rayOrigin = Vec3D(0, 0, 0);
    ray.gameSpace = nullptr;

    debugOn = false;

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

  Vec3D SheepPhysics::GetBodyUpNormal(GameSpace* space, SheepFizz::Handle handle)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyUpNormal(handle);
  }

  Vec3D SheepPhysics::GetCollisionPoint(GameSpace* space, SheepFizz::ExternalManifold manifold)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetCollisionPoint(manifold);
  }

  std::string SheepPhysics::GetCollisionGroup(GameSpace* space, SheepFizz::Handle handle)
  {
    CollisionGroup group = ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyCollisionGroup(handle);
    return m_collisionGroupNames[group];
  }

  //shape
  unsigned int SheepPhysics::GetBodyVertexNumber(GameSpace* space, SheepFizz::Handle handle)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyVertexNumber(handle);
  }
  
  Vec3D SheepPhysics::GetBodyVertex(GameSpace* space, SheepFizz::Handle handle, unsigned int vertex)
  {
    return ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->GetBodyVertex(handle, vertex);
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

  void SheepPhysics::SetBodyRotation(GameSpace* space, SheepFizz::Handle handle, Vec3D direction)
  {
    ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyRot(handle, direction);
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

  //raycast
  //********************
  void SheepPhysics::SetRayConfig(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name)
  {
    if (m_collisionGroup.find(name) != m_collisionGroup.end())
      ray.collisionGroup = m_collisionGroup[name];

    else
      ray.collisionGroup = Collide;
    
    ray.rayDirection = rayDirection;
    ray.rayOrigin = rayOrigin;
  }

  bool SheepPhysics::SimpleRayCast(GameSpace* space)
  {
    return  ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->RayCaster(&ray);
  }

  bool SheepPhysics::ComplexRayCast(GameSpace* space)
  {
    ray.findFirstCollision = true;
    return  ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->RayCaster(&ray);
  }

  Vec3D SheepPhysics::GetFirstCollision()
  {
    return ray.firstCollisionLocation;
  }

  void SheepPhysics::RayDestruction(GameSpace* space, float damage)
  {
    if (ray.findFirstCollision)
    {
      //GameSpace* space = (GameSpace*)(ray.gameSpace);
      Handle handleObj = (unsigned)(ray.firstCollisionBody);

      GameObject* obj = space->GetHandles().GetAs<GameObject>(handleObj);
      if (obj->name == "Player")
      {
        PlayerController *playerCont = obj->GetComponent<PlayerController>(ePlayerController);
        int playerNum = playerCont->playerNum;
        playerCont->DealDamage(damage, playerNum);
          //DealDamage(50, playerNum);
        //int playerNum = obj->;
        //((PlayerController*)obj)->DealDamage(1, playerNum);
      }

      rayComplexDraws.push_back(std::pair<Vec2, Vec2>(ray.rayOrigin, ray.firstCollisionLocation));
      //raySimpleDraws.push_back(std::pair<Vec2, Vec2>(ray.rayOrigin, ray.rayDirection));
      //Draw::SetColor(1, 0, 0, 1);
      //Vec2 collisionLocation = Draw::ToScreen(ray.firstCollisionLocation);
      //Vec2 origin = Draw::ToScreen(ray.rayOrigin);
      //Vec2D direction = ray.rayDirection;
      //direction.y = -direction.y;
      //Draw::DrawBeam(origin, direction);
      //Draw::DrawBeam(Vec2(0, 0), Vec2(1, 1));
      //Draw::DrawLine(origin.x, origin.y, collisionLocation.x, collisionLocation.y);
      //Draw::DrawLine(origin.x, ray.rayOrigin.y, ray.firstCollisionLocation.x, ray.firstCollisionLocation.y, 1);
    }

    else
    {
      if (!ray.bodyIntersections_.empty())
        for (int i = 0; i < ray.bodyIntersections_.size(); ++i)
          debugRayBodyCollisions_.push_back(ray.bodyIntersections_[i]);
      raySimpleDraws.push_back(std::pair<Vec2, Vec2>(ray.rayOrigin, ray.rayDirection));
    }

    ray.bodyIntersections_.clear();
    ray.findFirstCollision = false;
  }

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

  void SheepPhysics::ChangeCollisionGroup(GameSpace* space, SheepFizz::Handle handle, std::string name)
  {
    if (m_collisionGroup.find(name) != m_collisionGroup.end())
    {
      CollisionGroup group = m_collisionGroup[name];
      ((SheepFizz::PhysicsSpace*)(space->m_pSpace))->SetBodyCollisionGroup(handle, group);
    }
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
    if (SHEEPINPUT->Keyboard.KeyIsPressed(VK_F11))
    {
      debugOn = !debugOn;
      if (!debugOn)
        GRAPHICS->SetWireframe(false);
    }

		std::vector<GameSpace*>& gSpaces = ENGINE->Spaces();
		for(size_t i = 0; i < gSpaces.size(); ++i)
		{
			if (gSpaces[i]->m_pSpace && !gSpaces[i]->Paused())
				((SheepFizz::PhysicsSpace*)(gSpaces[i]->m_pSpace))->Step();
		}
	}

}