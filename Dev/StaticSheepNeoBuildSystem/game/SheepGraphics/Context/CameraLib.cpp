/*****************************************************************
Filename: CameraLib.cpp
Project:
Author(s): Scott Nelson (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Context.h"
namespace DirectSheep
{
  GFX_API Handle RenderContext::NewCamera()
  {
    Handle newCam(CAMERA, new Camera(1920, 1080, true));
    m_CameraPool.push_back((Camera*)newCam.ptr);
    return newCam;
  }

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

  //GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y)
  //{
  //  if (Camera.type == CAMERA)
  //    ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y);
  //}

  //GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y, float z)
  //{
  //  if (Camera.type == CAMERA)
  //    ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y, z);
  //}

  //GFX_API void RenderContext::SetCamScale(Handle Camera, float width, float height)
  //{
  //  if (Camera.type == CAMERA)
  //    ((DirectSheep::Camera*)Camera.ptr)->SetScale(width, height);
  //}

  //GFX_API void RenderContext::SetCamFOV(Handle Camera, float FOV)
  //{
  //  if (Camera.type == CAMERA)
  //    ((DirectSheep::Camera*)Camera.ptr)->SetFov(FOV);
  //}

  GFX_API void RenderContext::SetActiveCamera(Handle Camera)
  {
    if (Camera.type == CAMERA && Camera.ptr)
      m_Perspective.ptr = Camera.ptr;
    else
      ActivateDefaultCamera();
  }

  GFX_API Handle RenderContext::GetActiveCamera()
  {
    return m_camera;
  }

  GFX_API void RenderContext::ActivateDefaultCamera()
  {
    m_Perspective.ptr = m_editor.ptr;
  }

  GFX_API Framework::Mat4D RenderContext::GetCamViewProj(Handle camera)
  {
    if (camera.type == CAMERA)
    {
      float* m = (float*)&((DirectSheep::Camera*)camera.ptr)->GetViewProj();
      return Framework::Mat4D(m);
    }

    return Framework::Mat4D(-1.0f, -1.0f, -1.0f);
      
  }

  
}