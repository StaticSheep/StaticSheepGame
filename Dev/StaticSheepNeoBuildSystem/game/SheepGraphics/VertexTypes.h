#pragma once


#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace DirectSheep
{
  // Our Custom Vertex

  typedef DirectX::XMFLOAT2 Vec2;
  typedef DirectX::XMFLOAT3 Vec3;
  typedef DirectX::XMFLOAT4 Vec4;

  typedef DirectX::PackedVector::XMCOLOR iCol4;
  typedef DirectX::XMVECTOR iVec4;

  typedef DirectX::XMFLOAT4X4 Mat4;
  typedef DirectX::XMMATRIX iMat4;

  struct DefaultBuffer
  {
    iMat4 World;
    iMat4 Final;
    Vec4 BlendColor;
    Vec2 uvBegin;
    Vec2 uvEnd;
  };

  template <typename T>
  inline DirectX::XMVECTOR ToVector(T input);

  template <>
  inline DirectX::XMVECTOR ToVector<DirectX::XMFLOAT4*>(DirectX::XMFLOAT4* input)
  {
    return XMLoadFloat4(input);
  }


  struct Vertex2D
  {
    Vec3 Position; // x, y, z

    float tu, tv;  // u, v
  };
}

