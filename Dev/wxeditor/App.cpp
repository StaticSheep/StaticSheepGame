/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/


#include "pch/precompiled.h"
#include "API.h"

#include "App.h"
#include "Entity.h"

#include "WxWindow.h"
#include "WxInputBackend.h"

#include "d3d/WxD3DCanvas.h"
#include "d3d/WxD3DContext.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "SheepGraphics/Context.h"
#include "config/Config.h"
#include "systems/graphics/SheepGraphics.h"

#include <wx/wx.h>

#include "AntTweakBar.h"


namespace fs = boost::filesystem;

namespace dit {

  void* EDITOR_WINDOW = nullptr;
  void* EDITOR_APP = nullptr;
  InputBackend* EDITOR_INPUT = nullptr;

  

#pragma region EditorWindow

  //-----------------------------------//

  void EditorWindow::OnGameWindowResize(wxSizeEvent& event)
  {
    wxSize canvasSize = gameWindowPanel->GetParent()->GetSize();
    realWindow->canvas->SetSize(canvasSize);
    realWindow->canvas->SetPosition(gameWindowPanel->GetParent()->GetPosition() + gameWindowPanel->GetPosition());
    realWindow->MakeCurrent();

    Framework::GRAPHICS->_ScreenWidth = canvasSize.GetX();
    Framework::GRAPHICS->_ScreenHeight = canvasSize.GetY();
  }

  //-----------------------------------//

  typedef WXLRESULT(*wp)(void);

  WXLRESULT OverrideWindProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam, std::function<WXLRESULT()>* fn)
  {
    /*int a = 10;
    WXLRESULT dRes = (*fn)();

    if (TwEventWin(((EditorWindow*)dit::EDITOR_WINDOW)->gameWindowPanel->GetHWND(), message, wParam, lParam))
    return 0;

    return dRes;*/

  #if USE_ANTTWEAKBAR 
    //if (TwEventWin(((EditorWindow*)dit::EDITOR_WINDOW)->gameWindowPanel->GetHWND(), message, wParam, lParam))
    //  return 0; // Event has been handled by AntTweakBar
  #endif
    
    //return (*fn)();
    //return AppWindow::MSWWindowProc(message, wParam, lParam);
    return 0;
  }

#pragma endregion

#pragma region wxApp overrides

    //-----------------------------------//

    bool App::OnInit()
    {
        // init wx
        if (!wxApp::OnInit())
            return false;

        wxInitAllImageHandlers();
        
        EDITOR_APP = this;

        Init();

        return true;
    }

    //-----------------------------------//

    void App::CleanUp()
    {
        Shutdown();
    }

    //-----------------------------------//

    int32 App::FilterEvent(wxEvent& event)
    {
        if (input == nullptr)
            return -1;

        // keyboard events
        auto type = event.GetEventType();
        if (type == wxEVT_CHAR)
        {
            input->ProcessCharEvent((wxKeyEvent&) event);
            return 1;
        }
        else if (type == wxEVT_KEY_DOWN)
        {
            input->ProcessKeyEvent((wxKeyEvent&) event, true);
            return 1;
        }
        else if (type == wxEVT_KEY_UP)
        {
            input->ProcessKeyEvent((wxKeyEvent&) event, false);
            return 1;
        }

        // mouse events
        static std::array<wxEventType, 13> mouseEvents{ {
            wxEVT_LEFT_DOWN
            , wxEVT_LEFT_UP
            , wxEVT_MIDDLE_DOWN
            , wxEVT_MIDDLE_UP
            , wxEVT_RIGHT_DOWN
            , wxEVT_RIGHT_UP
            , wxEVT_MOTION
            , wxEVT_LEFT_DCLICK
            , wxEVT_MIDDLE_DCLICK
            , wxEVT_RIGHT_DCLICK
            , wxEVT_LEAVE_WINDOW
            , wxEVT_ENTER_WINDOW
            , wxEVT_MOUSEWHEEL } };

        for (auto meType : mouseEvents)
        {
            if (type == meType)
            {
                input->ProcessMouseEvent((wxMouseEvent&) event);
                return -1; // still want to propogate the mouse event to the gui
            }
        }

        return -1;
    }

    //-----------------------------------//

#pragma endregion

#pragma region Common interface

    //-----------------------------------//

    void App::Init()
    {
        // create stuff
        core = new Core{};
        input = new WxInputBackend{};

        EDITOR_INPUT = input;

        // initialize all the things
        core->Init();

        Framework::AllocateEngine();

        InitGfx();
        InitEvents();

        Framework::InitEngine();

        // get the mouse pos
        auto& ms = input->mouse.state;

        // show it
        window->Show(true);
    }

    //-----------------------------------//

    void App::InitGfx()
    {
      // create the window
      auto editor = new EditorWindow{ nullptr, input };
      int32 width = 0, height = 0;
      editor->GetClientSize(&width, &height);
      auto gwp = editor->gameWindowPanel;

      EDITOR_WINDOW = editor;

      //wxAuiToolBarItem* item = new wxAuiToolBarItem();
      
      //item->SetLabel(wxString("TEST"));

      //editor->m_tool3->Assign(*(editor->m_tool2));

      window = new WxWindow({ (uint16)width, (uint16)height, String{ editor->GetTitle().c_str() }, editor }, editor, input);
      
      editor->realWindow = window;

      wxPoint windowPos = editor->gameWindowPanel->GetParent()->GetPosition() + editor->gameWindowPanel->GetPosition();
      window->canvas = new WxD3DCanvas(window, gwp, -1, nullptr, windowPos, gwp->GetSize(), gwp->GetWindowStyle());
      window->canvas->Reparent(editor);

      WxD3DContext* wxd3dCtx = (WxD3DContext*)window->CreateContext();
      window->MakeCurrent();

      //window->canvas->customWinProc = OverrideWindProc;

      wxSize canvasSize = gwp->GetSize();

      Framework::GRAPHICS->m_renderContext = (DirectSheep::RenderContext*)wxd3dCtx->GetRenderContext();
      Framework::GRAPHICS->_ScreenWidth = canvasSize.GetX();
      Framework::GRAPHICS->_ScreenHeight = canvasSize.GetY();

      // connect to wx events
      window->onUpdateEvent.push_back([&]() {
          Update();
      });

      window->onDrawEvent.push_back([&]() {
          Draw();
      });

      
    }

    //-----------------------------------//

    void App::InitEvents()
    {
        //-----------------------------------//
        // app events

    


        //-----------------------------------//
        // input

        // hook up mouse input
        Mouse::MouseDragConsumer onMouseDrag = [&](const MouseDragEvent& e) {
            OnMouseDrag(e);
        };
        Mouse::MouseWheelConsumer onMouseWheel = [&](const MouseWheelEvent& e) {
            OnMouseWheel(e);
        };

        input->mouse.onMouseDrag.push_back(onMouseDrag);
        input->mouse.onMouseWheelMove.push_back(onMouseWheel);

        // hook up the keyboard stuff
        Keyboard::KeyEventConsumer onKeys = [&](const KeyEvent&) {
            OnKeys();
        };
        
        input->keyboard.onKeyPress.push_back(onKeys);
    }

    //-----------------------------------//

    void App::Shutdown()
    {
      Framework::ENGINE->Shutdown();

      // shutdown / clean up
      core->Shutdown();

      // destroy resources
      delete input;
      delete window;
      delete core;

      input = nullptr;
      window = nullptr;
      core = nullptr;
    }

    //-----------------------------------//

    void App::Update()
    {
        // clear last frame's filewatch events
        watchEvents.clear();

        // main loop
        Framework::ENGINE->Step();

        // pump events from other threads
        core->Update();
    }

    //-----------------------------------//

    void App::OnKeys()
    {
        auto& kb = input->keyboard;

        // drop out on escape
        if (kb.IsKeyDown(Keys::Escape))
            window->wxw->Close();

        
    }

    //-----------------------------------//

    void App::OnMouseDrag(const MouseDragEvent& e)
    {
        
    }

    //-----------------------------------//

    void App::OnMouseWheel(const MouseWheelEvent& e)
    {
        
    }

    //-----------------------------------//

    void App::Draw()
    {
      //((DirectSheep::RenderContext*)window->ctx->GetRenderContext())
      
    }

    

    //-----------------------------------//

    void App::OnFileWatch(WatchID watchid, const String& dir, const String& filename, Action action)
    {
        // static helper data
        static std::array<String, 3> actionNames{ { "ADDED", "DELETED", "MODIFIED" } };

        // format event message
        String msg;
        mini::format(msg, "file watch - %0 \"%1%2\"", actionNames[(uint32) action].c_str(), dir.c_str(), filename.c_str());

        // prevent double-dispatched events
        auto eventHash = MurmurHash64(msg.c_str(), msg.size(), 0);
        if (watchEvents.find(eventHash) != watchEvents.end())
            return;

        watchEvents.insert(eventHash);
        log(msg.c_str());

        // determine if this file needs to be reloaded
        if (action == Actions::Modified)
        {
            fs::path file{ dir + filename };
        }
    }

    //-----------------------------------//

#pragma endregion
}

/*
void MyFrame::OnDropDownToolbarItem(wxAuiToolBarEvent& evt)
{
if (evt.IsDropDownClicked())
{
wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());

tb->SetToolSticky(evt.GetId(), true);

// create the popup menu
wxMenu menuPopup;

wxBitmap bmp = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16, 16));

wxMenuItem* m1 = new wxMenuItem(&menuPopup, 10001, _("Drop Down Item 1"));
m1->SetBitmap(bmp);
menuPopup.Append(m1);

wxMenuItem* m2 = new wxMenuItem(&menuPopup, 10002, _("Drop Down Item 2"));
m2->SetBitmap(bmp);
menuPopup.Append(m2);

wxMenuItem* m3 = new wxMenuItem(&menuPopup, 10003, _("Drop Down Item 3"));
m3->SetBitmap(bmp);
menuPopup.Append(m3);

wxMenuItem* m4 = new wxMenuItem(&menuPopup, 10004, _("Drop Down Item 4"));
m4->SetBitmap(bmp);
menuPopup.Append(m4);

// line up our menu with the button
wxRect rect = tb->GetToolRect(evt.GetId());
wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
pt = ScreenToClient(pt);


PopupMenu(&menuPopup, pt);


// make sure the button is "un-stuck"
tb->SetToolSticky(evt.GetId(), false);
}
}*/