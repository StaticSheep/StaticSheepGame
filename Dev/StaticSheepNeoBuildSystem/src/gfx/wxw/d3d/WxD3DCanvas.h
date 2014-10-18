/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"

#include <wx/wx.h>
#include <wx/window.h>

#define wxD3DCanvasName wxT("D3DCanvas")

namespace dit {

    //-----------------------------------//

    class WxWindow;
    class WxInputBackend;
    class WxD3DContext;

    //-----------------------------------//

    // ----------------------------------------------------------------------------
    // wxD3DCanvas: window which can be used for D3D rendering
    // ----------------------------------------------------------------------------

    class WxD3DCanvas : public wxWindow
    {
    public:
      WxD3DCanvas(WxWindow* window
            , wxWindow* parent
            , wxWindowID id = wxID_ANY
            , const int* attribList = nullptr
            , const wxPoint& pos = wxDefaultPosition
            , const wxSize& size = wxDefaultSize
            , long style = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE
            , const wxString& name = wxD3DCanvasName);

      ~WxD3DCanvas();

    public:
        void OnUpdate(wxTimerEvent& event);
        void OnRender(wxTimerEvent& event);
        void OnPaint(wxPaintEvent& event);

    public:
        WxWindow* window = nullptr;
        wxTimer updateTimer;
        wxTimer renderTimer;

        WxD3DContext* context = nullptr;

        float maxUpdateHz = 1.f / 60.f;
        float maxRenderHz = 1.f / 60.f;

        DECLARE_EVENT_TABLE()
    };

    //-----------------------------------//
}