/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "API.h"
#include "Core.h"
#include "WxD3DContext.h"
#include "WxWindow.h"
#include "wx/object.h"

#include <d3d11.h>

//#include <gl/glew.h>
//#include <gl/wglew.h>

#include "d3d/WxD3DCanvas.h"

#include "SheepGraphics/Context.h"

#ifdef D3DGetContext
#undef D3DGetContext
#endif
#define D3DGetContext() (dit::D3D::d3dCtx)

namespace dit {

    //-----------------------------------//

    namespace D3D {

      DirectSheep::RenderContext* d3dCtx = nullptr;

    }

    WxD3DContext::WxD3DContext(WxD3DCanvas* canvas, WxWindow* window)
        : GfxContext(window)
    {

    }

    //-----------------------------------//

    WxD3DContext::~WxD3DContext()
    {
        log("Destroying D3D Context");
        if (D3DGetContext() != nullptr)
        {
          D3DGetContext()->Uninitialize(D3DGetContext());
          D3DGetContext() = nullptr;
        }

    }

    //-----------------------------------//

    void WxD3DContext::MakeCurrent()
    {
        if (userData == nullptr)
        {
            log("WxD3DContext::MakeCurrent() failed because the window handle is null.");
            return;
        }

#if USE_OPENGL
#else

        WxD3DCanvas* canvas = ((WxWindow*) userData)->canvas;
        assert(canvas && "expected a valid wxD3DCanvas instance");


        if (D3DGetContext() == nullptr)
        {
          D3DGetContext() = DirectSheep::RenderContext::Allocate();
          DirectSheep::RenderContext* d3d = D3DGetContext();

          canvas->context = this;

          bool res = d3d->Initialize(canvas->GetHWND(), canvas->GetSize().GetX(), canvas->GetSize().GetY());

          if (!res)
          {
            log("DirectX failed to initialize!");
          }
        }
#endif
    }

    DirectSheep::RenderContext* WxD3DContext::GetRenderer()
    {
      return D3DGetContext();
    }

    //-----------------------------------//

}