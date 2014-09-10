#pragma once



namespace DirectSheep
{
  // Our Custom Vertex

  typedef D3DXVECTOR2 Vec2;
  typedef D3DXVECTOR3 Vec3;
  typedef D3DXVECTOR4 Vec4;
  typedef D3DXMATRIXA16 Mat4;

  struct DefaultBuffer
  {
    Mat4 World;
    Mat4 Final;
    D3DXCOLOR BlendColor;
  };

  struct Vertex2D
  {
    Vec3 Position; // x, y, z

    float tu, tv;  // u, v
  };
      //D3DXCOLOR Color;
}


