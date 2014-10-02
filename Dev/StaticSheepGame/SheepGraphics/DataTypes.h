#pragma once

#include <dxgiformat.h>
#include "api.h"

namespace DirectSheep
{
  enum ObjectType
  {
    NONE = -1,
    VERTEX_SHADER,
    PIXEL_SHADER,
    TEXTURE,
    VERTEX_BUFFER,
    INDEX_BUFFER,
    CONSTANT_BUFFER,
    RENDER_TARGET,
    BACK_BUFFER,
    oTypecount,
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
   GFX_API Dimension(unsigned w = 0, unsigned h = 0) : width(w), height(h) {};
    unsigned width;
    unsigned height;
  };

  struct InputElement
  {
  GFX_API InputElement(const std::string& sem, Format form, bool inst = false) : semantic(sem), format(form), instance(inst) {}

    std::string semantic;
    Format format;
    bool instance;
  };

  typedef std::vector<InputElement> InputLayout;

  struct DisplayMode
  {
   GFX_API DisplayMode(unsigned width, unsigned height, unsigned Hz)
      : resolution(Dimension(width, height)), refreshRate(Hz) {}

    Dimension resolution;
    unsigned refreshRate;
  };

  struct Color
  {
   GFX_API Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
   GFX_API Color(float r, float g, float b, float a=1) : r(r), g(g), b(b), a(a) {}
   Color(const DirectX::XMVECTORF32& color) : r(color.f[0]), g(color.f[1]), b(color.f[2]), a(color.f[3])  {}

    float r;
    float g;
    float b;
    float a;
  };
}