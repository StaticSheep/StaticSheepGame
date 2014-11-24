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
    Mat4 matFinal;

    Mat4 scaleMat, rotMat, transMat;

    rotMat = DirectX::XMMatrixIdentity();
    transMat = DirectX::XMMatrixIdentity();
    scaleMat = DirectX::XMMatrixIdentity();

    scaleMat = DirectX::XMMatrixScaling(m_spriteTrans.w, m_spriteTrans.h, 1.0f);
    rotMat = DirectX::XMMatrixRotationZ(m_spriteTrans.theta);
    scaleMat = DirectX::XMMatrixMultiply(scaleMat, rotMat);


    transMat = DirectX::XMMatrixTranslation(m_camUse ? m_spriteTrans.x :
      m_spriteTrans.x + m_spriteTrans.w / 2,
      m_camUse ? m_spriteTrans.y : -m_spriteTrans.y - m_spriteTrans.h / 2, 0.0f);

    scaleMat = DirectX::XMMatrixMultiply(scaleMat, transMat);

    matFinal = scaleMat * ((Camera*)m_camera.ptr)->getViewProj();

    m_deviceContext->RSSetState(m_rastState[m_currentRast]);

    m_deviceContext->PSSetSamplers(0, 1, &m_sampleStates[0]);

    SetBlendMode(BLEND_MODE_ALPHA);

    m_genericEffect->bind(m_deviceContext);
    m_genericEffect->bindPosUV(m_deviceContext, ((Camera*)m_camera.ptr)->getProj(), ((Camera*)m_camera.ptr)->getView(), scaleMat, Vec2(0,0), Vec2(1,1));
    m_genericEffect->bindAmbient (m_deviceContext, m_spriteBlend, 1);

    m_deviceContext->Draw(vertexCount, vertexStart);
  }

  void RenderContext::DrawSpriteText(const char * text, float size, const char * font)
  {
    Mat4 matFinal;

    Mat4 rotMat, transMat;

    matFinal = XMMatrixIdentity();
    rotMat = XMMatrixIdentity();
    transMat = XMMatrixIdentity();


    rotMat = XMMatrixRotationRollPitchYaw(-DirectX::XM_PI, 0.0f, m_spriteTrans.theta);


    transMat = XMMatrixTranslation(m_spriteTrans.x, m_camUse ? m_spriteTrans.y : -m_spriteTrans.y, 0.0f);

    rotMat = XMMatrixMultiply(rotMat, transMat);

    matFinal = rotMat * ((Camera*)m_camera.ptr)->getViewProj();

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
      //(UINT32)0xFFFFFFFF,
      DirectX::PackedVector::XMCOLOR(m_spriteBlend.z * 255, m_spriteBlend.y * 255,
      m_spriteBlend.x * 255, m_spriteBlend.w * 255),// Text color, 0xAaBbGgRr
      NULL,
      (float*)(&matFinal)->m,
      FW1_RESTORESTATE | FW1_LEFT | FW1_TOP | FW1_NOWORDWRAP
      );// Flags (for example FW1_RESTORESTATE to keep context states 

  }

  void RenderContext::DrawBatched(DirectSheep::Handle texture)
  {
    unsigned width = GetTextureSize(texture).width;
    unsigned height = GetTextureSize(texture).height;

    DirectX::SpriteEffects effect = DirectX::SpriteEffects_None;

    RECT sourcePos;
    sourcePos.left = (long)(width * (m_spriteTrans.uvBegin.x));
    sourcePos.right = (long)(width * (m_spriteTrans.uvEnd.x));
    sourcePos.top = (long)(height * m_spriteTrans.uvBegin.y);
    sourcePos.bottom = (long)(height * m_spriteTrans.uvEnd.y);

    m_batcher->Draw(m_textureRes[texture.index].m_ShaderRes,
               Vec2(m_spriteTrans.x, m_spriteTrans.y),
               &sourcePos,
               XMLoadFloat4(&m_spriteBlend),
               m_spriteTrans.theta,
               Vec2((sourcePos.right - sourcePos.left) / 2.0f, (sourcePos.bottom - sourcePos.top) / 2.0f),
               Vec2(m_spriteTrans.w, -m_spriteTrans.h), m_flip, m_spriteTrans.z);
  }


  void RenderContext::frameStart(void)
  {
    ClearBackBuffer();
    ClearDepthBuffer();
  }

  void RenderContext::StartBatch()
  {
    m_batcher->Begin(SpriteSortMode_Texture, m_blendStateMap[BLEND_MODE_ALPHA], m_sampleStates[0], m_depthBuffer.m_depthState, m_rastState[m_currentRast], nullptr, ((Camera*)m_camera.ptr)->getViewProj());
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