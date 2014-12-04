/*****************************************************************
Filename: Draw.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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

 // Bitwise conversion from RGB format to BGR format
#define RGBTOBGR(color) (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16)

  /*!
      \brief
      Unbatched draw call
  */
  void RenderContext::Draw(unsigned vertexCount, unsigned vertexStart)
  {
    // Matrix used for final transformation
    Mat4 matFinal;

    // Intermediates
    Mat4 scaleMat, rotMat, transMat;

    // Set all to identity
    rotMat = DirectX::XMMatrixIdentity();
    transMat = DirectX::XMMatrixIdentity();
    scaleMat = DirectX::XMMatrixIdentity();

    // Rotate
    rotMat = DirectX::XMMatrixRotationZ(m_spriteTrans.theta);

    // Scale
    scaleMat = DirectX::XMMatrixScaling(m_spriteTrans.w, m_spriteTrans.h, 1.0f);
    
    // Translate
    transMat = DirectX::XMMatrixTranslation(m_camUse ? m_spriteTrans.x :
      m_spriteTrans.x + m_spriteTrans.w / 2,
      m_camUse ? m_spriteTrans.y : -m_spriteTrans.y - m_spriteTrans.h / 2, 0.0f);

    // Scale x Rotate
    scaleMat = DirectX::XMMatrixMultiply(scaleMat, rotMat);

    // Trans x scale
    scaleMat = DirectX::XMMatrixMultiply(scaleMat, transMat);

    // ObjectToWorld x ViewProj: TODO find out why this was working
    matFinal = ((Camera*)m_camera.ptr)->getViewProj()  * scaleMat;

    // Bind raster state
    m_deviceContext->RSSetState(m_rastState[m_currentRast]);

    // Bind sampler state
    m_deviceContext->PSSetSamplers(0, 1, &m_sampleStates[0]);

    // Alpha yay
    SetBlendMode(BLEND_MODE_ALPHA);

    // Bind general case shader
    m_genericEffect->bind(m_deviceContext);
    m_genericEffect->bindPosUV(m_deviceContext, ((Camera*)m_camera.ptr)->getProj(), ((Camera*)m_camera.ptr)->getView(), scaleMat, Vec2(0,0), Vec2(1,1));
    m_genericEffect->bindAmbient (m_deviceContext, m_spriteBlend, 1);

    // Draw
    m_deviceContext->Draw(vertexCount, vertexStart);
  }

  /*!
      \brief
      Draw text to screen
  */
  void RenderContext::DrawSpriteText(const char * text, float size, const char * font)
  {
    // Resulting transformation matrix
    Mat4 matFinal;

    // Intermediates
    Mat4 rotMat, transMat;

    // Null out
    matFinal = XMMatrixIdentity();
    rotMat = XMMatrixIdentity();
    transMat = XMMatrixIdentity();

    // Configure rotation
    rotMat = XMMatrixRotationRollPitchYaw(-DirectX::XM_PI, 0.0f, m_spriteTrans.theta);

    // Translate
    transMat = XMMatrixTranslation(m_spriteTrans.x,
      m_camUse ? m_spriteTrans.y : -m_spriteTrans.y, 0.0f);

    // Roatate
    rotMat = XMMatrixMultiply(rotMat, transMat);

    // Merge with camera
    matFinal = rotMat * ((Camera*)m_camera.ptr)->getViewProj();

    FW1_RECTF rect;
    rect.Left = rect.Right = 0.0f;
    rect.Top = rect.Bottom = 0.0f;

    std::string boop(text);
    std::wstring test(boop.begin(), boop.end());

    std::string sfont(font);
    std::wstring WFont(sfont.begin(), sfont.end());

    // Draw the string
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

  /*!
      \brief
      Batch a draw call
  */
  GFX_API void RenderContext::DrawBatched(DirectSheep::Handle texture)
  {
    unsigned width = GetTextureSize(texture).width;
    unsigned height = GetTextureSize(texture).height;

    DirectX::SpriteEffects effect = DirectX::SpriteEffects_None;

    m_spriteTrans.x = m_camUse ? m_spriteTrans.x :
      m_spriteTrans.x + m_spriteTrans.w / 2;

    m_spriteTrans.y = m_camUse ? m_spriteTrans.y :
      -m_spriteTrans.y - m_spriteTrans.h / 2;
      
    m_spriteTrans.w = m_camUse ? m_spriteTrans.w :
      m_spriteTrans.w / width;

    m_spriteTrans.h = m_camUse ? m_spriteTrans.h :
      m_spriteTrans.h / height;

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
               Vec2((sourcePos.right - sourcePos.left) / 2.0f,
               (sourcePos.bottom - sourcePos.top) / 2.0f),
               Vec2(m_spriteTrans.w, -m_spriteTrans.h), m_flip, m_spriteTrans.z);
  }


  void RenderContext::frameStart(void)
  {
    ClearBackBuffer();
    ClearDepthBuffer();
  }

  void RenderContext::StartBatch()
  {
    m_batcher->Begin(SpriteSortMode_Texture, m_blendStateMap[BLEND_MODE_ALPHA],
      m_sampleStates[0], m_depthBuffer.m_depthState,
      m_rastState[m_currentRast], nullptr,
      ((Camera*)m_camera.ptr)->getViewProj());
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