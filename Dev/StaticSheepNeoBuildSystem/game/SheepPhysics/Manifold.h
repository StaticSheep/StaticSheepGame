#pragma once
#include "Body.h"

namespace SheepFizz
{

	#define POSCORRECT	0.4f	//the percent for positional correction
	#define POSSLACK	0.05f	//the penetration needed to start positional correction
	#define MAXVALUE	3.402823466e+38F  //the max value of a float, used in rec collision

	//manifold struct is used for collisions
	//it gives the collision normal and depth of penetration for
	//later modifications
	class Manifold
	{
		public:
		//constructor - requires the two bodies
		Manifold(Body* a, Body* b);
	
		//Initialize is used after manifold interactions have been calculated to
		//determine how the bodies will react - it is not called unless
		//a manifold has a contact count above 0
		//calculates the manifold Resitution, StatFric, and DynamFric by taking
		//the minimum of each
		void Initialize(void);

		//this function simply contains the jump table used to go to other
		//collision and resolution functions
		void ManifoldInteraction(void);

		//this function applies the forces derived from the manifold
		void ApplyForces(void);

		//adjusts for sinking objects and rounding errors
		void PositionalCorrection(void);

		//the two bodies involved in the possible collision
		Body *A;
		Body *B;

		Vec3D normal;				        //the normal from A to B
		Vec3D contacts[2];			    //the contact points

		float penetration;			    //depth of penetration between objects along normal
		float mResitution;			    //the restitution for the two objects
		float mDynamicFriction;		  //the dynamic friction of the two objects
		float mStaticFriction;		  //the static friction of the two objects

		unsigned int contactCount;	    //the number of contacts in the manifold

    //accumulated impulses for Legragian
    float accumulatedImpulse[2];    
    float accumulatedTanImpulse[2];
	
	};

	//collision resolution functions
	void RectangleCircleManifold(Manifold& m);
	void CircleRectangleManifold(Manifold& m);
	void RectangleRectangleManifold(Manifold& m);
	void CircleCircleManifold(Manifold& m);


}