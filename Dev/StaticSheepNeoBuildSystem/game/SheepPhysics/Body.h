/*****************************************************************
Filename: Body.h
Project:
Author(s): Jon Sourbeer (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "Material.h"
#include "Shape.h"
#include "BodyProperties.h"
#include "CollisionGroups.h"

namespace SheepFizz
{

  class MassData
  {
	  public:
		  float mass;
		  float inverseMass;

		  float inertia;			//used for angular calculations
		  float inverseInertia;
  };

  class Body
  {
	  public:
		  //body requires a shape* to be passed - all others default to base constructors
		  Body(Shape* shape,								//a pointer to the shape created
			  Material& material,							//material of the specific body
        bool collisionCallback,         //activates collisionCallback
			  Vec3D position = Vec3D(0,0,0),
			  Vec3D velocity = Vec3D(0,0,0),
			  Vec3D force = Vec3D(0,0,0),
			  void* userData = NULL,
			  float orientation = PI,
			  float angularVelocity = 0,
			  float torque = 0,
			  CollisionGroup collisionGroup = Resolve, //used for determining collision groups
			  unsigned int bodyGroup = 0,					//used for certain collisions/resolutions 
			  float gravityScale = 1,						//used to modify gravity
			  unsigned int gravityOn = 1					//used to turn gravity on and off
			  );					

		  //used in initialization of body - computes mass
		  void ComputeMass(void);

		  //function used to identify type of body
		  //and apply special functions to player
		  void GetBodyProperties(void);

		  //turn gravity on and off in a direction
		  void ActivateGravity(void);
		  void DeactivateGravity(void);
		  void SetGravityNormal(Vec3D& normal) { gravityNormal_ = normal; }

		  //used for expansion for other collisions
		  void ObjectCollision(unsigned int rectangleSide);
		  void ResolveNormal(Vec3D& normal);

		  //apply forces directly to the body - an impulse is an
		  //instantaneous force application, so no dt is applied
		  void ApplyImpulse(Vec3D& impulse, Vec3D& contactvector);

		  //scale and change the body
		  void ChangeBody(float x, float y = 0);

		  void ChangeMaterial(Material& material);

		  //shape and material used to calculate MassData;
		  //shape and material are defined by the game object
		  Shape *shape_;
		  Material material_;
		  MassData massData_;

		  //used to tweak values on a specific object;
		  float gravityScale_;

		  //turns gravity on and off for a specific body;
		  unsigned int gravityOn_;
		  Vec3D gravityNormal_;

		  //body group - used for different groups of objects
		  //e.g. can be used to turn off gravity for all of one type of object - *not implemented*
		  unsigned int bodyGroup_;

		  //collisiongroup - used to ignore collision for various types of objects;
		  CollisionGroup collisionGroup_;

		  bool collisionCallback_;

		  //key forces and values for body
		  Vec3D velocity_;
		  Vec3D position_;
		  Vec3D force_;

		  //rotation values
		  float orientation_;
		  float angularVelocity_;
		  float torque_;

      //friction
      float frictionMod_;

		  Handle self;

		  void* userData;
  };

}