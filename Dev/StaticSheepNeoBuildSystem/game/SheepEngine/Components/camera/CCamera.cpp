


#include "pch/precompiled.h"

#include "systems/graphics/SheepGraphics.h"
#include "CCamera.h"

namespace Framework
{
  Camera::Camera()
  {
    transform = NULL;
    GRAPHICS->CurrentCamera = this->GetOwner()->GetComponentHandle(eCamera);
  }

  Camera::~Camera()
  {
  }

  void Camera::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);
    FOV = 75;
  }

  void Camera::Remove()
  {

  }
}

