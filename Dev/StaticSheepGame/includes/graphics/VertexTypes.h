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

  extern ID3D11InputLayout *LAYOUT;

  struct VertexBufferQuad
  {
    ID3D11Buffer *vBuffer;
    ID3D11Buffer *cBuffer;
  };

  extern VertexBufferQuad *QUAD;
}