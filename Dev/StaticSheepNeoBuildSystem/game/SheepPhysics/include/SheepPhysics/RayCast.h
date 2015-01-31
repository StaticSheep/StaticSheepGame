
#ifdef SHEEPPHYSICS
#include "Manifold.h"
#endif

#pragma once
#include <vector>

namespace SheepFizz
{
  class Body;
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

      std::vector<Body*> intersections_;
    
      //determine whether we should search for first collision
      //and where it occurs
      Body* firstCollision_;
      bool findFirstCollision_;

      //used for polygon collision
      unsigned int support_;

      //reflection of the ray - calculated only after the first
      //collision is found
      bool reflectRay_;
      Vec3D reflectionDirection_;

      Vec3D position_;
      Vec3D direction_;

  };

  
}
