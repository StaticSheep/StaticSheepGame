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
    
    if (trans->GetTranslation().x > 1008 || trans->GetTranslation().x < -1008)
    {
      
      moveFactor *= -1;
      velocity *= moveFactor;
      body->SetVelocity(velocity);
      
    }
  }

  void Pinwheel::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Pinwheel::LogicUpdate));

    gTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    gCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);

    if (moveLeft)
      moveFactor = -1;
    else
      moveFactor = 1;

    velocity = Vec3D(50.0f, 0, 0);
    BoxCollider* body = space->GetHandles().GetAs<BoxCollider>(gCollider);
    body->SetVelocity(velocity);
    body->SetAngVelocity(-moveFactor);
    body->SetBodyFrictionMod(1.0f);
  }

  void Pinwheel::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}