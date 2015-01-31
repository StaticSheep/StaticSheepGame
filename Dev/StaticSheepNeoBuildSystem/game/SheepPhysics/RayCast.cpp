#include "precompiled.h"
#include "RayCast.h"

namespace SheepFizz
{

  //bool(*simpleraytests[2])(Body* body) = { SimpleRayCircleTest, SimpleRayRectangleTest };
  //bool(*complexraytests[2])(Body* body) = { ComplexRayCircleTest, ComplexRayRectangleTest };

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
    
    if (centerVec.SquareLength() <= (circle->shape_->GetRadius() * circle->shape_->GetRadius()))
      return true;

    return false;
  }

    //determines the direction the normal from the raycast should point relative to a body
  Vec3D RayCast::RayNormal(Body* rectangle)
  {
    Vec3D normalVec = rectangle->position_ - position_;
    normalVec = (normalVec * direction_) * direction_;
    normalVec = rectangle->position_ - normalVec;

    return normalVec;
  }

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
  }

  bool RayCast::SimpleRayRectangleTest(Body* rectangle)
  {
    Rectangle* rec = (Rectangle*)(rectangle->shape_);
    Vec3D normal;
    Vec3D rayNormal;

    for (unsigned int i = 0; i < rec->GetVertexNumber(); ++i)
    {
      normal = rec->GetNormal(i);

      if ((normal * direction_) >= 0)
        continue;

        //determine direction of ray normal
      rayNormal = RayNormal(rectangle);

      if (RaySupport(rec, rayNormal, i, support_))
        return true;
    }

    return false;
  }

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

    return true;
  }

  void FindFirstCollision()
  {


  }


}