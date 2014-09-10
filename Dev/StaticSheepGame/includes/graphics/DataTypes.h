#pragma once
#include <unordered_map>

namespace DirectSheep
{
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

}

extern DirectSheep::DirectX_Core* CORE;
extern DirectSheep::States *STATES;
extern DirectSheep::TextureMap *TEXTUREMAP;
extern DirectSheep::ShapeStates SHAPESTATES;
extern DirectSheep::VertexBufferQuad *QUAD;
extern DirectSheep::ShaderMap SHADERMAP;
