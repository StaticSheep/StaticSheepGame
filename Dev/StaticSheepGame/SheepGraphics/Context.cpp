#include "Context.h"
namespace DirectSheep
{
  RenderContext::RenderContext(void)
  {
  }

  RenderContext::~RenderContext(void)
  {
  }

  //Returns true if the RenderContext is Initialized, else false
  bool RenderContext::IsInitialized(void) const
  {
    return m_initialized;
  }

  //Initializes the RenderContext
  //Returns true if successful, else false
  bool RenderContext::Initialize(HWND hwnd, float height, float width)
  {

  }

  //Uninitializes the RenderContext
  void RenderContext::Uninitialize(void)
  {
  }

  /////////////////////////////////////////////////////////////
  //              PRIVATE INITIALIZE FUNCTIONS               //
  /////////////////////////////////////////////////////////////

  void RenderContext::InitializeDXGI(void)
  {
  }
  void RenderContext::InitializeDisplayModes(void)
  {
  }
  void RenderContext::InitializeDeviceAndSwapChain(void)
  {
  }
  //void InitializeRasterizerState(void);
  void RenderContext::InitializeBackBuffer(void)
  {
  }
  void RenderContext::InitializeBlendModes(void)
  {
  }
  void RenderContext::InitializeSamplerState(void)
  {
    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    DXVerify(m_device->CreateSamplerState(&sd, &m_sampleStates[0]));
  }

  /////////////////////////////////////////////////////////////
  //               INTERNAL RELEASE FUNCTIONS                //
  /////////////////////////////////////////////////////////////

  void RenderContext::ReleaseTextureIntern(const Handle& texture)
  {
  }
  void RenderContext::ReleaseVertexShaderIntern(const Handle& vertexShader)
  {
  }
  void RenderContext::ReleasePixelShaderIntern(const Handle& pixelShader)
  {
  }
  void RenderContext::ReleaseVertexBufferIntern(const Handle& vertexBuffer)
  {
  }
  void RenderContext::ReleaseIndexBufferIntern(const Handle& indexBuffer)
  {
  }
  void RenderContext::ReleaseConstantBufferIntern(const Handle& constantBuffer)
  {
  }
  void RenderContext::ReleaseRenderTargetIntern(const Handle& renderTarget)
  {
  }
}