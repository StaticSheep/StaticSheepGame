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
    float length;
    Handle obj;
  };

  class MultiRay
  {
    MultiRay(Vec3D origin, Vec3D direction, void* space, int width, int collisionGroup, std::vector<MCData>& results, int resolution = LASERWIDTHMOD);

    ~MultiRay();

    void Initialize();
    void SetWidth(int width);

    //raycasting
    bool ComplexRayCast();
    void RayDestruction();


    private:
      int m_width;
      int resolution;
      RayConfig m_ray;

      std::vector<MCData>* results;
      std::vector<Vec3D> positionOffsets;

  };

}