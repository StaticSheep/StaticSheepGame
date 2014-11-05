#pragma once

#include "Vertices.h"
#include "DirectXIncludes.h"
#include "vertex_types.h"

template<class VertexType>
class Model
{
public:

  Model(ID3D11Device* pDevice,
    VertexType* vertData, UINT numVerts,
    UINT* indexData, UINT numIndices, bool isDynamic = false) :
    m_numIndices(numIndices),
    m_numVerts(numVerts),
    m_vertexBuf(nullptr),
    m_indexBuf(nullptr)
  {
    DXVerify(loadFromData(pDevice, vertData, numVerts, indexData, numIndices, isDynamic));
  }

  ~Model() {}

  void bind(ID3D11DeviceContext* deviceContext)
  {
    UINT Stride = sizeof(VertexType);
    UINT offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuf, &Stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuf, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  }

  UINT getIndexCount() const
  {
    return m_numIndices;
  }

  UINT getVertexCount() const
  {
    return m_numVerts;
  }

  HRESULT loadFromData(ID3D11Device* dev,
    VertexType* vertData, UINT numVerts,
    UINT* indexData, UINT numIndices, bool Dynamic)
  {
    if (!dev || !vertData || !indexData)
      return E_INVALIDARG;

    HRESULT result(S_OK);

    D3D11_BUFFER_DESC vBufDesc;

    ZeroMemory(&vBufDesc, sizeof(vBufDesc));

    vBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBufDesc.ByteWidth = sizeof(VertexType) * numVerts;
    vBufDesc.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    vBufDesc.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

    D3D11_SUBRESOURCE_DATA vBufSub;
    vBufSub.pSysMem = vertData;
    vBufSub.SysMemPitch = 0;
    vBufSub.SysMemSlicePitch = 0;

    result = dev->CreateBuffer(&vBufDesc, &vBufSub, &m_vertexBuf);

    if (SUCCEEDED(result))
    {
      D3D11_BUFFER_DESC iBufDesc;
      ZeroMemory(&iBufDesc, sizeof(iBufDesc));
      iBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
      iBufDesc.ByteWidth = sizeof(UINT) * numIndices;
      iBufDesc.Usage = D3D11_USAGE_DEFAULT;
      iBufDesc.CPUAccessFlags = 0;

      D3D11_SUBRESOURCE_DATA iBufSub;
      iBufSub.pSysMem = indexData;
      iBufSub.SysMemPitch = 0;
      iBufSub.SysMemSlicePitch = 0;

      result = dev->CreateBuffer(&iBufDesc, &iBufSub, &m_indexBuf);
    }
    return result;
  }

private:


  UINT m_numIndices;
  UINT m_numVerts;

  ID3D11Buffer *m_vertexBuf;
  ID3D11Buffer *m_indexBuf;
};