/*****************************************************************
Filename: CCamera.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"

#include "systems/graphics/SheepGraphics.h"
#include "Context/Context.h"
#include "CCamera.h"

#include "Camera/Camera.h"

namespace Framework
{
  Camera::Camera() : m_viewPort(1920, 1080), m_active(false)
  {
    // Calculates camera Fov based off it's default size4
    // This gives pixel to viewport match for near plane (z = 0)
    m_FOV = 2.0f * (atan(m_viewPort.y / (2.0f * 100)));
    transform = NULL;
  }

  Camera::~Camera()
  {
  }

  void Camera::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    trans->m_useDirtyHook = true;

    this->GetOwner()->hooks.Add("TransformDirty", self, BUILD_FUNCTION(Camera::UpdatePosition));

    m_camHandle = GRAPHICS->RC()->NewCamera();

    if(m_active || !space->m_edit)
      SetActive(true);

    UpdatePosition();

  }

  void Camera::Remove()
  {
    this->GetOwner()->hooks.Remove("TransformDirty", self);
  }

  void Camera::UpdatePosition()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    DirectSheep::Camera* cam = GRAPHICS->RetrieveCamera(m_camHandle);
    cam->SetPosition(trans->GetTranslation().x, trans->GetTranslation().y);
    cam->SetFov(m_FOV);
    cam->SetScale(m_viewPort.x_, m_viewPort.y_);
  }

  void Camera::SetActive(bool isActive)
  {
    m_active = isActive;

    Camera* currCam = space->GetHandles().GetAs<Camera>(GRAPHICS->currentCamera);

    if (currCam && !IsActive())
      currCam->m_active = false;

    GRAPHICS->currentCamera = this->GetOwner()->GetComponentHandle(Framework::eCamera);

    if (space->m_edit)
      return;

    if (m_active)
      GRAPHICS->SetActiveCamera(m_camHandle);
  }

  void Camera::TweakSetActive(void * isActive)
  {
    SetActive(*(bool*)isActive);
  }

  bool Camera::IsActive()
  {
    if (this->GetOwner()->GetComponentHandle(Framework::eCamera) == GRAPHICS->currentCamera)
      return true;
    else
      return false;
  }

  // Sets field of view for camera
  void Camera::SetFov(float FOV)
  {
    m_FOV = FOV;
    GRAPHICS->RetrieveCamera(m_camHandle)->SetFov(m_FOV); 
  }

  void Camera::TweakSetFov(void* FOV)
  {
    SetFov(*(float*)FOV);
  }

  float Camera::GetFov()
  {
    return m_FOV;
  }

  void Camera::SetViewport(Vec2 viewport)
  {
    m_viewPort = viewport;

    GRAPHICS->RetrieveCamera(m_camHandle)->SetScale(m_viewPort.x_, m_viewPort.y_);
  }

  Vec2 Camera::GetViewport()
  {
    return m_viewPort;
  }
}

