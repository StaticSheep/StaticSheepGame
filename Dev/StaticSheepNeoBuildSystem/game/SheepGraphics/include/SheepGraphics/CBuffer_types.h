#pragma once
#include "Vertices.h"
#include "DataTypes.h"

namespace DirectSheep
{
  struct MatBuffer
  {
    Mat4 m_proj;
    Mat4 m_view;
    Mat4 m_world;
  };

  struct UV_ColBuffer
  {
    Vec4 m_Color;
    Vec2 m_UVBegin;
    Vec2 m_UVEnd;
  };

  struct LightBuffer
  {
    Vec4 atten;
    Color col;
    Vec4 pos;
  };

  struct LightsBuffer
  {
    Vec4 atten[10];
    Color col[10];
    Vec4 pos[10];
    int numLights;
    float padding[3];
  };
}
