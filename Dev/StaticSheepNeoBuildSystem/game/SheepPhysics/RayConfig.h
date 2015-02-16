#pragma once

#include "Matrix.h"
#include "ObjectAllocator.h"
#include "Handle.h"
#include "HandleManager.h"
#include <vector>

namespace SheepFizz
{
  struct RayConfig
  {
    //set by client
    Vec3D rayOrigin;
    Vec3D rayDirection;
    bool findFirstCollision;
    CollisionGroup collisionGroup;
  
    //pointer to the current gamespace, for collision resolution
    void* gameSpace;

    //returned by engine
    Vec3D firstCollisionLocation;
    void* firstCollisionBody;
    std::vector<void*> bodyIntersections_;   //the bodies the ray collided with
  };
}
