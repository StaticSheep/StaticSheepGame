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
    MCData(float length, Handle obj) : mc_length(length), mc_obj(obj){};
    float mc_length;
    Handle mc_obj;
  };

  class MultiRay
  {
    MultiRay(Vec3D& origin, Vec3D& direction, void* space, int width, 
      int collisionGroup, std::vector<MCData>& results, int resolution = LASERWIDTHMOD);

    ~MultiRay();

    void Initialize();
    void SetWidth(int width);

    //raycasting
    bool ComplexCaster();


    private:
      float m_width;
      int m_resolution;
      RayConfig m_ray;

      std::vector<MCData>* m_results;
      std::vector<Vec3D> m_positionOffsets;

  };

}