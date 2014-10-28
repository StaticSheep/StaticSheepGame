#include "precompiled.h"
#include "Context.h"
#include "Handle.h"

using namespace DirectX;
namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                    BIND FUNCTIONS                       //
  /////////////////////////////////////////////////////////////

  void RenderContext::BindVertexShader(const Handle& vsHandle)
  {
    if (vsHandle.type == VERTEX_SHADER)
    {
      m_deviceContext->VSSetShader(m_vertexShaderRes[vsHandle.index].vShader, 0, 0);
      m_deviceContext->IASetInputLayout(m_vertexShaderRes[vsHandle.index].inputLayout);
    }
  }

  void RenderContext::BindPixelShader(const Handle& psHandle)
  {
    if (psHandle.type == PIXEL_SHADER)
    {
      m_deviceContext->PSSetShader(m_pixelShaderRes[psHandle.index], 0, 0);
    }
  }

  void RenderContext::BindTexture(unsigned slot, const Handle& texHandle)
  {
    if (texHandle.type == TEXTURE)
      m_deviceContext->PSSetShaderResources(slot, 1, &m_textureRes[texHandle.index].shaderResourceView);
  }

  void RenderContext::BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot)
  {
    for (unsigned i = 0; i < count; ++i)
    {
      if (texHandles[i].type == TEXTURE)
        m_deviceContext->PSSetShaderResources(startSlot + i, 1, &m_textureRes[texHandles[i].index].shaderResourceView);
    }
  }

  void RenderContext::BindVertexBuffer(const Handle& vbHandle, size_t stride, size_t offset)
  {
    if (vbHandle.type == VERTEX_BUFFER)
      m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vbHandle.index], &stride, &offset);
  }

  void RenderContext::BindVertexBuffers(unsigned count, const Handle vertexBuffers[], size_t strides[], size_t offsets[])
  {
    for (unsigned i = 0; i < count; ++i)
    {
      if (vertexBuffers[i].type == VERTEX_BUFFER)
        m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vertexBuffers[i].index], &strides[i], &offsets[i]);
    }
  }

  void RenderContext::BindIndexBuffer(const Handle& ibHandle)
  {
    if (ibHandle.type == INDEX_BUFFER)
      m_deviceContext->IASetIndexBuffer(m_indexBufferRes[ibHandle.index], DXGI_FORMAT_R32_UINT, 0);
  }

  void RenderContext::BindConstantBuffer(unsigned slot, const Handle& cbHandle, ObjectType shaderType)
  {
    if (cbHandle.type == CONSTANT_BUFFER)
    {
      if (shaderType == VERTEX_SHADER)
        m_deviceContext->VSSetConstantBuffers(slot, 1, &m_constBufferRes[cbHandle.index]);
      if (shaderType == PIXEL_SHADER)
        m_deviceContext->PSSetConstantBuffers(slot, 1, &m_constBufferRes[cbHandle.index]);
    }
  }

  void RenderContext::BindRenderTarget(const Handle& rtHandle)
  {
    if (rtHandle.type == RENDER_TARGET)
      m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthBuffer.m_depthBuffer);
  }
}