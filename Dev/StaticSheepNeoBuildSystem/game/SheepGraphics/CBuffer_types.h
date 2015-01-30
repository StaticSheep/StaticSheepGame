/*****************************************************************
Filename: CBuffer_types.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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

  struct PosUV
  {
    Mat4 m_proj;
    Mat4 m_view;
    Mat4 m_world;
    Vec2 uvBegin;
    Vec2 uvEnd;
  };

  struct Ambient
  {
    Vec4 m_ambientCol;
    Vec4 m_intensity;
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

  struct LightFBuffer
  {
    float d50;
    float d0;
    Color col;
    Vec4 pos;
  };
}