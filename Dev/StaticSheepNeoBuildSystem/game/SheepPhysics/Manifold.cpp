#include "precompiled.h"
#include "Manifold.h"
#include "Vec3D.h"

namespace SheepFizz
{

	//constructor - requires the two bodies
	Manifold::Manifold(Body* a, Body* b) : A(a), B(b) {}
	
  //*******************
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
    
    for (unsigned int i = 0; i < 2; ++i)
    {
      accumulatedImpulse[i] = 0;
      accumulatedTanImpulse[i] = 0;
    }
    
	}//end of Initialize

	//complex declaration - 2D array of function pointers that take a pointer to a manifold and returns a void
	//uses an enumeration for the array values
	void (*manifold[2][2])(Manifold& m) = {{RectangleRectangleManifold, RectangleCircleManifold},
		{CircleRectangleManifold, CircleCircleManifold}};


	//uses enums for instant access to the array values
	void Manifold::ManifoldInteraction(void)
	{
			manifold[A->shape_->GetShape()][B->shape_->GetShape()](*this);
	}//end of ManifoldInteraction


  //*******************
	//positional correction is designed to prevent sinking of one
	//object into another
	void Manifold::PositionalCorrection(void)
	{
    //if both objects have infinite mass, skip calculations
    if(A->massData_.mass == 0 && B->massData_.mass == 0)
      return;

		Vec3D correction = (Maximum(penetration - POSSLACK, 0.0f) /
			(A->massData_.inverseMass + B->massData_.inverseMass)) * POSCORRECT
			* normal;

    correction.z = 0;

		A->position_ -= A->massData_.inverseMass * correction;
		B->position_ += B->massData_.inverseMass * correction;

	}//end of PositionalCorrection


  //*******************
	//determine the support point on body B relative to the face of body A
	float SupportPoint(Body& A, Body& B, Vec3D vertex, Vec3D normal, unsigned int& supportPoint)
	{
	float supportPenetration;
	float minMagnitude = MAXVALUE;

	//get the rotation matrices of each body
	Matrix2D aRotation(A.orientation_);
	Matrix2D bRotation(B.orientation_);

  unsigned int BvertexNumber = ((Rectangle*)B.shape_)->GetVertexNumber();

	//pull a vertex from B to check against A
  for (unsigned int i = 0; i < BvertexNumber; ++i)
	{
		//grab vertex from Body B - will act as support point
		Vec3D vertexB = ((Rectangle*)B.shape_)->GetVertex(i);

		//move vertexB into world space
		vertexB = bRotation * vertexB;
		vertexB += B.position_;

		//move it into A's space
		vertexB -= A.position_;
		vertexB = aRotation.Transpose() * vertexB;

		//get the penetration of the support point
		supportPenetration = vertexB.DotProduct(normal) - vertex.DotProduct(normal);

		//if the supportPenetration is smaller, it means the point is closer
		//a negative value is required for penetration
		if(supportPenetration < minMagnitude)
		{
		minMagnitude = supportPenetration;
		supportPoint = i;
		}
	}

	return minMagnitude;
	}//end of SupportPoint


  //*******************
	//find the incident face vertices of the incident body upon the reference body
	//the incident vertices will be in the reference body's frame
  void IncidentFace(Vec3D* incidentFaceVertices, Body* referenceBody, Body* incidentBody, 
	unsigned int referenceVertex, unsigned int supportPoint)
	{
	  Vec3D refNormal = ((Rectangle*)referenceBody->shape_)->GetNormal(referenceVertex);

    unsigned int incVertexNumber = ((Rectangle*)incidentBody->shape_)->GetVertexNumber();

	  //get the two normals possible for the incident body - the support point normal
	  //and the normal of the support point - 1
	  Vec3D supportNormal = ((Rectangle*)incidentBody->shape_)->GetNormal(supportPoint);
    unsigned int supportPoint2 = (supportPoint + 1) < incVertexNumber ? supportPoint + 1 : 0;
	  Vec3D preSupportNormal = ((Rectangle*)incidentBody->shape_)->GetNormal(supportPoint2);

	  //translate the normals into the proper reference frame
	  Matrix2D refRotation(referenceBody->orientation_);
	  Matrix2D incRotation(incidentBody->orientation_);

	  supportNormal = refRotation.Transpose() * (incRotation * supportNormal);
	  preSupportNormal = refRotation.Transpose() * (incRotation * preSupportNormal);

	  //determine which face to use - the most negative result will provide the best face
	  float supportMagnitude = refNormal.DotProduct(supportNormal);
	  float preSupportMagnitude = refNormal.DotProduct(preSupportNormal);

	  //determine which is the first face vertex and which is the second
	  //support point and support point + 1
	  if(supportMagnitude < preSupportMagnitude)
	  {
	  incidentFaceVertices[0] = ((Rectangle*)incidentBody->shape_)->GetVertex(supportPoint);
    supportPoint = (supportPoint + 1) < incVertexNumber ? supportPoint + 1 : 0;
	  incidentFaceVertices[1] = ((Rectangle*)incidentBody->shape_)->GetVertex(supportPoint);
	  }

	  //support point - 1 and support point
	  else
	  {
	  incidentFaceVertices[1] = ((Rectangle*)incidentBody->shape_)->GetVertex(supportPoint);
	  supportPoint = (supportPoint - 1) > -1 ? supportPoint - 1 : 3;		
	  incidentFaceVertices[0] = ((Rectangle*)incidentBody->shape_)->GetVertex(supportPoint);
	  }

	  //move the face vertices into the reference body's frame
	  incidentFaceVertices[0] = incRotation * incidentFaceVertices[0];
	  incidentFaceVertices[0] += incidentBody->position_;
	  incidentFaceVertices[0] -= referenceBody->position_;
	  incidentFaceVertices[0] = refRotation.Transpose() * incidentFaceVertices[0];

	  incidentFaceVertices[1] = incRotation * incidentFaceVertices[1];
	  incidentFaceVertices[1] += incidentBody->position_;
	  incidentFaceVertices[1] -= referenceBody->position_;
	  incidentFaceVertices[1] = refRotation.Transpose() * incidentFaceVertices[1];

	}//end of IncidentFace


  //*******************
	//determines if two polygons are colliding
	void RectangleRectangleManifold(Manifold& m)
	{
    //preset the contact count to 0;
    m.contactCount = 0;

	  //create shortcut pointers to each shape
	  Rectangle* a = (Rectangle*)(m.A->shape_);
	  Rectangle* b = (Rectangle*)(m.B->shape_);

	  //arrays to hold the support points of each object
	  //support point is the point on other object that
	  //is 'closest' to first object
	  unsigned int supportA;
	  unsigned int supportB;

	  //used to hold the support point in the support point
	  //function
	  unsigned int tempSupport;

	  //hold the smallest penetration for each shape
	  //used for collision detection and resolution of contact points
	  float leastPenetrationA = -MAXVALUE;
	  float leastPenetrationB = -MAXVALUE;
	  float tempPenetration;

	  //record the contact vertex/normal
	  unsigned int vertexA;
	  unsigned int vertexB;

	  //determine if rectangles/polys are colliding*****************
	  //test body A
    unsigned int AVertexNumber = ((Rectangle*)m.A->shape_)->GetVertexNumber();

    for (unsigned int i = 0; i < AVertexNumber; ++i)
	  {
	    tempPenetration = SupportPoint(*(m.A), *(m.B), a->GetVertex(i), 
	    a->GetNormal(i), tempSupport);

	    //if the penetration is greater than zero, axis of seperation found
	    if(tempPenetration > 0.0f)
	      return;

	    //find the least penetration - it should be negative, but the
	    //least negative
	    if(tempPenetration > leastPenetrationA)
	    {
	      //set penetration and support point
	      leastPenetrationA = tempPenetration;
	      supportA = tempSupport;
	      vertexA = i;
	    }
	  }

	  //test body B
    unsigned int BVertexNumber = ((Rectangle*)m.B->shape_)->GetVertexNumber();

	  for(unsigned int i = 0; i < BVertexNumber; ++i)
	  {
	    tempPenetration = SupportPoint(*(m.B), *(m.A), b->GetVertex(i), 
	    b->GetNormal(i), tempSupport);

	    //if the penetration is greater than zero, axis of seperation found
	    if(tempPenetration > 0.0f)
	      return;

	    //find the least penetration - it should be negative, but the
	    //least negative
	    if(tempPenetration > leastPenetrationB)
	    {
	      //set penetration and support point
	      leastPenetrationB = tempPenetration;
	      supportB = tempSupport;
	      vertexB = i;
	    }
	  }
	  //if this point is reached, the two objects are colliding
	  //end of supportpoint testing and SAT***********

	  //set ref and inc bodies***********
	  //create holding values for reference and incident faces
	  Body* referenceBody;
	  unsigned int referenceBodyVertex;

	  Body* incidentBody;
	  unsigned int incidentBodySupport;

	  //add a flag for the case where the B body becomes the
	  //reference face
	  bool flip = false;

	  //determine which object had the least penetration upon it
	  //and set the appropriate values
	  if(leastPenetrationA > leastPenetrationB)
	  {
	    referenceBody = m.A;
	    referenceBodyVertex = vertexA;

	    incidentBody = m.B;
	    incidentBodySupport = supportA;
	  }

	  else
	  {
	  referenceBody = m.B;
	  referenceBodyVertex = vertexB;

	  incidentBody = m.A;
	  incidentBodySupport = supportB;

	  flip = true;
	  }
	  //end of setting ref and inc bodies***********

	  //determine incident face
	  Vec3D incidentFaceVertices[2];
	  IncidentFace(incidentFaceVertices, referenceBody, incidentBody, referenceBodyVertex, incidentBodySupport);

    unsigned int refVertexNumber = ((Rectangle*)referenceBody->shape_)->GetVertexNumber();

	  //clipping***********
	  //find direction vector for reference face
	  Vec3D refVertex1 = ((Rectangle*)referenceBody->shape_)->GetVertex(referenceBodyVertex);
	  unsigned int refVertexNext = (referenceBodyVertex + 1 < refVertexNumber) ? referenceBodyVertex + 1 : 0;
	  Vec3D refVertex2 = ((Rectangle*)referenceBody->shape_)->GetVertex(refVertexNext);
	  Vec3D refDirection = refVertex2 - refVertex1;

	  refDirection.Normalize();

	  //find the incident face direction
	  Vec3D incDirection = incidentFaceVertices[1] - incidentFaceVertices[0];

	  //determine if the two incident faces are outside the bounds of the side
	  //keep track of the amount that needs to be clipped by determining the distance
	  //of each vertex from the side being tested
	  //the ref face with the face vertex
	  float incidentDirection1 = refDirection.DotProduct(incidentFaceVertices[0]);
	  //the other face vertex with ref face (opposite direction)
	  float opDirection1 = refDirection.DotProduct(incidentFaceVertices[1]);
	  //the edge of the face with the ref face
	  float sideDirection1 = refDirection.DotProduct(refVertex2);
	  //the magnitude of the face vert with the edge of ref face
	  float incMagnitude1 = incidentDirection1 - sideDirection1;
	  //the other face with the edge of the ref face
	  float opMagnitude1 = sideDirection1 - opDirection1;

	  //clip other side
	  float incidentDirection2 = (-refDirection).DotProduct(incidentFaceVertices[1]);
	  float opDirection2 = (-refDirection).DotProduct(incidentFaceVertices[0]);
	  float sideDirection2 = (-refDirection).DotProduct(refVertex1);
	  float incMagnitude2 = incidentDirection2 - sideDirection2;
	  float opMagnitude2 = sideDirection2 - opDirection2;

	  //the ratio for moving along the incDirection
	  float incidentRatio;

	  //clip sides
	  if(incMagnitude1 > 0)
	  {
	  incidentRatio = incMagnitude1 / (incMagnitude1 + opMagnitude1);
	  incidentFaceVertices[0] = incidentRatio * incDirection + incidentFaceVertices[0];
	  }

	  if(incMagnitude2 > 0)
	  {
	  incidentRatio = incMagnitude2 / (incMagnitude2 + opMagnitude2);
	  incidentFaceVertices[1] = -incidentRatio * incDirection + incidentFaceVertices[1];
	  }

	  //set the normal for the manifold
	  m.normal = ((Rectangle*)referenceBody->shape_)->GetNormal(referenceBodyVertex);

    if (m.normal.x < EPSILON && m.normal.x > -EPSILON)
      m.normal.x = 0;

    if (m.normal.y < EPSILON && m.normal.y > -EPSILON)
      m.normal.y = 0;

    if (m.normal.z < EPSILON && m.normal.z > -EPSILON)
      m.normal.z = 0;

	  //get the rotation matrix for the orientation
	  Matrix2D refRotation(referenceBody->orientation_);

	  //test if the incident vertices are behind the reference face - set the contact points
	  //back into world space
	  if(incidentFaceVertices[0].DotProduct(m.normal) - (refVertex1.DotProduct(m.normal)) < 0)
	  {
	  m.penetration = -(incidentFaceVertices[0].DotProduct(m.normal) - (refVertex1.DotProduct(m.normal)));
	  m.contacts[m.contactCount++] = referenceBody->position_ + refRotation * incidentFaceVertices[0];
	  }

	  if(incidentFaceVertices[1].DotProduct(m.normal) - (refVertex1.DotProduct(m.normal)) < 0)
	  {
	  m.penetration -= (incidentFaceVertices[1].DotProduct(m.normal) - (refVertex1.DotProduct(m.normal)));
	  m.contacts[m.contactCount++] = referenceBody->position_ + refRotation * incidentFaceVertices[1];
	  }
	  //end of clipping***********

	  //shift the reference normal back into world space
	  m.normal = refRotation * m.normal;

	  //flip the normal if B is the reference face and average the penetration
	  if(flip)
	  m.normal = -(m.normal);

	  if(m.penetration > 2)
	    m.penetration = 4;

	  m.penetration /= m.contactCount;

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
		if(m.A->massData_.mass == 0 && m.B->massData_.mass == 0)
			return;

		//set contacts to zero
		m.contactCount = 0;

		//create a pointer for ease of access
		Rectangle* b = (Rectangle*)(m.B->shape_);

		//create rotation matrix for rectangle(B)
		Matrix2D bRotation(m.B->orientation_);

		//put the center of circle(A) in rectangle's(B) ref frame
		Vec3D circleCenter = m.A->position_;
		circleCenter -= m.B->position_;
		circleCenter = bRotation.Transpose() * circleCenter;

		float maxSeparation = -MAXVALUE;

		//used for marking which side of the rectangle is the
		//incident side
		unsigned int rectangleSide;

    unsigned int BVertexNumber = b->GetVertexNumber();

    for (unsigned int i = 0; i < BVertexNumber; ++i)
		{
			//find the separation between circle and rec
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
		unsigned int rectangleSide2 = ((rectangleSide + 1) < BVertexNumber) ? (rectangleSide + 1) : 0;
		Vec3D rectangleVertex2 = b->GetVertex(rectangleSide2);

		//create a side for dot products
		Vec3D side = rectangleVertex2 - rectangleVertex1;

		//determine if circle is within bounds of a side
		//if circleRecMagnitude1 is pos, and 2 is negative, it is within bounds
		//pos, pos implies that it is past vertex2's side
		//neg, neg implies it is past vertex1's side
		float circleRecMagnitude1 = side.DotProduct(circleCenter - rectangleVertex1);
		float circleRecMagnitude2 = side.DotProduct(circleCenter - rectangleVertex2);

		if(circleRecMagnitude1 <= 0 && circleRecMagnitude2 <= 0)
		{
			//if the circle center is at corner 1
			if((circleCenter - rectangleVertex1).SquareLength() > (m.A->shape_->GetRadius() * m.A->shape_->GetRadius()))
				return;
			m.penetration = m.A->shape_->GetRadius() - (circleCenter - rectangleVertex1).Length();
		
			//determine the normal
			m.normal = rectangleVertex1 - circleCenter;
			m.normal = bRotation * m.normal;
			m.normal.Normalize();

			//create contact point - take the vertex and put it into world space
			m.contacts[0] = m.B->position_ + (bRotation * rectangleVertex1);
			m.contactCount = 1;
		}

		else if(circleRecMagnitude1 > 0 && circleRecMagnitude2 > 0)
		{
			//if the circle center is at corner 2
			if((circleCenter - rectangleVertex2).SquareLength() > (m.A->shape_->GetRadius() * m.A->shape_->GetRadius()))
				return;
			m.penetration = m.A->shape_->GetRadius() - (circleCenter - rectangleVertex2).Length();

			//determine the normal
			m.normal = rectangleVertex2 - circleCenter;
			m.normal = bRotation * m.normal;
			m.normal.Normalize();

			//create contact point - take the vertex and put it into world space
			m.contacts[0] = m.B->position_ + (bRotation * rectangleVertex2);
			m.contactCount = 1;

		}

		else
		{
			//if the circle center is within the bounds of a side
			m.penetration = m.A->shape_->GetRadius() - maxSeparation;

			//determine the normal - negative because normal goes from A
			//to B (circle to rectangle)
			m.normal = (b->GetNormal(rectangleSide));

			//if(m.normal.DotProduct(circleCenter - rectangleVertex1)> m.A->shape_->GetRadius())
				//return;

			m.normal = bRotation * -m.normal;

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
