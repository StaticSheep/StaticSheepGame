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

    m_CamHandle = GRAPHICS->m_renderContext->NewCamera();

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

    GRAPHICS->m_renderContext->SetCamPosition(m_CamHandle, trans->GetTranslation().x, trans->GetTranslation().y);
    GRAPHICS->m_renderContext->SetCamFOV(m_CamHandle, m_FOV);
    GRAPHICS->m_renderContext->SetCamScale(m_CamHandle, m_viewPort.x_, m_viewPort.y_);
  }

  void Camera::SetActive(bool isActive)
  {
    m_active = isActive;

    Camera* currCam = space->GetHandles().GetAs<Camera>(GRAPHICS->CurrentCamera);

    if (currCam && !IsActive())
      currCam->m_active = false;

    GRAPHICS->CurrentCamera = this->GetOwner()->GetComponentHandle(Framework::eCamera);

    if (space->m_edit)
      return;

    if (m_active)
      GRAPHICS->m_renderContext->SetCamActive(m_CamHandle);
  }

  void Camera::TweakSetActive(void * isActive)
  {
    SetActive(*(bool*)isActive);
  }

  bool Camera::IsActive()
  {
    if (this->GetOwner()->GetComponentHandle(Framework::eCamera) == GRAPHICS->CurrentCamera)
      return true;
    else
      return false;
  }

  // Sets field of view for camera
  void Camera::SetFov(float FOV)
  {
    m_FOV = FOV;

    GRAPHICS->m_renderContext->SetCamFOV(m_CamHandle, m_FOV);
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

    GRAPHICS->m_renderContext->SetCamScale(m_CamHandle, m_viewPort.x_, m_viewPort.y_);
  }

  Vec2 Camera::GetViewport()
  {
    return m_viewPort;
  }
}

