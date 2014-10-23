///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __APPWINDOW_H__
#define __APPWINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/panel.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace dit
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class AppWindow
	///////////////////////////////////////////////////////////////////////////////
	class AppWindow : public wxFrame 
	{
		private:
		
		protected:
			wxPanel* gizmoPanel;
			wxPanel* spacePanel;
			wxStaticText* m_staticText3;
			wxPanel* objectsPanel;
			wxPanel* propertiesPanel;
			wxMenuBar* menuBar;
			wxMenu* fileMenu;
			wxAuiToolBar* m_auiToolBar1;
			wxAuiToolBarItem* m_tool1; 
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnGameWindowResize( wxSizeEvent& event ) { event.Skip(); }
			virtual void OnNewSpace( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			wxPanel* gamePanel;
			wxPanel* gameWindowPanel;
			wxBoxSizer* bSizer12;
			wxAuiToolBarItem* m_tool2; 
			
			AppWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sheep Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxDEFAULT_FRAME_STYLE|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL|wxWANTS_CHARS );
			wxAuiManager m_mgr;
			
			~AppWindow();
		
	};
	
} // namespace dit

#endif //__APPWINDOW_H__
