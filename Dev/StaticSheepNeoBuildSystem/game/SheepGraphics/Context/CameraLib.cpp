#include "precompiled.h"
#include "Context.h"
namespace DirectSheep
{
  GFX_API Handle RenderContext::NewCamera()
  {
    return Handle(CAMERA, new Camera(1920, 1080, true));
  }

  GFX_API void RenderContext::SetUseCam(bool camUse)
  {
    m_camUse = camUse;
    if (m_camUse)
      m_camera.ptr = m_Perspective.ptr;
    else
      m_camera.ptr = m_Ortho.ptr;
  }

  GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y);
  }

  GFX_API void RenderContext::SetCamPosition(Handle Camera, float x, float y, float z)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetPosition(x, y, z);
  }

  GFX_API void RenderContext::SetCamScale(Handle Camera, float width, float height)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetScale(width, height);
  }

  GFX_API void RenderContext::SetCamFOV(Handle Camera, float FOV)
  {
    if (Camera.type == CAMERA)
      ((DirectSheep::Camera*)Camera.ptr)->SetFov(FOV);
  }

  GFX_API void RenderContext::SetCamActive(Handle Camera)
  {
    if (Camera.type == CAMERA && Camera.ptr)
      m_Perspective.ptr = Camera.ptr;
    else
      SetCamDefault();
  }

  GFX_API Handle RenderContext::GetCamActive()
  {
    return m_Perspective;
  }

  GFX_API void RenderContext::SetCamDefault()
  {
    m_Perspective.ptr = m_editor.ptr;
  }
}