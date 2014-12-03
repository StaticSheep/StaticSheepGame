#pragma once
#include "Vertices.h"
namespace DirectSheep
{
  class Camera
  {
  public:

    // Getters and setters for camera position

    // Getters and setters for camera orientation

    // getters and setters for camera fov



  private:

    Mat4 m_View;
    Mat4 m_Projection;
    Mat4 m_viewProj;
    Vec3 m_position;
    float m_width;
    float m_height;
    float m_Theta;
    float m_Fov;
    bool m_perspective;
  };
}
