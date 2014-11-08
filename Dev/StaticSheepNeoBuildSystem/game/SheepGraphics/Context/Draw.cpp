#include "precompiled.h"
#include "Context.h"
#include "Handle.h"
#include "SpriteBatch.h"

using namespace DirectX;

namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                     DRAW FUNCTIONS                      //
  /////////////////////////////////////////////////////////////
#define RGBTOBGR(color) (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16)

  void RenderContext::Draw(unsigned vertexCount, unsigned vertexStart)
  {
    iMat4 matFinal;

    iMat4 scaleMat, rotMat, transMat;

    rotMat = DirectX::XMMatrixIdentity();
    transMat = DirectX::XMMatrixIdentity();
    scaleMat = DirectX::XMMatrixIdentity();

    scaleMat = DirectX::XMMatrixScaling(m_spriteTrans.w, m_spriteTrans.h, 1.0f);
    rotMat = DirectX::XMMatrixRotationZ(m_spriteTrans.theta);
    scaleMat = DirectX::XMMatrixMultiply(scaleMat, rotMat);

    transMat = DirectX::XMMatrixTranslation(m_spriteTrans.x, m_spriteTrans.y, 0.0f);

    scaleMat = DirectX::XMMatrixMultiply(scaleMat, transMat);

    matFinal = scaleMat * m_camera.viewProj;

    DefaultBuffer buffer;
    buffer.Final = matFinal;
    buffer.World = scaleMat;
    buffer.BlendColor = m_spriteBlend;
    buffer.uvBegin = m_spriteTrans.uvBegin;
    buffer.uvEnd = m_spriteTrans.uvEnd;

    m_deviceContext->RSSetState(m_rastState[m_currentRast]);

    m_deviceContext->PSSetSamplers(0, 1, &m_sampleStates[0]);

    SetBlendMode(BLEND_MODE_ALPHA);

    m_deviceContext->UpdateSubresource(m_constBufferRes[0], 0, 0, &buffer, 0, 0);

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_deviceContext->Draw(vertexCount, vertexStart);
  }

  void RenderContext::DrawSpriteText(const char * text, float size, const char * font)
  {
    iMat4 matFinal;

    iMat4 rotMat, transMat;

    matFinal = XMMatrixIdentity();
    rotMat = XMMatrixIdentity();
    transMat = XMMatrixIdentity();

    rotMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_spriteTrans.theta);

    transMat = XMMatrixTranslation(m_spriteTrans.x, m_spriteTrans.y, 0.0f);

    rotMat = XMMatrixMultiply(rotMat, transMat);

    matFinal = rotMat * m_camera.viewProj;

    FW1_RECTF rect;
    rect.Left = rect.Right = 0.0f;
    rect.Top = rect.Bottom = 0.0f;

    std::string boop(text);
    std::wstring test(boop.begin(), boop.end());

    std::string sfont(font);
    std::wstring WFont(sfont.begin(), sfont.end());

    m_font.m_fontWrapper->DrawString(
      m_deviceContext,
      test.c_str(),// String
      WFont.c_str(),
      size,
      &rect,
      (UINT32)0xFFFFFFFF,
      //RGBTOBGR(Vec3(m_spriteBlend.z, m_spriteBlend.y, m_spriteBlend.x, m_spriteBlend.w)),// Text color, 0xAaBbGgRr
      NULL,
      (float*)((Mat4*)(&matFinal))->m,
      FW1_RESTORESTATE | FW1_LEFT | FW1_VCENTER | FW1_NOWORDWRAP
      );// Flags (for example FW1_RESTORESTATE to keep context states 

  }

  void RenderContext::DrawBatched(DirectSheep::Handle texture)
  {
    unsigned width = GetTextureSize(texture).width;
    unsigned height = GetTextureSize(texture).height;

    RECT sourcePos;
    sourcePos.left = (long)(width * m_spriteTrans.uvBegin.x);
    sourcePos.right = (long)(width * m_spriteTrans.uvEnd.x);
    sourcePos.top = (long)(height * m_spriteTrans.uvBegin.y);
    sourcePos.bottom = (long)(height * m_spriteTrans.uvEnd.y);

    m_batcher->Draw(m_textureRes[texture.index].m_ShaderRes,
               Vec2(m_spriteTrans.x, m_spriteTrans.y),
               &sourcePos,
               XMLoadFloat4(&m_spriteBlend),
               m_spriteTrans.theta,
               Vec2((sourcePos.right - sourcePos.left) / 2.0f, (sourcePos.bottom - sourcePos.top) / 2.0f),
               Vec2(m_spriteTrans.w, -m_spriteTrans.h), DirectX::SpriteEffects_None,-m_spriteTrans.z);
  }


  void RenderContext::frameStart(void)
  {
    ClearBackBuffer();
    ClearDepthBuffer();
  }

  void RenderContext::StartBatch()
  {
    m_batcher->Begin(SpriteSortMode_Texture, m_blendStateMap[BLEND_MODE_ALPHA], m_sampleStates[0], m_depthBuffer.m_depthState, m_rastState[m_currentRast], nullptr, m_camera.viewProj);
  }

  void RenderContext::EndBatch()
  {
    m_batcher->End();
  }

  void RenderContext::frameEnd(void)
  {
    Present();
  }

  void RenderContext::Present(void)
  {
    m_swapChain->Present(m_vsync, 0);
  }
}