#pragma once


#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace DirectSheep
{
  // Our Custom Vertex

  typedef DirectX::SimpleMath::Vector2 Vec2;
  typedef DirectX::SimpleMath::Vector3 Vec3;
  typedef DirectX::SimpleMath::Vector4 Vec4;
  typedef DirectX::SimpleMath::Color Color;

  typedef DirectX::PackedVector::XMCOLOR iCol4;
  typedef DirectX::XMVECTOR iVec4;

  typedef DirectX::SimpleMath::Matrix Mat4;
  typedef DirectX::XMMATRIX iMat4;

  struct DefaultBuffer
  {
    iMat4 World;
    iMat4 Final;
    Vec4 BlendColor;
    Vec2 uvBegin;
    Vec2 uvEnd;
  };

  struct Bufferpf
  {
    Vec4 ambientColor;
    float ambientIntensity;
  };

  struct PSframe
  {

  };

  struct PSObject;




  struct Vertex2D
  {
    Vec3 Position; // x, y, z

    float tu, tv;  // u, v
  };
}

