/******************************************************************************
Filename: RayConfig.h
Project:  GAM 250
Author(s): Jonathan Sourbeer (primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
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
    RayConfig();

    //set by client
    Vec3D rayOrigin;
    Vec3D rayDirection;
    bool findFirstCollision;
    CollisionGroup collisionGroup;
  
    //pointer to the current gamespace, for collision resolution
    void* gameSpace;

    //returned by engine
    Vec3D firstCollisionLocation;
    void* firstCollisionObject;

    std::vector<Vec3D> bodyIntersections_;   //the bodies the ray collided with - currently set to position for debug info
    //std::vector<void*> bodiesIntersected_;
  };
}
