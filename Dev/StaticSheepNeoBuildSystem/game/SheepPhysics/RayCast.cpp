#include "precompiled.h"
#include "RayCast.h"

namespace SheepFizz
{

  void(*simpleraytest[2])(Body* body) = { SimpleRayCircleTest, SimpleRayRectangleTest };
  void(*complexraytest[2])(Body* body) = { ComplexRayCircleTest, ComplexRayRectangleTest };

  RayCast::RayCast()
  {

  }

  RayCast::~RayCast()
  {

  }

  void RayCast::Initialize(Vec3D& position, Vec3D& direction)
  {
    position_ = position;
    direction_ = direction.Normalize();
    findFirstCollision_ = false;
    reflectRay_ = false;
  }

  void RayCast::SetFindFirstCollision(bool findFirstCollision)
  {
    findFirstCollision_ = findFirstCollision;
  }

  void RayCast::SetReflectRay(bool reflectRay)
  {
    reflectRay_ = reflectRay;
  }

  bool RayCast::SimpleRayTest(Body* body)
  {
    bool rayIntersect = false;

    rayIntersect = simpleraytest[body->shape_->GetShape()];

    if (rayIntersect)
      intersections_.push_back(body);

    return rayIntersect;
  }

  bool RayCast::SimpleRayCircleTest(Body* circle)
  {
      //create a vector between the circle and the ray's start
    Vec3D centerVec = circle->position_ - position_;

      //project it onto the direction
    centerVec = (centerVec * direction_) * direction_;

      //create a new vector perpendicular to it for test against the radius
    centerVec = centerVec - circle->position_;
    
    if (centerVec.SquareLength <= (circle->shape_->GetRadius() * circle->shape_->GetRadius()))
      return true;

    return false;
  }

  bool RayCast::SimpleRayRectangleTest(Body* rectangle)
  {
    Rectangle* rec = (Rectangle*)(rectangle->shape_);

    for (unsigned int i = 0; i < rec->GetVertexNumber(); ++i)
    {
      Vec3D normal = rec->GetNormal(i);

      if ((normal * direction_) >= 0)
        return false;

    }

    return true;
  }

  bool RayCast::ComplexRayTest(Body* body)
  {
    bool rayIntersect = false;

    rayIntersect = complexraytest[body->shape_->GetShape()];

    if (rayIntersect)
      intersections_.push_back(body);

    return rayIntersect;
  }

  bool RayCast::ComplexRayCircleTest(Body* circle)
  {
    bool rayIntersect = SimpleRayCircleTest(circle);

    if (!rayIntersect)
      return false;
  


    return true;
  }

  bool RayCast::ComplexRayRectangleTest(Body* rectangle)
  {
    bool rayIntersect = SimpleRayRectangleTest(rectangle);

    if (!rayIntersect)
        return false;

    
  }

  void FindFirstCollision()
  {


  }


}