#pragma once
#include <vector>
#include "Body.h"

namespace SheepFizz
{

  class RayCast
  {
    RayCast();
    ~RayCast();

    void Initialize(Vec3D& position, Vec3D& direction);

    //construct the raycast - position, direction
    //pass in objects to test
    //circle and rec tests
    //create vector of objects to hold results
    //measure dist to sort
    //return true or false

    bool SimpleRayTest(Body* body);
    bool ComplexRayTest(Body* body);

    void SetFindFirstCollision(bool findFirstCollision);
    void FindFirstCollision();

    void SetReflectRay(bool reflectRay);
    void FindReflectionDirection();

  private:
      //faster tests
    bool SimpleRayCircleTest(Body* circle);
    bool SimpleRayRectangleTest(Body* rectangle);

      //complex tests take longer but find point of contact
    bool ComplexRayCircleTest(Body* circle);
    bool ComplexRayRectangleTest(Body* rectangle);


    std::vector<Body*> intersections_;
    
    //determine whether we should search for first collision
    //and where it occurs
    Body* firstCollision_;
    bool findFirstCollision_;

    //reflection of the ray - calculated only after the first
    //collision is found
    bool reflectRay_;
    Vec3D reflectionDirection_;

    Vec3D position_;
    Vec3D direction_;

  };

}