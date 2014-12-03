#pragma once

#include "Handle.h"
namespace Framework
{
  class Camera : public GameComponent
  {
  public:
    Camera();
    ~Camera();
    virtual void Initialize();
    virtual void Remove();

    // Sets this camera to used
    void SetActive(bool isActive);

    void TweakSetActive(void * isActive);

    // Returns if the camera is active or not
    bool IsActive();

    // Sets field of view for camera
    void SetFov(float FOV);
    void TweakSetFov(void * FOV);
    float GetFov();

    void SetViewport(Vec2 viewport);
    Vec2 GetViewport();
    bool m_active;
    float m_FOV;
  private:

    void UpdatePosition();
    Handle transform;
    DirectSheep::Handle m_CamHandle;
    Vec2 m_viewPort;
  };

}