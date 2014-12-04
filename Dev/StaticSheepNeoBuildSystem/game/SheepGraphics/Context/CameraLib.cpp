/*****************************************************************
Filename: CameraLib.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Context.h"
namespace DirectSheep
{
  /*!
      \brief
      Allocate a new perspective camera
  */
  GFX_API Handle RenderContext::NewCamera()
  {
    Handle newCam(CAMERA, new Camera(1920, 1080, true));
    m_CameraPool.push_back((Camera*)newCam.ptr);
    return newCam;
  }

  /*!
      \brief
      Toggle which primary camera is in use
  */
  GFX_API void RenderContext::SetCamState(int camState)
  {
    switch (camState)
    {
    case 0:
      m_camera.ptr = m_Perspective.ptr;
      m_camUse = true;
      break;
    case 1:
      m_camera.ptr = m_Ortho.ptr;
      m_camUse = false;
      break;
    case 2:
      m_camera.ptr = m_orthoScreen.ptr;
      m_camUse = false;
      break;
    default:
      m_camera.ptr = m_Ortho.ptr;
      m_camUse = false;
      break;
    }
  }

  /*!
      \brief
      Moves camera referenced by handle
  */
  GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y);
  }

  /*!
      \brief
      Moves camera referenced by handle
  */
  GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y, float z)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y, z);
  }

  /*!
      \brief
      Scales camera referenced by handle
  */
  GFX_API void RenderContext::SetCamScale(Handle Camera, float width, float height)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetScale(width, height);
  }

  /*!
      \brief
      Sets camera FOV referenced by handle
  */
  GFX_API void RenderContext::SetCamFOV(Handle Camera, float FOV)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetFov(FOV);
  }

  /*!
      \brief
      Set camera as active
  */
  GFX_API void RenderContext::SetCamActive(Handle Camera)
  {
    if (Camera.type == CAMERA && Camera.ptr)
      m_Perspective.ptr = Camera.ptr;
    else
      SetCamDefault();
  }

  /*!
      \brief
      Return handle to active camera
  */
  GFX_API Handle RenderContext::GetCamActive()
  {
    return m_Perspective;
  }

  /*!
      \brief
      Set active camera as editor camera
  */
  GFX_API void RenderContext::SetCamDefault()
  {
    m_Perspective.ptr = m_editor.ptr;
  }
}