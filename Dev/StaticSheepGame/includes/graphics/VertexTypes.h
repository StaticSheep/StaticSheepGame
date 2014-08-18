#pragma once



namespace DirectSheep
{
  // Our Custom Vertex

  typedef D3DXVECTOR2 Vec2;
  typedef D3DXVECTOR3 Vec3;
  typedef D3DXVECTOR4 Vec4;
  typedef D3DXMATRIXA16 Mat4;

  struct CBUFFER
  {
    Mat4 Final;
  };

  struct Vertex2D
  {
    Vec3 Position; // x, y, z
    D3DXCOLOR Color;
    float tu, tv;  // u, v
  };

  struct VertexBufferQuad
  {
    ID3D11Buffer *vBuffer;
  };

}

extern DirectSheep::VertexBufferQuad *QUAD;
