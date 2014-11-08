#include "precompiled.h"
#include "Camera.h"
namespace DirectSheep
{
  Camera::Camera(float screenWidth, float screenHeight, bool isPerspective) : m_perspective(isPerspective),
    m_width(screenWidth),
    m_height(screenHeight),
    m_position(0, 0, -400.0f), m_Theta(0),
    m_Projection(DirectX::SimpleMath::Matrix::Identity),
    m_View(m_Projection),
    m_viewProj(m_Projection)
  {

  }

  Camera::~Camera()
  {
  }

  // Getters and setters for camera position
  void Camera::SetPosition(float x, float y, float z)
  {
    m_position = Vec3(x, y, z);
    BuildView();
  }

  void Camera::SetPosition(Vec3 position)
  {
    m_position = position;
    BuildView();
  }

  Vec3 Camera::GetPosition()
  {
    return m_position;
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
    m_Fov;
    BuildPerspective();
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
  }

  void Camera::SetScale(Vec2 scale)
  {
    m_width = scale.x;
    m_height = scale.y;

    if (m_perspective)
      BuildPerspective();
    else
      BuildOrthographic();
  }

  Vec2 Camera::GetScale()
  {
    return Vec2(m_width, m_height);
  }

  void Camera::BuildView()
  {
    float cameraX = m_position.x;
    float cameraY = m_position.y;
    float cameraZ = m_position.z;

    Vec4 eyepoint(cameraX, cameraY, cameraZ, 0);

    Vec4 lootAtPoint(cameraX, cameraY, 0.0f, 1.0f);

    Vec4 upVector(0.0f, 1.0f, 0.0f, 0);

    m_View = DirectX::XMMatrixLookAtLH(eyepoint, lootAtPoint, upVector);
  }

  void Camera::BuildPerspective()
  {
    m_Projection = DirectX::XMMatrixPerspectiveFovLH((FLOAT)DirectX::XMConvertToRadians(m_Fov), m_width / m_height, 1.0f, 1000.0f);
  }

  void Camera::BuildOrthographic()
  {
    m_Projection = DirectX::XMMatrixOrthographicLH((float)m_width, (float)m_height, 1.0f, 1000.0f);
  }
}