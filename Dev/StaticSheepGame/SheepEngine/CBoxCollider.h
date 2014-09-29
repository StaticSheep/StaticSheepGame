#pragma once

#include "CRigidBody.h"

namespace Framework
{
  class BoxCollider : public RigidBody
  {
  public:
    // Default constructor
    BoxCollider();

    // Deconstructor
    ~BoxCollider();

    // Initialization routine
    void Initialize();

    // Removal function
    void Remove();

    void SetVelocity()
    {
      RigidBody::SetVelocity();
    }


  };
}