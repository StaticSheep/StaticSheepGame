#include "Material.h"
#include "Shape.h"

namespace SheepFizz
{

struct MassData
{
	float mass;
	float inverseMass;

	float inertia;			//used for angular calculations
	float inverseInertia;
};

enum BodyGroup{

	BodyGroup1,
	BodyGroup2,
	BodyGroup3,
	BodyGroupLength
};

enum CollisionGroup{

	CollGroup1,
	CollGroup2,
	CollGroupLength

};


struct Body

		//used in initialization of body - computes mass

		//apply forces directly to the body - an impulse is an

		//shape and material used to calculate MassData;
		//shape and materail are defined by the gameobject
		Shape *shape_;
		Material material_;
		MassData massData_;

		//used to tweak values on a specific object;
		float gravityScale_;

		//turns gravity on and off for a specific body;
		unsigned int gravityOn_;

		//body group - used for different groups of objects
		//e.g. can be used to turn off gravity for all of one type of object - *not implemented*
		BodyGroup bodyGroup_;

		//collisiongroup - used to ignore collision for various types of objects;
		CollisionGroup collisionGroup_;

		//key forces and values for body
		Vec3D velocity_;
		Vec3D position_;
		Vec3D force_;

		//rotation values
		float orientation_;
		float angularVelocity_;
		float torque_;


};

}
