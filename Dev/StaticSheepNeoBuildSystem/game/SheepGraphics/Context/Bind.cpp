/*****************************************************************
Filename: Bind.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Context.h"
#include "Handle.h"

using namespace DirectX;
namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                    BIND FUNCTIONS                       //
  /////////////////////////////////////////////////////////////

  /*!
      \brief
      Used for sprite flipping animation
  */
  void RenderContext::SetSpriteFlip(bool xFlip, bool yFlip)
  {
    if (xFlip && yFlip)
      m_flip = DirectX::SpriteEffects_FlipBoth;
    else if (xFlip)
      m_flip = DirectX::SpriteEffects_FlipHorizontally;
    else if (yFlip)
      m_flip = DirectX::SpriteEffects_FlipVertically;
    else
      m_flip = DirectX::SpriteEffects_None;
  }

  /*!
      \brief
      Toggles wirframe for debug draw
  */
  void RenderContext::setWireFrame(bool isWired)
  {
    if (isWired)
      m_currentRast = RastStates::Wire;
    else
      m_currentRast = RastStates::Fill;
  }

  /*!
      \brief
      Resizes window when change in resolution occurs
  */
  void RenderContext::Resize(float width, float height)
  {
    // Swapchain is required here
    if (m_swapChain)
    {
      // Calculate new dimensions
      m_viewport.dim = Dimension((unsigned)width, (unsigned)height);
      m_viewport.offsetX = 0;
      m_viewport.offsetY = 0;

      // Disengage Swapchain from backbuffer
      m_deviceContext->OMSetRenderTargets(0, 0, 0);

      // Release back buffer and depth buffer
      m_backBuffer->Release();
      m_depthBuffer.m_depthBuffer->Release();
      m_depthBuffer.texture2D->Release();

      // Resize swapchain to new resolution
      DXVerify(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

      // Recreate depth buffer and back buffer
      CreateDepthBuffer();

      InitializeBackBuffer();

      // Recreate viewport
      SetViewport(0, 0, Dimension((unsigned)width, (unsigned)height));

      // Update orthoscreen camera to match
      ((Camera*)m_orthoScreen.ptr)->SetScale(width, height);
    }
  }

  /*!
      \brief
      Bind a texture by handle
  */
  void RenderContext::BindTexture(unsigned slot, const Handle& texHandle)
  {
    if (texHandle.type == TEXTURE)
      m_deviceContext->PSSetShaderResources(slot, 1, &m_textureRes[texHandle.index].m_ShaderRes);
  }

  /*!
      \brief
      Bind multiple textures, useful for certain shaders
  */
  void RenderContext::BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot)
  {
    for (unsigned i = 0; i < count; ++i)
    {
      if (texHandles[i].type == TEXTURE)
        m_deviceContext->PSSetShaderResources(startSlot + i, 1, &m_textureRes[texHandles[i].index].m_ShaderRes);
    }
  }

  /*!
      \brief
      Bind a single vertex buffer to the device
  */
  void RenderContext::BindVertexBuffer(const Handle& vbHandle, size_t stride, size_t offset)
  {
    if (vbHandle.type == VERTEX_BUFFER)
      m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vbHandle.index], &stride, &offset);
  }

  /*!
      \brief
      Bind multiple vertex buffers to the device
  */
  void RenderContext::BindVertexBuffers(unsigned count, const Handle vertexBuffers[], size_t strides[], size_t offsets[])
  {
    for (unsigned i = 0; i < count; ++i)
    {
      if (vertexBuffers[i].type == VERTEX_BUFFER)
        m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vertexBuffers[i].index], &strides[i], &offsets[i]);
    }
  }

  /*!
      \brief
      Bind an index buffer
  */
  void RenderContext::BindIndexBuffer(const Handle& ibHandle)
  {
    if (ibHandle.type == INDEX_BUFFER)
      m_deviceContext->IASetIndexBuffer(m_indexBufferRes[ibHandle.index], DXGI_FORMAT_R32_UINT, 0);
  }

  /*!
      \brief
      Binds a constant buffer to both pixel and vertex shader
  */
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

  /*!
      \brief
      Bind to a new render target
  */
  void RenderContext::BindRenderTarget(const Handle& rtHandle)
  {
    if (rtHandle.type == RENDER_TARGET)
      m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthBuffer.m_depthBuffer);
  }
}