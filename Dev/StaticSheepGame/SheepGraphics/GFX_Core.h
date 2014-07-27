#pragma once

namespace Sheep_GFX
{
  struct DirectX_Core
  {
    IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
    ID3D11Device *dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
    ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
    ID3D11InputLayout *pLayout;            // the pointer to the input layout
  };


	template< typename RefType >
	void SafeRelease( RefType& interfacePtr )
	{
		if( interfacePtr ) interfacePtr->Release();
		interfacePtr = NULL;
	}
}