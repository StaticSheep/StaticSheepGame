/*****************************************************************
Filename: Camera.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Camera.h"
#include "Context\Context.h"

namespace DirectSheep
{
  Camera::Camera(float screenWidth, float screenHeight, bool isPerspective) :
    m_perspective(isPerspective),
    m_width(screenWidth),
    m_height(screenHeight),
    m_position(Vec3(0, 0, -100.0f)),
    m_Theta(0)
  {
    // Calculation to determine Pixel Perfect Near Plane
    m_Fov = 2.0f * (atan(screenHeight / (2.0f * abs(m_position.z))));
    BuildView();
    if (m_perspective)
      BuildPerspective();
    else
      BuildOrthographic();
    m_viewProj = m_View * m_Projection;
  }

  Camera::~Camera()
  {
  }

  // Getters and setters for camera position
  void Camera::SetPosition(float x, float y, float z)
  {
    m_position = Vec3(x, y, z);
    BuildView();
    m_viewProj = m_View * m_Projection;
  }

  void Camera::SetPosition(Vec3 position)
  {
    m_position = position;
    BuildView();
    m_viewProj = m_View * m_Projection;
  }

  Vec3 Camera::GetPosition() const
  {
    return m_position;
  }

  void Camera::SetPosition(float x, float y)
  {
    m_position.x = x;
    m_position.y = y;
    BuildView();
    m_viewProj = m_View * m_Projection;
  }

  void Camera::SetPosition(Vec2 position)
  {
    m_position.x = position.x;
    m_position.y = position.y;
    BuildView();
    m_viewProj = m_View * m_Projection;
  }

  // Getters and setters for camera orientation
  void Camera::SetOrientation(float orientation)
  {
    m_Theta = orientation;
  }

  float Camera::GetOrientation() const
  {
    return m_Theta;
  }

  // getters and setters for camera fov
  void Camera::SetFov(float fov)
  {
    m_Fov = fov;
    BuildPerspective();
    m_viewProj = m_View * m_Projection;
  }

  float Camera::GetFov() const
  {
    return m_Fov;
  }

  void Camera::SetScale(float width, float height)
  {
    m_width = width;
    m_height = height;

    if (m_perspective)
      BuildPerspective();
    else
      BuildOrthographic();
    m_viewProj = m_View * m_Projection;
  }

  void Camera::SetScale(Vec2 scale)
  {
    m_width = scale.x;
    m_height = scale.y;

    if (m_perspective)
      BuildPerspective();
    else
      BuildOrthographic();
    m_viewProj = m_View * m_Projection;
  }

  Vec2 Camera::GetScale()
  {
    return Vec2(m_width, m_height);
  }

  Mat4 Camera::GetView() const
  {
    return m_View;
  }

  Mat4 Camera::GetProj() const
  {
    return m_Projection;
  }

  Mat4 Camera::GetViewProj() const
  {
    return m_viewProj;
  }

  void Camera::BuildView()
  {
    Vec4 eyepoint(m_position.x, m_position.y, m_position.z, 0.0f);

    Vec4 lootAtPoint(m_position.x, m_position.y, 0.0f, 0.0f);

    Vec4 upVector(0.0f, 1.0f, 0.0f, 0.0f);

    m_View = DirectX::XMMatrixLookAtLH(eyepoint, lootAtPoint, upVector);
  }

  void Camera::BuildPerspective()
  {
    m_Projection = DirectX::XMMatrixPerspectiveFovLH(m_Fov, m_width / m_height, abs(m_position.z) - 10, abs(m_position.z) + 1000.0f);
  }

  void Camera::BuildOrthographic()
  {
    m_Projection = DirectX::XMMatrixOrthographicOffCenterLH(0, m_width, -m_height, 0, 0.0f, 1000.0f);
  }

  Vec3 Camera::ToWorld(Vec2 screenPos) const
  {
    if (m_perspective)
    {

      Vec4 world = Vec4::Transform(Vec4(2 * (screenPos.x/SCREEN_WIDTH) - 1,
        -(2 * (screenPos.y / SCREEN_HEIGHT) - 1),
        0.11f, 1), m_viewProj.Invert());
      world.w = 1.0f / world.w;

      world.x *= world.w;
      world.y *= world.w;
      world.z *= world.w;

      //world.x = 1600 * (world.x + 1) / 2;
      //world.y = 900 * (-world.y + 1) / 2;

      return world;
    }
    else
    {
      return Vec3(screenPos.x, screenPos.y, 0);
    }
    
  }

  Vec2 Camera::ToScreen(Vec3 worldPos) const
  {
    if (m_perspective)
    {
      Vec4 screen = Vec4::Transform(Vec4(worldPos.x, worldPos.y, 0.0f, 1), m_viewProj);
      screen.x = SCREEN_WIDTH * ((screen.x / screen.w) + 1) / 2;
      screen.y = SCREEN_HEIGHT * ((-screen.y / screen.w) + 1) / 2;

      return Vec2((float*)&screen);
    }
    else
    {
      return Vec2(worldPos);
    }
    
  }

}