#include "precompiled.h"
#include "Camera.h"
namespace DirectSheep
{
  Camera::Camera(float screenWidth, float screenHeight, bool isPerspective) : m_perspective(isPerspective),
    m_width(screenWidth),
    m_height(screenHeight),
    m_Fov(75),
    m_position(Vec3(0, 0, -400.0f)), 
    m_Theta(0)
  {
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

  Vec3 Camera::GetPosition()
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

  float Camera::GetOrientation()
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

  float Camera::GetFov()
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

  Mat4 Camera::getView()
  {
    return m_View;
  }

  Mat4 Camera::getProj()
  {
    return m_Projection;
  }

  Mat4 Camera::getViewProj()
  {
    return m_viewProj;
  }

  void Camera::BuildView()
  {
    Vec4 eyepoint(m_position.x, m_position.y, m_position.z, 0);

    Vec4 lootAtPoint(m_position.x, m_position.y, 0.0f, 1.0f);

    Vec4 upVector(0.0f, 1.0f, 0.0f, 0);

    m_View = DirectX::XMMatrixLookAtLH(eyepoint, lootAtPoint, upVector);
  }

  void Camera::BuildPerspective()
  {
    m_Projection = DirectX::XMMatrixPerspectiveFovLH((FLOAT)DirectX::XMConvertToRadians(m_Fov), m_width / m_height, 1.0f, 1000.0f);
  }

  void Camera::BuildOrthographic()
  {
    m_Projection = DirectX::XMMatrixOrthographicOffCenterLH(0, m_width, m_height, 0, 1.0f, 1000.0f);
  }
}