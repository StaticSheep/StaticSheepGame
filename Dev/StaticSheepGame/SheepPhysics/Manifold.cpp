#include "Manifold.h"

namespace PHYSICS
{

//complex declaration - 2D array of function pointers that take a pointer to a manifold and returns a void
//uses an enumeration for the array values
void (*manifold[2][2])(Manifold& m) = {{RectangleRectangleManifold, RectangleCircleManifold},
	{CircleRectangleManifold, CircleCircleManifold}};


//uses enums for instant access to the array values
void Manifold::ManifoldInteraction(void)
{
	//check collision group
	if(A->collisionGroup_ == B->collisionGroup_)
		manifold[A->shape_->GetShape()][B->shape_->GetShape()](*this);
}//end of ManifoldInteraction


//apply forces in the direction of the normals determined in
//collision functions
void Manifold::ApplyForces(void)
{
	//if both objects have infinite mass, skip calculations
	if(A->massData_.mass == 0 && B->massData_.mass == 0)
		return;

	//the force must be applied across all contact points
	for(int i = 0; i < contactCount; ++i)
	{
		//this is broken into two primary sections, a seperation
		//force calculation (impulse) and a friction calculation

		//*******************
		//calculate impulse
		//*******************
		//calculate the relative vectors between the center of mass
		//and each of the contact points
		Vec3D aRepulsionVec = contacts[i] - A->position_;
		Vec3D bRepulsionVec = contacts[i] - B->position_;

		//determine the relative velocity to determine opposite impulse force
		//calculate as if A is stationary and B is moving
		//equation is relveloc = velocity + Cross(angularvelocity, relative vec)
		Vec3D relativevelocity = B->velocity_ + 
			Vec3D(0,0,B->angularVelocity_) * bRepulsionVec 
			- A->velocity_ - Vec3D(0,0,A->angularVelocity_) * aRepulsionVec;

		//determine the contact velocity - the relative velocity along the 
		//collision normal
		float contactVelocity = relativevelocity.DotProduct(normal);

		//if contactVelocity is greater than 0, it means that the objects are
		//seperating and we don't want to screw it up
		if(contactVelocity > 0)
			return;

		//determine the angular vectors - cross the aRepVec with norm
		//will give only z value - equivalent to scalar
		Vec3D aRepulsionCrossNorm = aRepulsionVec * normal;
		Vec3D bRepulsionCrossNorm = bRepulsionVec * normal;

		//determine the inverse mass sum of the two bodies
		//a combination of the total mass plus the moments of inertia
		//used in calculation of scalar to multiply along collision
		//normal for final calculation of impulse
		float inverseMassSum = A->massData_.inverseMass + B->massData_.inverseMass +
			(aRepulsionCrossNorm.z_ * aRepulsionCrossNorm.z_ * A->massData_.inverseInertia) +
			(bRepulsionCrossNorm.z_ * bRepulsionCrossNorm.z_ * B->massData_.inverseInertia);

		//calculate impulse scalar : "j"
		float j = -(1.0f + mResitution) * contactVelocity / inverseMassSum;

		//divide by number of contact points
		j /= contactCount;


		//calculate impulse and apply to bodies
		//A's impulse is negative because the normal is
		//from B to A;
		Vec3D impulse = j * normal;
		A->ApplyImpulse(-impulse, aRepulsionVec);
		B->ApplyImpulse(impulse, bRepulsionVec);

		//*******************
		//calculate friction
		//*******************
		//uses the same basic equations as impulse, but replaces normal vector
		//with tangent vector - in the direction closest to the relative velocity

		//calculate the normalized tangent vector
		//by removing the relative velocity component along the normal, only
		//the tangent remains (non-normalized)
		Vec3D tangent = relativevelocity - (normal * contactVelocity);
		tangent.Normalize();

		//calculate friction contact velocity - negative to tangentVector
		float frictionContactVelocity = relativevelocity.DotProduct(-tangent);

		//calculate friction scalar
		float jFriction = frictionContactVelocity / inverseMassSum;
		jFriction /= contactCount;


		//calculate friction impulse - use Coulomb's Law of Friction
		//friction force cannot exceed normal impulse
		Vec3D frictionImpulse;
		if(std::abs(jFriction) < j * mStaticFriction)
			frictionImpulse = jFriction * tangent;
		else
			frictionImpulse = tangent * -j * mDynamicFriction;

		//apply friction impulse
		A->ApplyImpulse(-frictionImpulse, aRepulsionVec);
		B->ApplyImpulse(frictionImpulse, bRepulsionVec);
	}

}//end of ApplyForces

//positional correction is designed to prevent sinking of one
//object into another
void Manifold::PositionalCorrection(void)
{
	Vec3D correction = (Maximum(penetration - POSSLACK, 0.0f) /
		(A->massData_.inverseMass + B->massData_.inverseMass)) * POSCORRECT
		* normal;
	A->position_ -= A->massData_.inverseMass * correction;
	B->position_ += B->massData_.inverseMass * correction;

}//end of PositionalCorrection

void RectangleRectangleManifold(Manifold& m)
{
	//create shortcut pointers to each shape
	Shape* a = m.A->shape_;
	Shape* b = m.B->shape_;

	//record the centers of both bodies
	Vec3D aCenter = m.A->position_;
	Vec3D bCenter = m.B->position_;

	//create world orientation matrices for both bodies
	Matrix2D aRotation(m.A->orientation_);
	Matrix2D bRotation(m.B->orientation_);

}//end of RectangleRectangleManifold

void RectangleCircleManifold(Manifold& m)
{
	m.contactCount = 0;

	Shape* a = m.A->shape_;
	Shape* b = m.B->shape_;

	Vec3D aCenter = m.A->position_;
	Vec3D bCenter = m.B->position_;




}//end of RectangleCircleManifold

void CircleRectangleManifold(Manifold& m)
{

	Manifold temp(m.B, m.A);
	RectangleCircleManifold(temp);
	m.normal = -temp.normal;
	m.penetration = temp.penetration;
	m.contactCount = temp.contactCount;
	m.contacts[0] = temp.contacts[0];
	m.contacts[1] = temp.contacts[1];
	
}//end of CircleRectangleManifold


void CircleCircleManifold(Manifold& m)
{
	//preset the contact count to 0;
	m.contactCount = 0;

	//create pointers to shapes for ease of use
	Shape* a = m.A->shape_;
	Shape* b = m.B->shape_;

	//test collision - collision resolution will be applied 
	//during 'step' functions
	//**************
	//get critical values first - length and two centers
	float collisionLength = a->GetRadius() + b->GetRadius();
	Vec3D aCenter = m.A->position_;
	Vec3D bCenter = m.B->position_;

	//create a normal vector from A to B;
	Vec3D circNormal = bCenter - aCenter;

	//if normal is longer than the two radii, exit
	if(circNormal.Length() > collisionLength)
		return;

	//determine penetration
	m.penetration = collisionLength - circNormal.Length();

	//normalize circNormal vector for future use
	circNormal.Normalize();

	//determine contact points - only 1 point assumed for
	//circles
	m.contactCount = 1;
	m.contacts[0] = aCenter + circNormal * 
		(a->GetRadius() - m.penetration);

	m.normal = circNormal;
	//**************
	
}//end of CircleCircleManifold

}
