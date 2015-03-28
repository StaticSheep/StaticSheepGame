#pragma once

#include "SheepPhysics\Handle.h"
#include "SheepPhysics\Shape.h"
#include "systems/physics/SheepPhysics.h"
#include "components/base/Component.h"
#include "SheepPhysics\RayConfig.h"

namespace SheepFizz
{
  class Material;
}

namespace Framework
{
  #define LASERWIDTHMOD 4

  class Transform;

  struct MCData
  {
    MCData() : length(0), obj(Handle::null) {}
    MCData(float length, unsigned obj) : length(length), obj(obj){};
    float length;
    Handle obj;
  };

  bool MultiRayCaster(Vec3D& origin, Vec3D& direction, GameSpace* space, int width,
    int collisionGroup, std::vector<MCData>& results, int resolution = LASERWIDTHMOD);
}