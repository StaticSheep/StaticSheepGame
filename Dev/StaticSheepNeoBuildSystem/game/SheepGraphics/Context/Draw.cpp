#include "precompiled.h"
#include "Interface.h"
#include "Handle.h"
#include "SpriteBatch.h"

using namespace DirectX;

namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                     DRAW FUNCTIONS                      //
  /////////////////////////////////////////////////////////////
#define RGBTOBGR(color) (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16)

  void Interface::Draw(unsigned vertexCount, unsigned vertexStart)
  {
  }

  

  void Interface::DrawSpriteText(const char * text, float size, const char * font)
  {
    iMat4 matFinal;

    iMat4 rotMat, transMat;

    matFinal = XMMatrixIdentity();
    rotMat = XMMatrixIdentity();
    transMat = XMMatrixIdentity();


    rotMat = XMMatrixRotationRollPitchYaw((float)XM_PI, 0.0f, m_theta);


    transMat = XMMatrixTranslation(m_position.x, m_position.y, 0.0f);

    rotMat = XMMatrixMultiply(rotMat, transMat);

    matFinal = rotMat * m_camera.viewProj;

    FW1_RECTF rect;
    rect.Left = rect.Right = 0.0f;
    rect.Top = rect.Bottom = 0.0f;

    std::string boop(text);
    std::wstring test(boop.begin(), boop.end());

    std::string sfont(font);
    std::wstring WFont(sfont.begin(), sfont.end());

    m_FontWrapper->DrawString(
      m_graphics->getContext(),
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


  void Interface::DrawBatched(DirectSheep::Handle texture)
  {
    //m_Context->renderBatch(m_TextureRes[texture.index], m_position, m_BlendCol, m_theta, m_scale, m_UV);
  }


  void Interface::frameStart(void)
  {
    m_Context->startFrame(m_clearColor);
    StartBatch();
  }

  void Interface::StartBatch()
  {
    //m_Context->startBatch(Mat4(m_camera.viewProj));
  }

  void Interface::EndBatch()
  {
    //m_Context->endBatch();
  }

  void Interface::frameEnd(void)
  {
    m_Context->endFrame();

    m_EffectRes[0]->bindMatrices(m_graphics->getContext(), m_projection, m_view, m_world);
    m_EffectRes[0]->bindLight(m_graphics->getContext(), m_cursorLight);
    m_Context->render(m_posModel, m_EffectRes[0], BlendStates::ADDITIVE);

    Present();
  }

  void Interface::Present(void)
  {
    m_Context->present();
  }
}