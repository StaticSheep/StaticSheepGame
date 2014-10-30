#include "precompiled.h"
#include "Interface.h"
#include "Handle.h"

using namespace DirectX;
namespace DirectSheep
{
  void Interface::SetUV(float x1, float y1, float x2, float y2)
  {
    m_UV = Vec4(x1, y1, x2, y2);
  }
  void Interface::SetPosition(const float x, const float y)
  {
    m_position = Vec2(x, y);
  }
  void Interface::SetRotation(const float theta)
  {
    m_theta = theta;
  }
  void Interface::SetDimensions(const float w, const float h)
  {
    m_scale = Vec2(w, h);
  }
  void Interface::SetBlendCol(const float r, const float g, const float b, const float a)
  {
    m_BlendCol = Color(r, g, b, a);
  }
  void Interface::SetUseCam(bool camUse)
  {
    m_camera.used = camUse;
  }

  void Interface::BindTexture(unsigned slot, const Handle& texHandle)
  {
    m_currTex = &m_TextureRes[texHandle.index];
  }
}