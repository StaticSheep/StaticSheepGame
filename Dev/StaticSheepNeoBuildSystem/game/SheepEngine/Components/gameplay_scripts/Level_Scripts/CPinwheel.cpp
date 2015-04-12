/*****************************************************************
Filename: CPinwheel.cpp
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "CPinwheel.h"
#include "../../colliders/CBoxCollider.h"

namespace Framework
{
  Pinwheel::Pinwheel() : moveLeft(false)
  {
    
  }

  Pinwheel::~Pinwheel()
  {

  }

  void Pinwheel::LogicUpdate(float dt)
  {
    BoxCollider* body = space->GetHandles().GetAs<BoxCollider>(gCollider);
    Transform* trans = space->GetHandles().GetAs<Transform>(gTransfrom);
    
    if (moveLeft)
      moveFactor = -1;
    else
      moveFactor = 1;

    body->SetVelocity(velocity * (float)moveFactor);
    body->SetAngVelocity(-(float)moveFactor);

    if (trans->GetTranslation().x > 1008 || trans->GetTranslation().x < -1008)
    {
      space->GetGameObject(owner)->Destroy();
    }
  }

  void Pinwheel::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Pinwheel::LogicUpdate));

    gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);

    velocity = Vec3D(120.0f, 0, 0);
    BoxCollider* body = space->GetHandles().GetAs<BoxCollider>(gCollider);
    body->SetBodyFrictionMod(0.0f);
    body->m_snap = true;
  }

  void Pinwheel::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}