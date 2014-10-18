/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/


#include "precompiled.h"
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


namespace fs = boost::filesystem;

namespace dit {

#pragma region EditorWindow

  //-----------------------------------//

  void EditorWindow::OnOpenFile(wxCommandEvent& event)
  {
    wxFileDialog ofd{
    this
    , "Open XYZ file"
    , "", ""
    , "OBJ files (*.obj)|*.obj"
    , wxFD_OPEN | wxFD_FILE_MUST_EXIST };

    auto res = ofd.ShowModal();
    if (res == wxID_CANCEL)
      return;

    String fileName{ ofd.GetPath().c_str() };
  }

  //-----------------------------------//

  void EditorWindow::OnExit(wxCommandEvent& event)
  {
    this->Close();
  }

  //-----------------------------------//

  void EditorWindow::OnPropGridChanging(wxPropertyGridEvent& event)
  {
    event.Veto();
  }

  //-----------------------------------//

  void EditorWindow::OnFlagSolidMesh(wxCommandEvent& event)
  {
    //send(core()->root->events, "toggle-flag", App::Flags::SolidMesh);
  }

  //-----------------------------------//

  void EditorWindow::OnFlagDrawHoles(wxCommandEvent& event)
  {
    //send(core()->root->events, "toggle-flag", App::Flags::DrawHoles);
  }

  //-----------------------------------//

  void EditorWindow::OnFlagCoordFrame(wxCommandEvent& event)
  {
    //send(core()->root->events, "toggle-flag", App::Flags::CoordFrame);
  }

  //-----------------------------------//

#pragma endregion

#pragma region wxApp overrides

    //-----------------------------------//

    bool App::OnInit()
    {
        // init wx
        if (!wxApp::OnInit())
            return false;

        wxInitAllImageHandlers();

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

        // initialize all the things
        core->Init();
        InitGfx();
        InitEvents();

        // get the mouse pos
        auto& ms = input->mouse.state;
        lastMousePos = float2{ ms.x, ms.y };

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

      window = new WxWindow({ (uint16)width, (uint16)height, String{ editor->GetTitle().c_str() }, editor }, editor, input);

      window->canvas = new WxD3DCanvas(window, gwp, -1, nullptr, gwp->GetPosition(), gwp->GetSize(), gwp->GetWindowStyle());
      window->canvas->Reparent(editor);

      window->CreateContext();
      window->MakeCurrent();

      // extern pointer to renderer context


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
      window->canvas->context->GetRenderer()->ClearBackBuffer();
      window->canvas->context->GetRenderer()->ClearDepthBuffer();



      window->canvas->context->GetRenderer()->Present();

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