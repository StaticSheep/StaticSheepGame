#pragma once

#include "SheepPhysics\Handle.h"
#include "SheepPhysics\Shape.h"
#include "systems/physics/SheepPhysics.h"

namespace SheepFizz
{
  class Material;
}

namespace Framework
{

  class Transform;

  class Ray : public GameComponent
  {

    //raycasting
    virtual void SetRayCast(Vec3D& rayOrigin, Vec3D& rayDirection, std::string name);
    virtual bool SimpleRayCast();
    virtual bool ComplexRayCast();
    virtual void RayDestruction(float damage);

  };

}