#pragma once

namespace DirectSheep
{
  struct DirectX_Core
  {
    IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
    ID3D11Device *dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
    ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
    ID3D11InputLayout *pLayout;            // the pointer to the input layout
    ID3D11VertexShader *pVS;               // the pointer to the vertex shader
    ID3D11PixelShader *pPS;                // the pointer to the pixel shader
    ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer
  };

  struct States
  {
    ID3D11RasterizerState *RSDefault;    // the default rasterizer state
    //ID3D11RasterizerState *pRSWireFrame;  // a rasterizer using wireframe
    ID3D11SamplerState *SS;              // three sampler states
    ID3D11BlendState *BS;                // a typical blend state
  };

  struct shapeStates
  {
    Vec2 position;
    Vec2 scale;
    float rotation;
    Vec4 Color;
    std::string filename;
    bool useCamera;
  };

  typedef std::map<std::string, ID3D11ShaderResourceView*> TextureMap;

	template< typename RefType >
	void SafeRelease( RefType& interfacePtr )
	{
		if( interfacePtr ) interfacePtr->Release();
		interfacePtr = NULL;
	}
}

extern DirectSheep::DirectX_Core* CORE;
extern DirectSheep::States *STATES;
extern DirectSheep::TextureMap TEXTUREMAP;
extern float ScreenWidth;
extern float ScreenHeight;