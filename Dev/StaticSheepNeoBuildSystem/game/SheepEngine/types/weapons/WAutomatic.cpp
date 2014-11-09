#include "pch/precompiled.h"
#include "WAutomatic.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"

namespace Framework
{
  Automatic::Automatic()
  {
    delay = 5;
    damage = 10;
  }

  Automatic::~Automatic()
  {

  }
}