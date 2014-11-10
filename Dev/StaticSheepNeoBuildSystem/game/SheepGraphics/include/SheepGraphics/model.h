#pragma once

#include "DirectXIncludes.h"
#include "vertex_types.h"

template<class VertexType>
class Model
{
public:

  HRESULT loadFromData(ID3D11Device* dev,
    VertexType* vertData, UINT numVerts,
    UINT* indexData, UINT numIndices, bool Dynamic)
      return E_INVALIDARG;


    D3D11_BUFFER_DESC vBufDesc;


    vBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBufDesc.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    vBufDesc.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

    D3D11_SUBRESOURCE_DATA vBufSub;
    vBufSub.pSysMem = vertData;
    vBufSub.SysMemPitch = 0;
    vBufSub.SysMemSlicePitch = 0;

    return result;
  }

private:


  UINT m_numIndices;
  UINT m_numVerts;

  ID3D11Buffer *m_vertexBuf;
  ID3D11Buffer *m_indexBuf;
};
