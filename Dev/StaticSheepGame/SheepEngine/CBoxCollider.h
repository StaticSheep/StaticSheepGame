#pragma once

#include "CRigidBody.h"

namespace Framework
{
  class BoxCollider : public RigidBody
  {
    // Default constructor
    BoxCollider();

    // Deconstructor
    ~BoxCollider();

    // Initialization routine
    void Initialize();

    // Removal function
    void Remove();

  };
}