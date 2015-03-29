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

  void RenderContext::SetWireFrame(bool isWired)
  {
    if (isWired)
      m_currentRast = RastStates::Wire;
    else
      m_currentRast = RastStates::Fill;
  }

  void RenderContext::Resize(float width, float height)
  {
    if (m_swapChain)
    {
      float TargetAspect = 1920.0f / 1080.0f;
      float viewportHeight = width / TargetAspect;
      float viewportWidth = width;

      if (m_letterbox)
      {
        if (viewportHeight > height)
        {
          viewportHeight = height;
          viewportWidth = height * TargetAspect;
        }

        m_viewport.dim = Dimension(viewportWidth, viewportHeight);
        m_viewport.offsetX = (width - viewportWidth) / 2;
        m_viewport.offsetY = (height - viewportHeight) / 2;
        //m_viewport.offsetY = 0;// viewportHeight / 2.0f;
        //m_viewport.offsetX = 0;
        //m_viewport.offsetY = 0;

        ViewportScalar.x = viewportWidth / width;
        ViewportScalar.y = viewportHeight / height;


      }
      else
      {
        m_viewport.dim = Dimension(width, height);
        m_viewport.offsetX = 0;
        m_viewport.offsetY = 0;
      }

      

      SCREEN_WIDTH = width;
      SCREEN_HEIGHT = height;

      m_deviceContext->OMSetRenderTargets(0, 0, 0);

      m_backBuffer->Release();
      m_depthBuffer.m_depthBuffer->Release();
      m_depthBuffer.texture2D->Release();

      DXVerify(m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

      SetViewport(m_viewport.offsetX, m_viewport.offsetY, m_viewport.dim);

      CreateDepthBuffer();

      InitializeBackBuffer();

      //Release(m_lightTarget);
      Release(m_canvasTarget);
      
      //CreateRenderTarget(m_lightTarget, DXGI_FORMAT_R8G8B8A8_UNORM,
      //  Dimension(SCREEN_WIDTH, SCREEN_HEIGHT));

      CreateRenderTarget(m_canvasTarget, DXGI_FORMAT_R8G8B8A8_UNORM,
        Dimension(m_viewport.dim.width, m_viewport.dim.height));

      ((Camera*)m_orthoScreen.ptr)->SetScale(m_viewport.dim.width,
        m_viewport.dim.height);

      ((Camera*)m_postEffects.ptr)->SetScale(m_viewport.dim.width,
        m_viewport.dim.height);



      UpdatePrimativeEffect();
    }
  }

  void RenderContext::BindTexture(unsigned slot, const Handle& texHandle)
  {
    if (texHandle.type == TEXTURE)
      m_deviceContext->PSSetShaderResources(slot, 1, &m_textureRes[texHandle.index].m_ShaderRes);
  }

  void RenderContext::BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot)
  {
    for (unsigned i = 0; i < count; ++i)
    {
      if (texHandles[i].type == TEXTURE)
        m_deviceContext->PSSetShaderResources(startSlot + i, 1, &m_textureRes[texHandles[i].index].m_ShaderRes);
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
      m_deviceContext->IASetIndexBuffer(m_indexBufferRes[ibHandle.index],
      DXGI_FORMAT_R32_UINT, 0);
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
      m_deviceContext->OMSetRenderTargets(1,
      &m_renderTargetRes[rtHandle.index].renderTargetView,
      m_renderTargetRes[rtHandle.index].hasDepthBuffer ? 
      m_renderTargetRes[rtHandle.index].depthBuffer.m_depthBuffer : 
      m_depthBuffer.m_depthBuffer);
  }
}