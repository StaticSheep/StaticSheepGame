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


#include "Effects.h"
#include "CommonStates.h"
#include <wrl\client.h>


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

    matFinal = scaleMat * ((Camera*)m_camera.ptr)->GetViewProj();

    m_deviceContext->RSSetState(m_rastState[m_currentRast]);

    m_deviceContext->PSSetSamplers(0, 1, &m_sampleStates[0]);

    SetBlendMode(BLEND_MODE_ALPHA);

    m_genericEffect->bind(m_deviceContext);
    m_genericEffect->bindPosUV(m_deviceContext, ((Camera*)m_camera.ptr)->GetProj(), ((Camera*)m_camera.ptr)->GetView(), scaleMat, Vec2(0,0), Vec2(1,1));
    m_genericEffect->bindAmbient (m_deviceContext, m_spriteBlend, 1);

    m_deviceContext->Draw(vertexCount, vertexStart);
  }

  void RenderContext::DrawSpriteText(const char * text, float size, const char * font)
  {

    std::string sText(text);
    std::wstring wText(sText.begin(), sText.end());

    if (m_font.count(font))
      m_font[font]->DrawString(m_batcher.get(), wText.c_str(), Vec2(m_spriteTrans.x, m_camUse ? m_spriteTrans.y : -m_spriteTrans.y), Colors::White, m_spriteTrans.theta, Vec2(0, 0), Vec2(1, -1), DirectX::SpriteEffects_None, 0);
    else
      m_font["Arial"]->DrawString(m_batcher.get(), wText.c_str(), Vec2(m_spriteTrans.x, m_camUse ? m_spriteTrans.y : -m_spriteTrans.y), Colors::White, m_spriteTrans.theta, Vec2(0, 0), Vec2(1, -1), DirectX::SpriteEffects_None, 0);
  }

  void RenderContext::DrawBatched(DirectSheep::Handle texture)
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


  void RenderContext::FrameStart(void)
  {
    ClearBackBuffer();
    ClearDepthBuffer();
  }

  void RenderContext::StartBatch()
  {
    

    m_batcher->Begin(SpriteSortMode_Texture, m_blendStateMap[BLEND_MODE_ALPHA],
      m_sampleStates[0], m_depthBuffer.m_depthState,
      m_rastState[m_currentRast], nullptr,
      ((Camera*)m_camera.ptr)->GetViewProj());
    
  }


  void RenderContext::EndBatch()
  {
    m_batcher->End();
    
  }

  void RenderContext::FrameEnd(void)
  {
    

    m_primativeEffect->Apply(m_deviceContext);
    m_deviceContext->IASetInputLayout(m_primativeLayout);

    m_primitiveBatch->Begin();

    std::pair<VertexPositionColor, VertexPositionColor>* line;

    while (!m_lineList.empty())
    {
      line = &(m_lineList.top());
      m_primitiveBatch->DrawLine(line->first, line->second);
      m_lineList.pop();
    }

    std::tuple<VertexPositionColor,
      VertexPositionColor, VertexPositionColor>* triangle;


    while (!m_triangleList.empty())
    {
      triangle = &(m_triangleList.top());
      m_primitiveBatch->DrawTriangle(std::get<0>(*triangle),
        std::get<1>(*triangle), std::get<2>(*triangle));
      m_triangleList.pop();
    }


    m_primitiveBatch->End();
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Present();
  }

  void RenderContext::Present(void)
  {
    m_swapChain->Present(m_vsync, 0);
  }


  void RenderContext::DrawLine(Vec3 start, Vec3 end,
    Color startColor, Color endColor)
  {
    if (!m_camUse)
    {
      start.y = -start.y;
      end.y = -end.y;
    }
    m_lineList.push(std::make_pair(VertexPositionColor(start, startColor),
      VertexPositionColor(end, endColor)));

    /*m_primitiveBatch->DrawLine(
      VertexPositionColor(start, startColor),
      VertexPositionColor(end, endColor));*/
  }

  void RenderContext::DrawLine(Vec3 start, Vec3 end)
  {
    if (!m_camUse)
    {
      start.y = -start.y;
      end.y = -end.y;
    }
    m_lineList.push(std::make_pair(VertexPositionColor(start, m_spriteBlend),
      VertexPositionColor(end, m_spriteBlend)));
    //m_primitiveBatch->DrawLine(
    //  VertexPositionColor(start, m_spriteBlend),
    //  VertexPositionColor(end, m_spriteBlend));
  }

  void RenderContext::DrawTriangle(Vec3 v1, Vec3 v2, Vec3 v3)
  {
    if (!m_camUse)
    {
      v1.y = -v1.y;
      v2.y = -v2.y;
      v3.y = -v3.y;
    }

    m_triangleList.push(std::make_tuple(VertexPositionColor(v1, m_spriteBlend),
      VertexPositionColor(v2, m_spriteBlend), VertexPositionColor(v3, m_spriteBlend)));

  }


  void RenderContext::DrawPLight(void)
  {
    static float a = 0;
    a += .01;
    m_PointLight->bindMatrices(m_deviceContext, XMMatrixIdentity(), XMMatrixIdentity(), XMMatrixIdentity());

    m_PointLight->bindLight(m_deviceContext, Light(Vec3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), Color(0, 1, 0, 1), Vec3(.050097f, 0.101329f, .007211f)));

    m_PLightModel->bind(m_deviceContext);

    m_PointLight->bind(m_deviceContext);

    SetBlendMode(BLEND_MODE_MULTIPLY);

    m_deviceContext->DrawIndexed(m_PLightModel->getIndexCount(), 0, 0);
  }

}

