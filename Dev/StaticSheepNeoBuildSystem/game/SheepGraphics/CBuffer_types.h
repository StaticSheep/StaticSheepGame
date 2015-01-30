/*****************************************************************
Filename: CBuffer_types.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "Vertices.h"
#include "DataTypes.h"
#define MAX_LIGHTS 100
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
    Vec4 atten[MAX_LIGHTS];
    Color col[MAX_LIGHTS];
    Vec4 pos[MAX_LIGHTS];
    int numLights;
    float padding[(MAX_LIGHTS * sizeof(Vec4) + sizeof(float)) % 16];
  };
}