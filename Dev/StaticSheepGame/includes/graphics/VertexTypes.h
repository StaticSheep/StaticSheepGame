#pragma once

typedef D3DXVECTOR3 Vec3;

namespace Framework
{
  // Our Custom Vertex

  struct Vertex2D
  {
    Vec3 Position; // x, y, z
    float tu, tv;  // u, v
  };
}