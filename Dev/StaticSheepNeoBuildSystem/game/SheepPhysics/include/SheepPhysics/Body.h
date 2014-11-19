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

		  //function used to identify type of body
		  //and apply special functions to player

		  //turn gravity on and off in a direction

		  //used for expansion for other collisions

		  //apply forces directly to the body - an impulse is an
		  //instantaneous force application, so no dt is applied

		  //scale and change the body


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

		  Handle self;

		  void* userData;
  };

}
