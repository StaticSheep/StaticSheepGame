#pragma once

namespace Framework
{
  class Camera : public GameComponent
  {
  public:
    Camera();
    ~Camera();
    virtual void Initialize();
    virtual void Remove();
    Handle transform;
    float FOV;
  };

}