
#ifdef SHEEPPHYSICS
#include "Manifold.h"
#endif

#pragma once
#include <vector>

namespace SheepFizz
{

  class RayCast
  {
    public:
      RayCast();
      ~RayCast();

      void Initialize(Vec3D& position, Vec3D& direction);

      //construct the raycast - position, direction
      //pass in objects to test
      //circle and rec tests
      //create vector of objects to hold results
      //measure dist to sort
      //return true or false

      bool RayTest(ObjectAllocator* bodies);

      bool SimpleRayTest(Body* body);
      bool ComplexRayTest(Body* body);

      void SetFindFirstCollision(bool findFirstCollision);

      void SetReflectRay(bool reflectRay);
      void FindReflectionDirection();

      //faster tests
      bool SimpleRayCircleTest(Body* circle);
      bool SimpleRayRectangleTest(Body* rectangle);

      //complex tests take longer but find point of contact
      bool ComplexRayCircleTest(Body* circle);
      bool ComplexRayRectangleTest(Body* rectangle);

    private:

      //helper functions for simple rec test detection
      Vec3D RayNormal(Body* rectangle);
      bool RaySupport(Rectangle* rectangle, Vec3D& normal, unsigned int vertex, unsigned int& support);
      bool RayRectangleIntersect(Vec3D& vertex, Vec3D& segmentDirection, Vec3D& contactPoint);

      std::vector<Body*> bodyIntersections_;  //will need to be passed by interface and removed

      //determine whether we should search for first collision
      //and where it occurs
      bool findFirstCollision_;
      Body* firstCollision_;
      Vec3D firstCollisionPoint_;
      
      //for comparison points - squared length for easy comparison
      float firstCollisionSquareLength_;

      //used for polygon collision
      unsigned int support_;
      Vec3D rayNormal_;

      //used for circle collision
      Vec3D circleNorm;

      //reflection of the ray - calculated only after the first
      //collision is found
      bool reflectRay_;
      Vec3D reflectionDirection_;

      //key values for the ray itself
      Vec3D position_;
      Vec3D direction_;

  };

  
}