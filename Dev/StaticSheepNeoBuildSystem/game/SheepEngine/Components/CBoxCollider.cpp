#include "precompiled.h"
#include "CBoxCollider.h"

namespace Framework
{
  BoxCollider::BoxCollider()
    :RigidBody(SheepFizz::Cir)
  {

  }

  BoxCollider::~BoxCollider()
  {

  }

  void BoxCollider::Initialize()
  {
    RigidBody::Initialize();
  }

  void BoxCollider::Remove()
  {
    RigidBody::Remove();
  }


}