#pragma once
#include "Manifold.h"
#include <vector>
#include "RayConfig.h"

namespace SheepFizz
{

  class RayCast
  {
    public:


      //construct the raycast - position, direction
      //pass in objects to test
      //circle and rec tests
      //create vector of objects to hold results
      //measure dist to sort
      //return true or false




      //faster tests

      //complex tests take longer but find point of contact

    private:

      //helper functions for simple rec test detection

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
