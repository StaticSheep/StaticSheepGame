#include "precompiled.h"
#include "Interface.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>

#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

namespace DirectSheep
{

  Interface * Interface::Allocate(void)
  {
    Interface* rc = (Interface*)_aligned_malloc(sizeof(Interface), 16);
    new (rc) Interface();
    return rc;
  }

  void Interface::UpdateCamera(float x, float y, float fov)
  {
    float cameraX = x;
    float cameraY = y;
    float cameraZ;

    if(m_camera.used)
      cameraZ = -400.0f;
    else
      cameraZ = -10.0f;

    Vec4 eyepoint(cameraX, cameraY, cameraZ, 0);

    Vec4 lootAtPoint(cameraX, cameraY, 0.0f, 1.0f);

    Vec4 upVector(0.0f, 1.0f, 0.0f, 0);

    iMat4 matView = XMMatrixLookAtLH(eyepoint, lootAtPoint, upVector);
    m_camera.view = matView;

    iMat4 matProj;
    if(m_camera.used)
      matProj = XMMatrixPerspectiveFovLH((FLOAT)XMConvertToRadians(fov), (float)m_Context->getWidth() / (float)m_Context->getHeight(), 1.0f, 1000.0f);
    else
      matProj = XMMatrixOrthographicLH((float)m_Context->getWidth(), (float)m_Context->getHeight(), 1.0f, 1000.0f);
    m_camera.proj = matProj;

    m_camera.viewProj = matView * matProj;
  }

  Interface::Interface(void) :
    m_initialized(false),
    m_vsync(false),
    m_clearColor(Colors::Black.operator const float *()),
    m_spriteBlend(Vec4(1, 1, 1, 1))
  {
  }

  Interface::~Interface(void)
  {
  }

  //Returns true if the Interface is Initialized, else false
  bool Interface::IsInitialized(void) const
  {
    return m_initialized;
  }

  float* Interface::GetTextureSize(DirectSheep::Handle texture)
  {
    return (float*)&Vec2(m_TextureRes[texture.index].getWidth(), m_TextureRes[texture.index].getHeight());
  }

  //Initializes the Interface
  //Returns true if successful, else false
  bool Interface::Initialize(HWND hwnd, float width, float height)
  {
    m_Context = new RenderContext(m_graphics.getFactory(), m_graphics.getDevice(), m_graphics.getContext(), hwnd, (UINT)width, (UINT)height, false);

    m_initialized = true;
    return true;
  }

  //Uninitializes the Interface
  void Interface::Uninitialize(Interface * rCon)
  {
    rCon->~Interface();
    _aligned_free(rCon);
  }

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

    void Interface::SetClearColor(const float r, const float g, const float b, const float a)
    {
      m_clearColor = Color(r, g, b, a);
    }

    void Interface::SetVSync(bool vsync)
    {
      m_vsync = vsync;
    }
    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

    void Interface::ClearBackBuffer(void)
    {
      m_Context->clearBackBuffer(m_clearColor);
    }

    void Interface::ClearDepthBuffer(void)
    {
      m_Context->clearDepthBuffer();
    }

    void* Interface::ExternalGetDevice(void)
    {
      return m_graphics.getDevice();
    }

}