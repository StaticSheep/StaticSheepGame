#include "precompiled.h"
#include "Interface.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>
using namespace DirectX;
namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                    CREATE FUNCTIONS                     //
  /////////////////////////////////////////////////////////////

  void Interface::CreateTexture(Handle& handle, const std::string& filename)
  {
    Tex2D temp(m_graphics.getDevice(), filename);

    m_TextureRes.push_back(temp);
    handle.type = TEXTURE;
    handle.index = m_TextureRes.size() - 1;
    m_handles.push_back(handle);
  }

  /*bool Interface::CreateFontWrapper(void)
  {
    DXVerify(FW1CreateFactory(FW1_VERSION, &m_font.m_fontFactory));

    FW1_FONTWRAPPERCREATEPARAMS Params;
    ZeroMemory(&Params, sizeof(Params));

    Params.SheetMipLevels = 5;
    Params.AnisotropicFiltering = TRUE;
    Params.DefaultFontParams.pszFontFamily = L"Arial";
    Params.DefaultFontParams.FontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    Params.DefaultFontParams.FontStyle = DWRITE_FONT_STYLE_NORMAL;
    Params.DefaultFontParams.FontStretch = DWRITE_FONT_STRETCH_NORMAL;

    DXVerify(m_font.m_fontFactory->CreateFontWrapper(m_device, NULL, &Params, &m_font.m_fontWrapper));

    m_font.m_fontFactory->Release();
    m_font.m_fontFactory = NULL;


    return true;
  }*/

  /////////////////////////////////////////////////////////////
  //              PRIVATE INITIALIZE FUNCTIONS               //
  /////////////////////////////////////////////////////////////

}