/*****************************************************************
Filename: CSaucerMissile.h
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../../transform/CTransform.h"

namespace Framework
{
  class SaucerMissile : public GameComponent
  {
  public:
    SaucerMissile();
    ~SaucerMissile();
    void LogicUpdate(float dt);
    void Initialize();
    void Remove();

    //member variables
    Handle mTransform;
    Handle mCollider;
  };
}