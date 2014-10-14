#include "CBoxCollider.h"

namespace Framework
{
  BoxCollider::BoxCollider()
    :RigidBody(SheepFizz::Rec)
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