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
    Vec3D centerVec = circle->position_ - position_;
    centerVec = (centerVec * direction_) * direction_;
    centerVec = centerVec - circle->position_;
    
    if (centerVec.SquareLength <= (circle->shape_->GetRadius() * circle->shape_->GetRadius()))
      return true;

    return false;
  }

  bool RayCast::SimpleRayRectangleTest(Body* rectangle)
  {

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

  }

  void FindFirstCollision()
  {


  }


}