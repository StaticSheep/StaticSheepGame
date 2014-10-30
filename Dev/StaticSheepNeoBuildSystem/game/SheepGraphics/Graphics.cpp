#include "precompiled.h"
#include "Graphics.h"

using namespace DirectX;

namespace DirectSheep
{
  Graphics::Graphics()
  {
    DXVerify(createFactory());

    DXVerify(createDeviceAndDeviceContext());
  }

  Graphics::~Graphics() {}

  IDXGIFactory* Graphics::getFactory() const
  {
    return m_factory;
  }

  ID3D11Device* Graphics::getDevice() const
  {
    return m_Device;
  }

  ID3D11DeviceContext* Graphics::getContext() const
  {
    return m_DeviceContext;
  }

  HRESULT Graphics::createFactory()
  {
    return CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_factory));
  }

  HRESULT Graphics::createDeviceAndDeviceContext()
  {
    D3D_DRIVER_TYPE driverTypes[] =
    {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
      D3D_DRIVER_TYPE_SOFTWARE
    };

    int numDriverTypes = ARRAYSIZE(driverTypes);

    HRESULT result(E_FAIL);
    for (int i = 0; i < numDriverTypes && FAILED(result); ++i)
    {
      m_driverType = driverTypes[i];
      result = D3D11CreateDevice(NULL, m_driverType, NULL, 0, NULL, 0, D3D11_SDK_VERSION,
        &m_Device, &m_featureLevel, &m_DeviceContext);
    }

    return result;
  }
}