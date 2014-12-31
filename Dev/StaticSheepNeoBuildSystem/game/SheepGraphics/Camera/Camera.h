/*****************************************************************
Filename: Camera.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "Vertices.h"
namespace DirectSheep
{
  class Camera
  {
  public:
    Camera(float screenWidth, float screenHeight, bool isPerspective);
    ~Camera();

    // Getters and setters for camera position
    void SetPosition(float x, float y);
    void SetPosition(Vec2 position);
    void SetPosition(float x, float y, float z);
    void SetPosition(Vec3 position);
    Vec3 GetPosition() const;

    // Getters and setters for camera orientation
    void SetOrientation(float orientation);
    float GetOrientation() const;

    // getters and setters for camera fov
    void SetFov(float fov);
    float GetFov() const;

    void SetScale(float width, float height);
    void SetScale(Vec2 scale);
    Vec2 GetScale();

    Mat4 GetView() const;
    Mat4 GetProj() const;
    Mat4 GetViewProj() const;

    Vec3 ToWorld(Vec2 screenPos) const;
    Vec2 ToScreen(Vec3 worldPpos) const;

  private:
    void BuildView();
    void BuildPerspective();
    void BuildOrthographic();

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