///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "pch/precompiled.h"

#include "AppWindow.h"

#include "res/control_play_blue.png.h"
#include "res/control_stop_blue.png.h"

///////////////////////////////////////////////////////////////////////////
using namespace dit;

AppWindow::AppWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	gizmoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( gizmoPanel, wxAuiPaneInfo() .Left() .Caption( wxT("Gizmo") ).Dock().Resizable().FloatingSize( wxSize( 216,58 ) ).DockFixed( false ).BottomDockable( false ).TopDockable( false ).Layer( 2 ) );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	
	gizmoPanel->SetSizer( bSizer10 );
	gizmoPanel->Layout();
	gamePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( gamePanel, wxAuiPaneInfo() .Center() .Caption( wxT("Game") ).CloseButton( false ).Dock().Resizable().FloatingSize( wxSize( 816,58 ) ).DockFixed( false ).BottomDockable( false ).TopDockable( false ).Floatable( false ).Row( 1 ).Layer( 2 ) );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	gameWindowPanel = new wxPanel( gamePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer8->Add( gameWindowPanel, 1, wxEXPAND, 5 );
	
	
	gamePanel->SetSizer( bSizer8 );
	gamePanel->Layout();
	spacePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( spacePanel, wxAuiPaneInfo() .Left() .Caption( wxT("Spaces") ).Dock().Resizable().FloatingSize( wxSize( 41,58 ) ).DockFixed( false ).BottomDockable( false ).TopDockable( false ).Layer( 2 ) );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( spacePanel, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer9->Add( m_staticText3, 0, wxALL, 5 );
	
	
	spacePanel->SetSizer( bSizer9 );
	spacePanel->Layout();
	bSizer9->Fit( spacePanel );
	objectsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( objectsPanel, wxAuiPaneInfo() .Right() .Caption( wxT("Objects") ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).BottomDockable( false ).TopDockable( false ) );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	
	objectsPanel->SetSizer( bSizer11 );
	objectsPanel->Layout();
	propertiesPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( propertiesPanel, wxAuiPaneInfo() .Right() .Caption( wxT("Properties") ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).BottomDockable( false ).TopDockable( false ) );
	
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	
	propertiesPanel->SetSizer( bSizer12 );
	propertiesPanel->Layout();
	menuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* newSpaceBtn;
	newSpaceBtn = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Create Space") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( newSpaceBtn );
	
	menuBar->Append( fileMenu, wxT("File") ); 
	
	this->SetMenuBar( menuBar );
	
	m_auiToolBar1 = new wxAuiToolBar( this, wxID_ANY, wxPoint( -1,-1 ), wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_tool1 = m_auiToolBar1->AddTool( wxID_ANY, wxT("tool"), control_play_blue_png_to_wx_bitmap(), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool2 = m_auiToolBar1->AddTool( wxID_ANY, wxT("tool"), control_stop_blue_png_to_wx_bitmap(), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_auiToolBar1->Realize();
	m_mgr.AddPane( m_auiToolBar1, wxAuiPaneInfo().Top().CaptionVisible( false ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxSize( 74,54 ) ).BottomDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).Row( 1 ).Layer( 10 ) );
	
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	gamePanel->Connect( wxEVT_SIZE, wxSizeEventHandler( AppWindow::OnGameWindowResize ), NULL, this );
	this->Connect( newSpaceBtn->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnNewSpace ) );
}

AppWindow::~AppWindow()
{
	// Disconnect Events
	gamePanel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( AppWindow::OnGameWindowResize ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnNewSpace ) );
	
	m_mgr.UnInit();
	
}
