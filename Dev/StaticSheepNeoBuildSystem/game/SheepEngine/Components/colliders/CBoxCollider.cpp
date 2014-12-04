/*****************************************************************
Filename: CBoxCollider.cpp
Project:
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
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