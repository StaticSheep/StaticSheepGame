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
    m_genericEffect->bindPosUV(m_deviceContext,
      ((Camera*)m_camera.ptr)->GetProj(),
      ((Camera*)m_camera.ptr)->GetView(),
      scaleMat, Vec2(0,0), Vec2(1,1));


    m_genericEffect->bindAmbient (m_deviceContext, m_spriteBlend, 1);

    m_deviceContext->Draw(vertexCount, vertexStart);
  }

  void RenderContext::DrawSpriteText(const char * text, int index,
    Framework::Vec2D& scale)
  {
    if (index >= m_font.size() || index < 0)
    {
      m_font[0].m_spriteFont->DrawString(m_batcher.get(),
        L"Invalid Font", Vec2(m_spriteTrans.x,
        m_camUse ? m_spriteTrans.y : -m_spriteTrans.y),
        255 * m_spriteBlend, m_spriteTrans.theta,
        Vec2(0, 0), Vec2(1,-1), DirectX::SpriteEffects_None,
        m_spriteTrans.z);

      return;
    }

    std::string sText(text);
    std::wstring wText(sText.begin(), sText.end());

    m_font[index].m_spriteFont->DrawString(m_batcher.get(), wText.c_str(),
      Vec2(m_spriteTrans.x, m_camUse ? m_spriteTrans.y : -m_spriteTrans.y),
      255 * m_spriteBlend, m_spriteTrans.theta, Vec2(0, 0),
      Vec2(scale.x, -scale.y),
      DirectX::SpriteEffects_None, m_spriteTrans.z - 5);
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
    m_PointLights.clear();
  }

  void RenderContext::StartBatch()
  {
    m_batcher->Begin(SpriteSortMode_BackToFront, m_states->NonPremultiplied(),
      m_states->LinearWrap(), m_states->DepthDefault(),
      m_states->CullCounterClockwise(), nullptr,
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

    DebugLine* line;
    while (!m_lineList.empty())
    {
      line = &(m_lineList.top());
      m_primitiveBatch->DrawLine(line->first, line->second);
      m_lineList.pop();
    }

    DebugTriangle* triangle;
    while (!m_triangleList.empty())
    {
      triangle = &(m_triangleList.top());
      m_primitiveBatch->DrawTriangle(std::get<0>(*triangle),
        std::get<1>(*triangle), std::get<2>(*triangle));
      m_triangleList.pop();
    }

    DebugQuad* quad;
    while (!m_quadList.empty())
    {
      quad = &(m_quadList.top());
      m_primitiveBatch->DrawQuad(std::get<0>(*quad),
        std::get<1>(*quad), std::get<2>(*quad),
        std::get<3>(*quad));
      m_quadList.pop();
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

  void RenderContext::DrawQuad(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4)
  {
    if (!m_camUse)
    {
      v1.y = -v1.y;
      v2.y = -v2.y;
      v3.y = -v3.y;
      v4.y = -v4.y;
    }

    m_quadList.push(std::make_tuple(VertexPositionColor(v1, m_spriteBlend),
      VertexPositionColor(v2, m_spriteBlend),
      VertexPositionColor(v3, m_spriteBlend),
      VertexPositionColor(v4, m_spriteBlend)
      ));

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

  void RenderContext::BatchPLight(Framework::Vec3D position, Framework::Vec4D brightness, Framework::Vec3D attenuation)
  {
    // Convert stats from into DirectX container, do scalar ratio
    Vec3 pos(position.x, position.y, position.z);

    Color col(brightness.x / 255.0f, brightness.y / 255.0f,
      brightness.z / 255.0f, brightness.a * 20.0f);

    Vec3 at(attenuation.x / 15000.0f, attenuation.y / 15000.0f,
      attenuation.z / 15000.0f);

    // Store new light in vector, to be drawn at DrawPLights
    m_PointLights.push_back(Light(pos, col, at));
  }

  void RenderContext::DrawPLights(bool isLight)
  {
    // Are there lights to draw
    if (!isLight || !m_PointLights.size())
      return;

    // Bind matrix
    m_PointLight->bindMatrices(m_deviceContext,
      ((Camera*)m_postEffects.ptr)->GetProj(),
      ((Camera*)m_postEffects.ptr)->GetView(),
      DirectX::XMMatrixIdentity());

    // Bind lights to shader
    m_PointLight->bindLights(m_deviceContext,
      m_PointLights.data(),
      m_PointLights.size() < MAX_LIGHTS ? m_PointLights.size() : MAX_LIGHTS);

    // Bind light mesh
    m_PLightModel->bind(m_deviceContext);

    // Bind light effect
    m_PointLight->bind(m_deviceContext);

    // Set appropriate blend mode
    SetBlendMode(BLEND_MODE_MULTIPLY);

    // Draw lights
    m_deviceContext->DrawIndexed(m_PLightModel->getIndexCount(), 0, 0);
  }

}

