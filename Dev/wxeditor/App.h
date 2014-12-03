/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "Event.h"
#include "Core.h"


#include "window/AppWindow.h"

#include <wx/wx.h>
#include <boost/filesystem/path.hpp>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "wx/propgrid/propgrid.h"
#include "WxWindow.h"

//-----------------------------------//

namespace dit {

  
  //-----------------------------------//
  // Forward decls

  struct MouseMoveEvent;
  struct MouseDragEvent;
  struct MouseWheelEvent;

  class WxInputBackend;

  //-----------------------------------//

  
  extern void* EDITOR_WINDOW;
  extern void* EDITOR_APP;
  extern InputBackend* EDITOR_INPUT;

  class EditorWindow : public AppWindow
  {
  public:
    EditorWindow(wxWindow* parent = nullptr, WxInputBackend* input = nullptr)
          : AppWindow(parent)
      {}

  public:
      virtual void OnGameWindowResize(wxSizeEvent& event);
      virtual void OnNewSpace(wxCommandEvent& event);

      //virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

  public:
      WxInputBackend* input = nullptr;
      WxWindow* realWindow = nullptr;
  };

  //-----------------------------------//

  class App : public wxApp
  {
  public:
      // wx overrides
      virtual bool OnInit();
      virtual void CleanUp();
      virtual int32 FilterEvent(wxEvent& event);

  public:
      // common interface
      void Init();
      void Shutdown();
      void Update();
      void Draw();

  private:
      // helpers
      void InitGfx();
      void InitEvents();

        

      void OnMouseDrag(const MouseDragEvent& event);
      void OnMouseWheel(const MouseWheelEvent& event);
      void OnKeys();
      void OnFileWatch(WatchID watchid, const String& dir, const String& filename, Action action);

  public:
      // data
      Core*      core = nullptr;
      WxWindow*  window = nullptr;
      WxInputBackend* input = nullptr;
      EventMap appEvents;
      std::array<bool, 4> flags;

  private:
      std::unordered_set<uint64> watchEvents;
  };

  //-----------------------------------//

  DECLARE_APP(App)

  //-----------------------------------//
}