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





  private:

  private:

    D3D_FEATURE_LEVEL m_featureLevel;
    D3D_DRIVER_TYPE m_driverType;

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGIFactory* m_factory;

  };
}
