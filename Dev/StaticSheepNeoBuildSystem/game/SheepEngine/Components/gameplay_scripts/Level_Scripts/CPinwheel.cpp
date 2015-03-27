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
    int moveFactor;

    if (moveLeft)
      moveFactor = -1;
    else
      moveFactor = 1;

    BoxCollider* body = space->GetHandles().GetAs<BoxCollider>(gCollider);
    Transform* trans = space->GetHandles().GetAs<Transform>(gTransfrom);

    trans->SetTranslation(trans->GetTranslation() + Vec3(2.0 * moveFactor, 0, 0));

    //trans->SetRotation(trans->GetRotation() + (dt * 1.0f * -moveFactor));

    body->SetAngVelocity(-moveFactor);

    //destroy when out of bounds
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
  }

  void Pinwheel::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }
}