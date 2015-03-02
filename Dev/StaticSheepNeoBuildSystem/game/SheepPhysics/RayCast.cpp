#include "precompiled.h"
#include "Handle.h"
#include "RayCast.h"

namespace SheepFizz
{

  //bool(*simpleraytests[2])(Body* body) = { SimpleRayCircleTest, SimpleRayRectangleTest };
  //bool(*complexraytests[2])(Body* body) = { ComplexRayCircleTest, ComplexRayRectangleTest };

  RayCast::RayCast(){}

  RayCast::~RayCast(){}

  void RayCast::Initialize(RayConfig* config)
  {
    config_ = config;

    position_ = config_->rayOrigin;
    direction_ = (config_->rayDirection).Normalize();
    
    //set values to null
    firstCollisionSquareLength_ = FLT_MAX;
    firstCollision_ = nullptr;
    reflectRay_ = false;
    config_->bodyIntersections_.clear();

  }//end of Initialize

  void RayCast::SetReflectRay(bool reflectRay)
  {
    reflectRay_ = reflectRay;
  }//end of SetFindFirstCollision

  RayConfig* RayCast::GetRayConfig()
  {
    return config_;
  }

  Body* RayCast::GetFirstCollisionBody()
  {
    return firstCollision_;
  }
  
  Vec3D RayCast::GetFirstCollisionPoint()
  {
    return firstCollisionPoint_;
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

    return rayIntersect;
  }//end of ComplexRayTest


  bool RayCast::SimpleRayCircleTest(Body* circle)
  {

    return false;

    float intersect = ((circle->position_.x - position_.x) * direction_.y 
      - (circle->position_.y - position_.y) * direction_.x);
        
    intersect = (intersect * intersect) / (direction_.x * direction_.x + direction_.y * direction_.y);

    if (intersect < (circle->shape_->GetRadius() * circle->shape_->GetRadius()))
      return true;

    return false;
  }//end of SimpleRayCircleTest


  bool RayCast::ComplexRayCircleTest(Body* circle)
  {
    if (!SimpleRayCircleTest(circle))
      return false;

    
    /*float segmentLength = ((Circle*)(circle->shape_))->GetRadius() *
      ((Circle*)(circle->shape_))->GetRadius() - circleNorm.SquareLength();

    Vec3D collisionPoint = circle->position_ + circleNorm;
    collisionPoint += direction_ * -segmentLength;

    Vec3D testLength = collisionPoint - position_;
    float length = testLength.SquareLength();

    if (length < firstCollisionSquareLength_)
    {
      firstCollisionSquareLength_ = length;
      firstCollision_ = circle;
    }*/

    return true;
  }//end of ComplexRayCircleTest

    
    //determines the direction the normal from the raycast should point relative to a body
  Vec3D RayCast::RayNormal(Body* rectangle)
  {
    Vec3D normalVec = rectangle->position_ - position_;
    normalVec = (normalVec * direction_) * direction_;
    normalVec = rectangle->position_ - normalVec;
    normalVec.Normalize();

    return normalVec;
  }//end of RayNormal


  bool RayCast::RaySupport(Rectangle* rectangle, Vec3D& normal, unsigned int vertexNumber, unsigned int& support)
  {
      //test vertex associated with rectangle
    Vec3D vertex = rectangle->GetVertex(vertexNumber);
    Matrix2D rot(((Body*)rectangle)->orientation_);
    vertex = rot * vertex;
    float rayDist = (((Body*)rectangle)->position_ - position_) * normal;
    float vertDist = vertex * normal;

    if ((vertDist - rayDist) < 0)
    {
      support = vertexNumber;
      return true;
    }

      //check next vertex, in case it qualifies, but is skipped by early exit in simple check
    vertexNumber = (vertexNumber + 1) < rectangle->GetVertexNumber() ? vertexNumber + 1 : 0;

    vertex = rectangle->GetVertex(vertexNumber);
    vertex = rot * vertex;
    rayDist = (((Body*)rectangle)->position_ - position_) * normal;
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
    return ComplexRayRectangleTest(rectangle);
    
    /*Rectangle* rec = (Rectangle*)(rectangle->shape_);
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

    return false;*/
  }//end of SimpleRayRectangleTest

    //determines the collision point for ComplexRayRectangleTest
  bool RayCast::RayRectangleIntersect(Vec3D& vertex, Vec3D& nextVertex, Vec3D& segmentDirection, Vec3D& collisionPoint)
  {
    if (position_.x > vertex.x && direction_.x > 0 && position_.x > nextVertex.x && direction_.x > 0)
      return false;

    if (position_.x < vertex.x && direction_.x < 0 && position_.x < nextVertex.x && direction_.x < 0)
      return false;

    if (position_.y < vertex.y && direction_.y < 0 && position_.y < nextVertex.y && direction_.y < 0)
      return false;

    if (position_.y > vertex.y && direction_.y > 0 && position_.y > nextVertex.y && direction_.y > 0)
      return false;

    float denominator = direction_.x * segmentDirection.y - direction_.y * segmentDirection.x;

    float intersection = direction_.y * (vertex.x - position_.x) + direction_.x * (position_.y - vertex.y);

    if (!denominator)
      denominator = 1;

    intersection /= denominator;
    if (intersection <= 0 || intersection >= 1)
      return false;
    segmentDirection.z = 0;
    collisionPoint = segmentDirection * intersection + vertex;

    return true;
  }//end of RayRectangleIntersect


  bool RayCast::ComplexRayRectangleTest(Body* rectangle)
  {
    //bool rayIntersect = SimpleRayRectangleTest(rectangle);

    //if (!rayIntersect)
        //return false;
    bool rayIntersect;
    bool atLeastOneIntersect = false;

    for (unsigned int i = 0; i < ((Rectangle*)(rectangle->shape_))->GetVertexNumber(); ++i)
    {
      support_ = i;
      Vec3D vertex = ((Rectangle*)(rectangle->shape_))->GetVertex(support_);
      unsigned int postsupport = support_ + 1 < ((Rectangle*)(rectangle->shape_))->GetVertexNumber() ? support_ + 1 : 0;
      Vec3D lineOne = ((Rectangle*)(rectangle->shape_))->GetVertex(postsupport);
      

      Matrix2D Rot(rectangle->orientation_);
      vertex = Rot * vertex;
      lineOne = Rot * lineOne;
      
      vertex += rectangle->position_;
      lineOne += rectangle->position_;
      Vec3D nextVert = lineOne;

      lineOne = lineOne - vertex;

      Vec3D collisionPoint;

      Vec3D testLength;
      float length;
      rayIntersect = RayRectangleIntersect(vertex, nextVert, lineOne, collisionPoint);

      if (rayIntersect)
      {
        atLeastOneIntersect = true;
        testLength = collisionPoint - position_;
        length = testLength.SquareLength();

        if (length < firstCollisionSquareLength_)
        {
          firstCollisionSquareLength_ = length;
          firstCollision_ = rectangle;
          firstCollisionPoint_ = collisionPoint;
        }
      }
    }

    if (atLeastOneIntersect)
      return true;

    return false;

    /*switch (((Rectangle*)(rectangle->shape_))->GetVertexNumber())
    {
      case 4:
        {
          //use support point to determine the two sides ray could be intersecting
          unsigned int presupport = (support_ - 1) >= 0 ? support_ - 1 : ((Rectangle*)(rectangle->shape_))->GetVertexNumber() - 1;
          unsigned int postsupport = support_ + 1 < ((Rectangle*)(rectangle->shape_))->GetVertexNumber() ? support_ + 1 : 0;


          //create segments and get their vector directions
          Vec3D vertex = ((Rectangle*)(rectangle->shape_))->GetVertex(support_);
          Vec3D lineOne = ((Rectangle*)(rectangle->shape_))->GetVertex(presupport);
          Vec3D lineTwo = ((Rectangle*)(rectangle->shape_))->GetVertex(postsupport);

          vertex += rectangle->position_;
          lineOne += rectangle->position_;
          lineTwo += rectangle->position_;

          lineOne = lineOne - vertex;
          lineTwo = lineTwo - vertex;

          Vec3D collisionPoint;

          Vec3D testLength;
          float length;

          //test segment one
          //if the segment does not intersect, it means that the
          //ray is passing through the opposite side segment
          //ugly, but should move faster than standard iteration
          if (RayRectangleIntersect(vertex, lineOne, collisionPoint))
          {
             testLength = collisionPoint - position_;
             length = testLength.SquareLength();

             if (length < firstCollisionSquareLength_)
             {
               firstCollisionSquareLength_ = length;
               firstCollision_ = rectangle;
               firstCollisionPoint_ = collisionPoint;
             }
          }

          else
          {
            unsigned int presupport2 = (presupport - 1) >= 0 ? presupport - 1 : ((Rectangle*)(rectangle->shape_))->GetVertexNumber() - 1;
            Vec3D vertex2 = ((Rectangle*)(rectangle->shape_))->GetVertex(presupport);
            lineOne = ((Rectangle*)(rectangle->shape_))->GetVertex(presupport2);

            vertex2 += rectangle->position_;
            lineOne += rectangle->position_;

            lineOne = lineOne - vertex2;

            if (RayRectangleIntersect(vertex, lineOne, collisionPoint))
            {
              testLength = collisionPoint - position_;
              length = testLength.SquareLength();

              if (length < firstCollisionSquareLength_)
              {
                firstCollisionSquareLength_ = length;
                firstCollision_ = rectangle;
                firstCollisionPoint_ = collisionPoint;
              }
            }
          }

          //test segment two
          if (RayRectangleIntersect(vertex, lineTwo, collisionPoint))
          {
            testLength = collisionPoint - position_;
            length = testLength.SquareLength();

            if (length < firstCollisionSquareLength_)
            {
              firstCollisionSquareLength_ = length;
              firstCollision_ = rectangle;
              firstCollisionPoint_ = collisionPoint;
            }
          }

          else
          {
            unsigned int postsupport2 = postsupport + 1 < ((Rectangle*)(rectangle->shape_))->GetVertexNumber() ? postsupport + 1 : 0;
            Vec3D vertex2 = ((Rectangle*)(rectangle->shape_))->GetVertex(postsupport);
            lineTwo = ((Rectangle*)(rectangle->shape_))->GetVertex(postsupport2);

            vertex2 += rectangle->position_;
            lineTwo += rectangle->position_;
            lineTwo = lineTwo - vertex2;

            if (RayRectangleIntersect(vertex, lineTwo, collisionPoint))
            {
              testLength = collisionPoint - position_;
              length = testLength.SquareLength();

              if (length < firstCollisionSquareLength_)
              {
                firstCollisionSquareLength_ = length;
                firstCollision_ = rectangle;
                firstCollisionPoint_ = collisionPoint;
              }
            }
          }        
        }

        break;

        //will need to be filled in with basically same as above for polygon collision
        //will be slower than box
      default:
        break;
    }*/
    
  }

}//end of ComplexRayRectangleTest