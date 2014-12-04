/*****************************************************************
Filename: SheepPhysics.cpp
Project:
Author(s): Jon Sourbeer(primary), Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once


#include "SheepPhysics/api.h"
#include "SheepPhysics/Step.h"

#include "systems/System.h"



namespace Framework
{

  struct DebugPhysics
  {
    unsigned int bodies;
    unsigned int manifolds;
  };

	class SheepPhysics : public ISystem
	{
		public:
			// Sheep physics interface constructor
			SheepPhysics();
			// Sheep physics interface deconstructor
			~SheepPhysics();

			virtual std::string GetName() {return "SheepPhysics";};

			void RegisterComponents();

			void* CreateSpace(GameSpace*);

			void DeleteSpace(void*);

			//Initialization call
			void Initialize(void);

      //Debug
      void ReceiveMessage(Message& msg);
      void SetDebug(bool on);
      bool IsDebugOn(void);

			//Adds rectangles and circles to the physics engine
      SheepFizz::Handle AddBodies(GameObject* obj,
        SheepFizz::Shapes shape,			    //the shape to add (cir or rec)
        SheepFizz::Material& material,		//the material
        bool collisionCallback = 0,       //whether callback is active
        Vec3D position = Vec3D(),					//the positon of the transform
        float xradius = 1,						    //the radius or width
        float yval = 0,						        //the height (rec only)
        float orientation = 0             //orientation of object
        );

			//access to bodies for changing the bodies
			void ChangePhysBodies(GameSpace* space, SheepFizz::Handle handle, float xradius, float y);
			void ChangePhysMaterials(GameSpace* space, SheepFizz::Handle handle, SheepFizz::Material& material);
      void ChangeCollisionGroup(GameSpace* space, SheepFizz::Handle handle, std::string name);

			//gettors
			Vec3D GetBodyPosition(GameSpace* space, SheepFizz::Handle handle);
			Vec3D GetBodyVelocity(GameSpace* space, SheepFizz::Handle handle);
			Vec3D GetBodyForce(GameSpace* space, SheepFizz::Handle handle);
			float GetBodyRotation(GameSpace* space, SheepFizz::Handle handle);
			float GetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle);
			float GetBodyTorques(GameSpace* space, SheepFizz::Handle handle);

      Vec3D GetBodyGravityNormal(GameSpace* space, SheepFizz::Handle handle);
      Vec3D GetBodyUpNormal(GameSpace* space, SheepFizz::Handle handle);

      //debug
      const void* SheepPhysics::GetDebugData(void);

      //collision
      Vec3D GetCollisionNormal(GameSpace* space, Framework::Handle ownerHandle, SheepFizz::ExternalManifold manifold);
      Vec3D GetCollisionPoint(GameSpace* space, SheepFizz::ExternalManifold manifold);
      std::string GetCollisionGroup(GameSpace* space, SheepFizz::Handle handle);

			//settors
			void SetBodyPosition(GameSpace* space, SheepFizz::Handle handle, Vec3D position);
			void SetBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity);
			void SetBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force);
			void SetBodyRotation(GameSpace* space, SheepFizz::Handle handle, float rotation);
      void SetBodyRotation(GameSpace* space, SheepFizz::Handle handle, Vec3D direction);
			void SetBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc);
			void SetBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque);

      //gravity
      void SetBodyGravityOn(GameSpace* space, SheepFizz::Handle handle);
      void SetBodyGravityOff(GameSpace* space, SheepFizz::Handle handle);
      void SetBodyGravityNormal(GameSpace* space, SheepFizz::Handle handle, Vec3D normal);

      void SetBodyCollisionCallback(GameSpace* space, SheepFizz::Handle handle, bool collisionCallback);
			
			//adders
			void AddToBodyVelocity(GameSpace* space, SheepFizz::Handle handle, Vec3D velocity);
			void AddToBodyForce(GameSpace* space, SheepFizz::Handle handle, Vec3D force);
			void AddToBodyAngVelocity(GameSpace* space, SheepFizz::Handle handle, float angveloc);
			void AddToBodyTorques(GameSpace* space, SheepFizz::Handle handle, float torque);

      void SetDT(GameSpace* space, float dt);

			//remove bodies from the bodies held in the physics engine
			void RemoveBodies(GameSpace* space, SheepFizz::Handle handle);

			SheepFizz::Material* GetMaterial(std::string name);

      //shape info
      unsigned int GetBodyVertexNumber(GameSpace* space, SheepFizz::Handle handle);
      Vec3D GetBodyVertex(GameSpace* space, SheepFizz::Handle handle, unsigned int vertex);

			// Update per frame
			void Update(float dt);

			void Shutdown(void);

		private:
			std::unordered_map<std::string, SheepFizz::Material> m_materials;
      std::unordered_map<std::string, CollisionGroup> m_collisionGroup;
      std::unordered_map<CollisionGroup, std::string> m_collisionGroupNames;
      bool debugOn;
      DebugPhysics debugPhys;

	};

	extern SheepPhysics* PHYSICS;
}