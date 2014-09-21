// ------------------------------------------------------------------------------------
//  File     : GFXTypes.h
//  Author   : Kai-Hwa Yao
//  Copyright: All content © 2013-2014 DigiPen (USA) Corporation, all rights reserved.
// ------------------------------------------------------------------------------------

#pragma once

#include <dxgiformat.h>
#include "Color.h"
#include "Viewport.h"
#include "Camera.h"
#include "TextureCoordinate.h"
#include "Tools\SortedVector.h"
#include "..\Math\Matrix4.h"
#include "..\Math\Vector3.h"

namespace Graphics
{

enum ObjectType
{
  OT_NONE = -1,
  OT_VERTEX_SHADER,
  OT_PIXEL_SHADER,
  OT_TEXTURE,
  OT_VERTEX_BUFFER,
  OT_INDEX_BUFFER,
  OT_CONSTANT_BUFFER,
  OT_RENDER_TARGET,
  OT_BACK_BUFFER
};

enum PrimitiveTopology
{
  PRIMITIVE_TOPOLOGY_POINTLIST,
  PRIMITIVE_TOPOLOGY_LINELIST,
  PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

enum Format
{
  FORMAT_UCHAR4      = DXGI_FORMAT_R8G8B8A8_UNORM,
  FORMAT_UCHAR4_BGRA = DXGI_FORMAT_B8G8R8A8_UNORM,
  FORMAT_UNSIGNED1   = DXGI_FORMAT_R32_UINT,
  FORMAT_UNSIGNED2   = DXGI_FORMAT_R32G32_UINT,
  FORMAT_UNSIGNED3   = DXGI_FORMAT_R32G32B32_UINT,
  FORMAT_UNSIGNED4   = DXGI_FORMAT_R32G32B32A32_UINT,
  FORMAT_INT1        = DXGI_FORMAT_R32_SINT,
  FORMAT_INT2        = DXGI_FORMAT_R32G32_SINT,
  FORMAT_INT3        = DXGI_FORMAT_R32G32B32_SINT,
  FORMAT_INT4        = DXGI_FORMAT_R32G32B32A32_SINT,
  FORMAT_FLOAT1      = DXGI_FORMAT_R32_FLOAT,
  FORMAT_FLOAT2      = DXGI_FORMAT_R32G32_FLOAT,
  FORMAT_FLOAT3      = DXGI_FORMAT_R32G32B32_FLOAT,
  FORMAT_FLOAT4      = DXGI_FORMAT_R32G32B32A32_FLOAT,
};

enum BlendMode
{
  BLEND_MODE_NONE = -1,
  BLEND_MODE_ALPHA,
  BLEND_MODE_ADDITIVE,
};

enum RenderTargetMode
{
  RTM_NORMAL,
  RTM_DOWNSAMPLED,
  RTM_FIXED
};

struct Vertex
{
  float x, y;   // Position
  float u, v;   // Texture Coord
};

struct Dimension
{
  Dimension(unsigned w = 0, unsigned h = 0) : width(w), height(h) {};

  unsigned width;
  unsigned height;
};

struct InputElement
{
  InputElement(const std::string& sem, Format form, bool inst = false)
    : semantic(sem), format(form), instance(inst) {}

  std::string semantic;
  Format format;
  bool instance;
};

typedef std::vector<InputElement> InputLayout;

struct DisplayMode
{
  DisplayMode(unsigned width, unsigned height, unsigned Hz)
    : resolution(Dimension(width, height)), refreshRate(Hz) {}

  Dimension resolution;
  unsigned refreshRate;
};

} //namespace Graphics
