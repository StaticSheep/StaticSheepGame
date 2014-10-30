#pragma once

#include <list>
#include "DirectXIncludes.h"
#include "Effects\Effect.h"
#include "model.h"

namespace DirectSheep
{
  class Graphics
  {
  public:

    Graphics();
    ~Graphics();

    IDXGIFactory* getFactory() const;

    ID3D11Device* getDevice() const;

    ID3D11DeviceContext* getContext() const;

  private:
    HRESULT createFactory();
    HRESULT createDeviceAndDeviceContext();

  private:

    D3D_FEATURE_LEVEL m_featureLevel;
    D3D_DRIVER_TYPE m_driverType;

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGIFactory* m_factory;

  };
}