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
    ID3D11DepthStencilView *zbuffer;       // Pointer to depth buffer

    ID3D11VertexShader *pVS;               // Pointer to current Vertex Shader
    ID3D11PixelShader *pPS;                // Pointer to current Pixel Shader
    ID3D11InputLayout *pLayout;            // Pointer to current Input Layout

    ID3D11Buffer *pCBuffer;                // Pointer to constant buffer

    IFW1Factory *pFW1Factory;
	
    IFW1FontWrapper *pFontWrapper;
  };

  struct States
  {
    ID3D11RasterizerState *RSDefault;    // Standard rasterizer state
    //ID3D11RasterizerState *pRSWireFrame;  // Wireframe (primitive debug draw)
    ID3D11SamplerState *SS;              // Sampler State
    ID3D11BlendState *BS;                // Blend State (alpha, blending)
  };

  struct ShapeStates
  {
    Vec2 position;
    Vec2 scale;
    float rotation;
    Vec4 Color;
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
    ID3D11Buffer *vBuffer;
  };

	template< typename RefType >
	void SafeRelease( RefType& interfacePtr )
	{
		if( interfacePtr ) interfacePtr->Release();
		interfacePtr = NULL;
	}
}

extern DirectSheep::DirectX_Core* CORE;
extern DirectSheep::States *STATES;
extern DirectSheep::TextureMap *TEXTUREMAP;
extern DirectSheep::ShapeStates SHAPESTATES;
extern DirectSheep::VertexBufferQuad *QUAD;
extern DirectSheep::ShaderMap SHADERMAP;

extern float ScreenWidth;
extern float ScreenHeight;