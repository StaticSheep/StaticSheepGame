#include "Manifold.h"
#include "Vec3D.h"

namespace SheepFizz
{

	//constructor - requires the two bodies
	Manifold::Manifold(Body* a, Body* b) : A(a), B(b) {}
	
	//Initialize is used after manifold interactions have been calculated to
	//determine how the bodies will react - it is not called unless
	//a manifold has a contact count above 0
	//calculates the manifold Resitution, StatFric, and DynamFric by taking
	//the minimum of each
	void Manifold::Initialize(void)
	{
		mResitution = Minimum(A->material_.GetMaterialResitution(), 
			B->material_.GetMaterialResitution());

		mStaticFriction = Minimum(A->material_.GetMaterialStaticFriction(), 
			B->material_.GetMaterialStaticFriction());

		mDynamicFriction = Minimum(A->material_.GetMaterialDynamicFriction(), 
			B->material_.GetMaterialDynamicFriction());
	}//end of Initialize

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

	//simply calls the circle rectangle manifold
	void RectangleCircleManifold(Manifold& m)
	{
		//create a temp manifold
		Manifold check(m.B, m.A);

		//use other manifold collision function
		CircleRectangleManifold(check);
		m.contactCount = check.contactCount;
	
		if(m.contactCount)
		{
			//switch the normal
			m.normal = -check.normal;

			m.penetration = check.penetration;

			//put in first contact
			m.contacts[0] = check.contacts[0];
		
			//check for second contact
			if(m.contactCount == 2)
				m.contacts[1] = check.contacts[1];
		}

	}//end of RectangleCircleManifold

	//this function calculates collision and collision points
	//between a circle and a rectangle
	void CircleRectangleManifold(Manifold& m)
	{
		//set contacts to zero
		m.contactCount = 0;

		//create a pointer for ease of access
		Rectangle* b = (Rectangle*)(m.B->shape_);

		//create rotation matrix for rectangle(B)
		Matrix2D bRotation(m.B->orientation_);

		//put the center of circle(A) in rectangle's(B) ref frame
		Vec3D circleCenter = m.A->position_;
		circleCenter -= m.B->position_;
		circleCenter = bRotation * circleCenter;

		float maxSeparation = -MAXVALUE;

		//used for marking which side of the rectangle is the
		//incident side
		unsigned int rectangleSide;

		for(unsigned int i = 0; i < MAXVERTICES; ++i)
		{
			//find the seperation between circle and rec
			float separation = (b->GetNormal(i)).DotProduct(circleCenter - b->GetVertex(i));

			//if separation exceeds radius, the shapes are not colliding
			if(separation > m.A->shape_->GetRadius())
				return;

			//otherwise, see if the separation is bigger
			//if so, it means that is the axis of furthest distance between the two
			//not incident sides will be less positive or negative
			if(separation > maxSeparation)
			{
				maxSeparation = separation;
				rectangleSide = i;
			}
		}

		//get two vertices of rectangle
		Vec3D rectangleVertex1 = b->GetVertex(rectangleSide);
		unsigned int rectangleSide2 = ((rectangleSide + 1) < MAXVERTICES) ? (rectangleSide + 1) : 0;
		Vec3D rectangleVertex2 = b->GetVertex(rectangleSide2);

		//create a side for dotproducts
		Vec3D side = rectangleVertex2 - rectangleVertex1;

		//determine if circle is within bounds of a side
		//if circleRecMagnitude1 is pos, and 2 is negative, it is within bounds
		//pos, pos implies that it is past vertex2's side
		//neg, neg implies it is past vertex1's side
		float circleRecMagnitude1 = side.DotProduct(circleCenter - rectangleVertex1);
		float circleRecMagnitude2 = side.DotProduct(circleCenter - rectangleVertex2);

		if(circleRecMagnitude1 < 0 && circleRecMagnitude2 < 0)
		{
			//if the circle center is at corner 1 - verify penetration is positive
			if((circleCenter - rectangleVertex1).SquareLength() > (m.A->shape_->GetRadius() * m.A->shape_->GetRadius()))
				return;
			m.penetration = m.A->shape_->GetRadius() - (circleCenter - rectangleVertex1).Length();
		
			//determine the normal
			m.normal = rectangleVertex1 - circleCenter;
			m.normal = bRotation * m.normal;
			m.normal.Normalize();

			//create contact point - take the vertex and put it into world space
			m.contacts[0] = m.B->position_ + bRotation * rectangleVertex1;
			m.contactCount = 1;
		}

		else if(circleRecMagnitude1 > 0 && circleRecMagnitude2 > 0)
		{
			//if the circle center is at corner 2 - verify penetration is positive
			if((circleCenter - rectangleVertex2).SquareLength() > (m.A->shape_->GetRadius() * m.A->shape_->GetRadius()))
				return;
			m.penetration = m.A->shape_->GetRadius() - (circleCenter - rectangleVertex2).Length();

			//determine the normal
			m.normal = rectangleVertex2 - circleCenter;
			m.normal = bRotation * m.normal;
			m.normal.Normalize();

			//create contact point - take the vertex and put it into world space
			m.contacts[0] = m.B->position_ + bRotation * rectangleVertex2;
			m.contactCount = 1;

		}

		else
		{
			//if the circle center is within the bounds of a side
			m.penetration = m.A->shape_->GetRadius() - maxSeparation;

			//determine the normal - negative because normal goes from A
			//to B (circle to rectangle)
			m.normal = -(b->GetNormal(rectangleSide));
			m.normal = bRotation * m.normal;

			//create contact point - take the vertex and put it into world space
			//contact point in this case will be the negative face normal added
			//to the position of the circleCenter
			m.contacts[0] = -((b->GetNormal(rectangleSide)) * m.A->shape_->GetRadius()) + circleCenter;
			m.contacts[0] = m.B->position_ + bRotation * m.contacts[0];
			m.contactCount = 1;
		}

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
