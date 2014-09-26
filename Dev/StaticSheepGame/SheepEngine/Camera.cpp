#include "SheepGraphics.h"
#include "Camera.h"

namespace Framework
{
  Camera::Camera()
  {
    transform = NULL;
    GRAPHICS->CurrentCamera = this;
  }

  Camera::~Camera()
  {
    if(GRAPHICS->CurrentCamera == this)
      GRAPHICS->CurrentCamera = NULL;

    if(transform)
      delete transform;

    transform = NULL;
  }

  void Camera::Initialize()
  {
    transform = new Transform;
    FOV = 75;
    transform->Translation = Vec2(0,0);
    transform->Rotation = 0;
  }

  void Camera::Remove()
  {

  }
}

