/******************************************************************************
Filename: RayCast.h
Project:  GAM 250
Author(s): Jonathan Sourbeer (primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once
#include "Manifold.h"
#include <vector>
#include "RayConfig.h"

namespace SheepFizz
{

  class RayCast
  {
    public:
      RayCast();
      ~RayCast();

      void Initialize(RayConfig* config);

      //construct the raycast - position, direction
      //pass in objects to test
      //circle and rec tests
      //create vector of objects to hold results
      //measure dist to sort
      //return true or false

      RayConfig* GetRayConfig();
      Body* GetFirstCollisionBody();
      Vec3D GetFirstCollisionPoint();

      bool SimpleRayTest(Body* body);
      bool ComplexRayTest(Body* body);

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
      bool RayRectangleIntersect(Vec3D& vertex, Vec3D& nextVertex, Vec3D& segmentDirection, Vec3D& contactPoint);

      //determine whether we should search for first collision
      //and where it occurs
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

      RayConfig* config_;
  };

  
}