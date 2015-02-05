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

    //returned by engine
    Vec3D firstCollisionLocation;
    Handle firstCollisionBody;
    std::vector<Handle> bodyIntersections_;   //the bodies the ray collided with
  };
}
