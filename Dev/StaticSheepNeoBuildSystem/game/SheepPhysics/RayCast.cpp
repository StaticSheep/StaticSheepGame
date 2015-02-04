#include "precompiled.h"
#include "Handle.h"
#include "RayCast.h"

namespace SheepFizz
{

  //bool(*simpleraytests[2])(Body* body) = { SimpleRayCircleTest, SimpleRayRectangleTest };
  //bool(*complexraytests[2])(Body* body) = { ComplexRayCircleTest, ComplexRayRectangleTest };

  RayCast::RayCast(){}

  RayCast::~RayCast(){}

  void RayCast::Initialize(Vec3D& position, Vec3D& direction)
  {
    position_ = position;
    direction_ = direction.Normalize();
    findFirstCollision_ = false;
    firstCollisionSquareLength_ = FLT_MAX;
    firstCollision_ = nullptr;
    reflectRay_ = false;

    bodyIntersections_.clear();
  }//end of Initialize

  void RayCast::SetFindFirstCollision(bool findFirstCollision)
  {
    findFirstCollision_ = findFirstCollision;
  }//end of SetFindFirstCollision

  void RayCast::SetReflectRay(bool reflectRay)
  {
    reflectRay_ = reflectRay;
  }//end of SetFindFirstCollision

  bool RayCast::RayTest(ObjectAllocator* bodies_)
  {
    switch (findFirstCollision_)
    {
      case true:
        for (unsigned i = 0; i < (*bodies_).Size(); ++i)
          ComplexRayTest((Body*)(*bodies_)[i]);

        break;

      default:
        for (unsigned i = 0; i < (*bodies_).Size(); ++i)
          SimpleRayTest((Body*)(*bodies_)[i]);
        
        break;
    }

  }


  bool RayCast::SimpleRayTest(Body* body)
  {
    bool rayIntersect;

    switch (body->shape_->GetShape())
    {
      case Cir:
        rayIntersect = SimpleRayCircleTest(body);
        break;

      case Rec:
        rayIntersect = SimpleRayRectangleTest(body);
        break;

      default:
        break;
    }

    if (rayIntersect)
      bodyIntersections_.push_back(body);

    return rayIntersect;
  }//end of SimpleRayTest


  bool RayCast::ComplexRayTest(Body* body)
  {
    bool rayIntersect;

    switch (body->shape_->GetShape())
    {
    case Cir:
      rayIntersect = ComplexRayCircleTest(body);
      break;

    case Rec:
      rayIntersect = ComplexRayRectangleTest(body);
      break;

    default:
      break;
    }

    if (rayIntersect)
      bodyIntersections_.push_back(body);

    return rayIntersect;
  }//end of ComplexRayTest


  bool RayCast::SimpleRayCircleTest(Body* circle)
  {
      //create a vector between the circle and the ray's start
    circleNorm = circle->position_ - position_;

      //project it onto the direction
    circleNorm = (circleNorm * direction_) * direction_;

      //create a new vector perpendicular to it for test against the radius
    circleNorm = circleNorm - circle->position_;
    
    if (circleNorm.SquareLength() <= (circle->shape_->GetRadius() * circle->shape_->GetRadius()))
      return true;

    return false;
  }//end of SimpleRayCircleTest


  bool RayCast::ComplexRayCircleTest(Body* circle)
  {
    bool rayIntersect = SimpleRayCircleTest(circle);

    if (!rayIntersect)
      return false;

    float segmentLength = ((Circle*)(circle->shape_))->GetRadius() *
      ((Circle*)(circle->shape_))->GetRadius() - circleNorm.SquareLength();

    Vec3D collisionPoint = circle->position_ + circleNorm;
    collisionPoint += direction_ * -segmentLength;

    Vec3D testLength = collisionPoint - position_;
    float length = testLength.SquareLength();

    if (length < firstCollisionSquareLength_)
    {
      firstCollisionSquareLength_ = length;
      firstCollision_ = circle;
    }

    return true;
  }//end of ComplexRayCircleTest

    
    //determines the direction the normal from the raycast should point relative to a body
  Vec3D RayCast::RayNormal(Body* rectangle)
  {
    Vec3D normalVec = rectangle->position_ - position_;
    normalVec = (normalVec * direction_) * direction_;
    normalVec = rectangle->position_ - normalVec;

    return normalVec;
  }//end of RayNormal


  bool RayCast::RaySupport(Rectangle* rectangle, Vec3D& normal, unsigned int vertexNumber, unsigned int& support)
  {
      //test vertex associated with rectangle
    Vec3D vertex = rectangle->GetVertex(vertexNumber);
    float rayDist = position_ * normal;
    float vertDist = vertex * normal;

    if ((vertDist - rayDist) < 0)
    {
      support = vertexNumber;
      return true;
    }

      //check next vertex, in case it qualifies, but is skipped by early exit in simple check
    vertexNumber = (vertexNumber + 1) < rectangle->GetVertexNumber() ? vertexNumber + 1 : 0;

    vertex = rectangle->GetVertex(vertexNumber);
    rayDist = position_ * normal;
    vertDist = vertex * normal;

    if ((vertDist - rayDist) < 0)
    {
      support = vertexNumber;

      return true;
    }

    return false;
  }//end of RaySupport


  bool RayCast::SimpleRayRectangleTest(Body* rectangle)
  {
    Rectangle* rec = (Rectangle*)(rectangle->shape_);
    Vec3D normal;
   
    for (unsigned int i = 0; i < rec->GetVertexNumber(); ++i)
    {
      normal = rec->GetNormal(i);

      if ((normal * direction_) >= 0)
        continue;

        //determine direction of ray normal
      rayNormal_ = RayNormal(rectangle);

      if (RaySupport(rec, rayNormal_, i, support_))
        return true;
    }

    return false;
  }//end of SimpleRayRectangleTest

    //determines the collision point for ComplexRayRectangleTest
  bool RayCast::RayRectangleIntersect(Vec3D& vertex, Vec3D& segmentDirection, Vec3D& collisionPoint)
  {
    float denominator = direction_.x * segmentDirection.y - direction_.y * segmentDirection.x;
    if (!denominator)
      return false;

    float intersection = direction_.y * (vertex.x - position_.x) - direction_.x * (vertex.y - position_.y);
    intersection /= denominator;

    collisionPoint = segmentDirection * intersection + vertex;

    return true;
  }//end of RayRectangleIntersect


  bool RayCast::ComplexRayRectangleTest(Body* rectangle)
  {
    bool rayIntersect = SimpleRayRectangleTest(rectangle);

    if (!rayIntersect)
        return false;

    //use support point to determine the two sides ray could be intersecting
    unsigned int presupport = (support_ - 1) >= 0 ? support_ - 1 : ((Rectangle*)(rectangle->shape_))->GetVertexNumber() - 1;
    unsigned int postsupport = support_ + 1 < ((Rectangle*)(rectangle->shape_))->GetVertexNumber() ? support_ + 1 : 0;


    //create segments and get their vector directions
    Vec3D vertex = ((Rectangle*)(rectangle->shape_))->GetVertex(support_);
    Vec3D lineOne = ((Rectangle*)(rectangle->shape_))->GetVertex(presupport);
    Vec3D lineTwo = ((Rectangle*)(rectangle->shape_))->GetVertex(postsupport);

    lineOne = lineOne - vertex;
    lineTwo = lineTwo - vertex;

    Vec3D collisionPoint;

    //test segment one
    if (RayRectangleIntersect(vertex, lineOne, collisionPoint))
    {
      Vec3D testLength = collisionPoint - position_;
      float length = testLength.SquareLength();

      if (length < firstCollisionSquareLength_)
      {
        firstCollisionSquareLength_ = length;
        firstCollision_ = rectangle;
      }
    }
    
    //test segment two
    if (RayRectangleIntersect(vertex, lineTwo, collisionPoint))
    {
      Vec3D testLength = collisionPoint - position_;
      float length = testLength.SquareLength();

      if (length < firstCollisionSquareLength_)
      {
        firstCollisionSquareLength_ = length;
        firstCollision_ = rectangle;
      }
    }

    return true;
  }

}//end of ComplexRayRectangleTest