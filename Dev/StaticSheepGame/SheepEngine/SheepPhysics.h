/*****************************************************************
Filename: SheepPhysics.h
Project: 
Author(s): Zachary Nawar, Jon Sourbeer (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once


#include "physics\api.h"
#include "physics\handle.h"
#include "physics\step.h"

#include "System.h"



namespace Framework
{

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

			// Initialization call
			void Initialize(void);

			//Adds rectangles and circles to the physics engine
			SheepFizz::Handle AddBodies(GameObject* obj, 
				SheepFizz::Shapes shape,			//the shape to add (cir or rec)
				SheepFizz::Material& material,		//the material
				Vec3D position,						//the positon of the transform
				float xradius,						//the radius or width
				float yval = 0,						//the height (rec only)
				float orientation = 0);				//orientation of object

			//remove bodies from the bodies held in the physics engine
			void RemoveBodies(GameSpace* space, SheepFizz::Handle handle);

			SheepFizz::Material* GetMaterial(std::string name);

			// Update per frame
			void Update(float dt);

			void Shutdown(void);

		private:
			//void PhysicsStep(float dt);
			std::unordered_map<std::string, SheepFizz::Material> m_materials;
	};

	extern SheepPhysics* PHYSICS;
}