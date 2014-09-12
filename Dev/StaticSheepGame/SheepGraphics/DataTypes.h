#pragma once

#include <dxgiformat.h>
/*#include "Color.h"
#include "Viewport.h"
#include "Camera.h"
#include "TextureCoordinate.h"
#include "Tools\SortedVector.h"
#include "..\Math\Matrix4.h"
#include "..\Math\Vector3.h"*/

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

  struct Color
  {
    Color() : r(1), g(1), b(1), a(1) {}
    Color(float r, float g, float b, float a=1) : r(r), g(g), b(b), a(a) {}

    float r;
    float g;
    float b;
    float a;
  };
  /*
  struct DXShader
  {
    ID3D11VertexShader* VS;
    ID3D11PixelShader* PS;
    ID3D11InputLayout* Input;
  };

  typedef std::unordered_map<std::string, DXShader> ShaderMap;

  struct DirectX_Core
  {
    IDXGISwapChain *swapchain;             // Pointer to swap chain interface
    ID3D11Device *dev;                     // Pointer to D3D device
    ID3D11DeviceContext *devcon;           // Pointer to D3D device context
    ID3D11RenderTargetView *backbuffer;    // Pointer to back buffer
    ID3D11DepthStencilView *depthbuffer;       // Pointer to depth buffer

    ID3D11VertexShader *pVertexShader;               // Pointer to current Vertex Shader
    ID3D11PixelShader *pPixelShader;                // Pointer to current Pixel Shader
    ID3D11InputLayout *pInputLayout;            // Pointer to current Input Layout

    ID3D11Buffer *pCBuffer;                // Pointer to constant buffer

    IFW1Factory *pFW1Factory;
	
    IFW1FontWrapper *pFontWrapper;
  };

  struct States
  {
    ID3D11RasterizerState *RState;    // Standard rasterizer state
    //ID3D11RasterizerState *pRSWireFrame;  // Wireframe (primitive debug draw)
    ID3D11SamplerState *SamplerState;              // Sampler State
    ID3D11BlendState *BlendState;                // Blend State (alpha, blending)
    ID3D11DepthStencilState *DepthState;
  };

  struct ShapeStates
  {
    Vec2 position;
    Vec2 scale;
    float rotation;
    D3DXCOLOR Color;
    int TexID;
    bool useCamera;
  };

  struct TextureMap
  {
    std::vector<ID3D11ShaderResourceView*> TextureVec;
    std::map<std::string, int> TextureIndex;
  };

  struct VertexBufferQuad
  {
    ID3D11Buffer *VertexBuffer;
  };

	template< typename RefType >
	void Release( RefType& ptr )
	{
		if( ptr )         // If pointer exists
      ptr->Release(); // Release

		ptr = NULL;       // Set pointer to NULL
	}*/
}

/*extern DirectSheep::DirectX_Core* CORE;
extern DirectSheep::States *STATES;
extern DirectSheep::TextureMap *TEXTUREMAP;
extern DirectSheep::ShapeStates SHAPESTATES;
extern DirectSheep::VertexBufferQuad *QUAD;
extern DirectSheep::ShaderMap SHADERMAP;*/