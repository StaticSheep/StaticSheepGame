/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "WxD3DCanvas.h"
#include "WxWindow.h"
#include "WxInputBackend.h"

namespace dit {

    //-----------------------------------//

    enum class Timers : uint32
    {
        Update = wxID_HIGHEST + 321
        , Render
    };

    BEGIN_EVENT_TABLE(WxD3DCanvas, wxWindow)
        EVT_TIMER((uint32) Timers::Update, WxD3DCanvas::OnUpdate)
        EVT_TIMER((uint32)Timers::Render, WxD3DCanvas::OnRender)
        EVT_PAINT(WxD3DCanvas::OnPaint)
    END_EVENT_TABLE()

    //-----------------------------------//

    /*
    wxWindow(wxWindow *parent,
    wxWindowID id,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = 0,
    const wxString& name = wxPanelNameStr)
    */

    WxD3DCanvas::WxD3DCanvas(WxWindow* window_, wxWindow* parent, wxWindowID id, const int* attribList, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
        : wxWindow(parent, id, pos, size, style | (wxFULL_REPAINT_ON_RESIZE & (~wxCLIP_CHILDREN)), name)
        , window(window_)
        , updateTimer(this, (uint32)Timers::Update)
        , renderTimer(this, (uint32)Timers::Render)
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);

        updateTimer.Start(maxUpdateHz);
        renderTimer.Start(maxRenderHz);

        
    }

    //-----------------------------------//

    WxD3DCanvas::~WxD3DCanvas()
    {
        renderTimer.Stop();
        updateTimer.Stop();
    }

    //-----------------------------------//

    void WxD3DCanvas::OnUpdate(wxTimerEvent& event)
    {
        if (window)
            window->HandleUpdate();
    }

    //-----------------------------------//
    
    void WxD3DCanvas::OnRender(wxTimerEvent& event)
    {
        Refresh();
    }

    //-----------------------------------//

    void WxD3DCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        // From the PaintEvent docs: "the application must always create
        // a wxPaintDC object, even if you do not use it."
        // http://docs.wxwidgets.org/trunk/classwx_paint_event.html
        wxPaintDC dc(this);

        if (window)
            window->HandleDraw();

        //SwapBuffers();
    }

    //-----------------------------------//

}