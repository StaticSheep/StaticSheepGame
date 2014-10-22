#include "Body.h"

#define POSCORRECT	0.4f	//the percent for positional correction
#define POSSLACK	0.05f	//the penetration needed to start positional correction

//manifold struct is used for collisions
//it gives the collision normal and depth of penetration for
//later modifications
struct Manifold
{
	//constructor - requires the two bodies
	Manifold(Body* a, Body* b) : A(a), B(b) {}
	
	//Initialize is used after manifold interactions have been calculated to
	//determine how the bodies will react - it is not called unless
	//a manifold has a contact count above 0
	//calculates the manifold Resitution, StatFric, and DynamFric by taking
	//the minimum of each
	void Initialize(void)
	{
		mResitution = Minimum(A->material_.GetMaterialResitution(), 
			B->material_.GetMaterialResitution());

		mStaticFriction = Minimum(A->material_.GetMaterialStaticFriction(), 
			B->material_.GetMaterialStaticFriction());

		mDynamicFriction = Minimum(A->material_.GetMaterialDynamicFriction(), 
			B->material_.GetMaterialDynamicFriction());
	}//end of Initialize

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

	Vec3D normal;				//the normal from A to B
	Vec3D contacts[2];			//the contact points

	float penetration;			//depth of penetration between objects along normal
	float contactCount;			//the number of contacts in the manifold
	float mResitution;			//the restitution for the two objects
	float mDynamicFriction;		//the dynamic friction of the two objects
	float mStaticFriction;		//the static friction of the two objects
	
};

//collision resolution functions
void RectangleCircleManifold(Manifold& m);
void CircleRectangleManifold(Manifold& m);
void RectangleRectangleManifold(Manifold& m);
void CircleCircleManifold(Manifold& m);

