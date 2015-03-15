#pragma once

#include "SheepPhysics\Handle.h"
#include "SheepPhysics\Shape.h"
#include "systems/physics/SheepPhysics.h"
#include "components/base/Component.h"

namespace SheepFizz
{
  class Material;
}

namespace Framework
{

  class Transform;

  class MultiRay
  {

    //raycasting
    virtual void SetRayCast(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name);
    virtual bool SimpleRayCast();
    virtual bool ComplexRayCast();
    virtual void RayDestruction(float damage);

  };

}