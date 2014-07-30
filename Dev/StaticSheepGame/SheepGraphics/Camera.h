#pragma once

namespace DirectSheep
{

  struct Transform
  {
    Vec3 position;
    float Rotation;
  };

  struct Camera
  {
    Transform * transform;
    Mat4 ProjMatrix;
    Mat4 ViewMatrix;
    Mat4 ViewProjMatrix;
    Vec2 ScreenDimensions;
  };

}
extern DirectSheep::Camera *CAMERA;
