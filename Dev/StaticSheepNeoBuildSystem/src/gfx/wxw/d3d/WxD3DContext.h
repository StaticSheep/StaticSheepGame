/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "GfxContext.h"

//-----------------------------------//
// Forward decls

class wxObject;
class wxWindow;

namespace DirectSheep
{
  class RenderContext;
}


//-----------------------------------//

namespace dit {

    //-----------------------------------//
    // Forward decls

    class WxWindow;
    class WxD3DCanvas;

    //-----------------------------------//

    class WxD3DContext : public GfxContext
    {
    public:

        WxD3DContext(WxD3DCanvas* canvas, WxWindow* window);

        DirectSheep::RenderContext* GetRenderer();

        virtual ~WxD3DContext();

        void MakeCurrent();

    public:
    };

    //-----------------------------------//
}