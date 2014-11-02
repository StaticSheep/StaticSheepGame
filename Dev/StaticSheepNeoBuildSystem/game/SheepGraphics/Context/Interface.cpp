#include "precompiled.h"
#include "Interface.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DXGI.lib")
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
    m_spriteBlend(Vec4(1, 1, 1, 1)),
    m_graphics(new Graphics())
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
    m_Context = new RenderContext(m_graphics->getFactory(), m_graphics->getDevice(), m_graphics->getContext(), hwnd, (UINT)width, (UINT)height, false);
    int desktopSizeX = GetSystemMetrics(SM_CXSCREEN);
    int desktopSizeY = GetSystemMetrics(SM_CYSCREEN);

    int windowSizeX = static_cast<int>(desktopSizeX * 0.6f);
    int windowSizeY = static_cast<int>(desktopSizeY * 0.6f);

    // Create the input object
    SIZE windowSize = { windowSizeX, windowSizeY };
    PositionVertex vertices[4] = {
        { Vec3(-windowSizeX / 2.f, windowSizeY / 2.f, 0.f) }, // TOP LEFT
        { Vec3(windowSizeX / 2.f, windowSizeY / 2.f, 0.f) }, // TOP RIGHT
        { Vec3(-windowSizeX / 2.f, -windowSizeY / 2.f, 0.f) }, // BOTTOM LEFT
        { Vec3(windowSizeX / 2.f, -windowSizeY / 2.f, 0.f) }, // BOTTOM RIGHT
    };

    UINT indices[6] = {
      0, 1, 2,
      2, 1, 3
    };

    m_posModel = new Model<PositionVertex>(m_graphics->getDevice(), vertices, 4, indices, 6);

    m_EffectRes.push_back( new PointLight(m_graphics->getDevice()));

    m_world = XMMatrixIdentity();
    m_view = XMMatrixLookAtLH(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f), Vec3(0.f, 1.f, 0.f));
    m_projection = XMMatrixOrthographicLH(static_cast<float>(windowSizeX), static_cast<float>(windowSizeY),
      0.f, 100.f);

    m_cursorLight = Light(
      Vec3(0.f, 0.f, 0.f),
      Color(1.0f, 1.0f, 1.0f, 1.f),
      Vec3(0.050097f, 0.101329f, 0.007211f));


    CreateFontWrapper();
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
      return m_graphics->getDevice();
    }

}