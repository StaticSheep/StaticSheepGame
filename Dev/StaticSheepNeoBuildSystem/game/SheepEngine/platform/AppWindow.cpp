///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "precompiled.h"

#include "AppWindow.h"

///////////////////////////////////////////////////////////////////////////
using namespace dit;

AppWindow::AppWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	MenuBar = new wxMenuBar( 0 );
	FileMenu = new wxMenu();
	wxMenuItem* OpenFile;
	OpenFile = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Open file..") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( OpenFile );
	
	wxMenuItem* Exit;
	Exit = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( Exit );
	
	MenuBar->Append( FileMenu, wxT("File") ); 
	
	this->SetMenuBar( MenuBar );
	
	wxBoxSizer* RootSizer;
	RootSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* PropGridSizer;
	PropGridSizer = new wxBoxSizer( wxVERTICAL );
	
	PropGridSizer->SetMinSize( wxSize( 150,-1 ) ); 
	solidMeshCheckbox = new wxCheckBox( this, wxID_ANY, wxT("solid mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	solidMeshCheckbox->SetValue(true); 
	PropGridSizer->Add( solidMeshCheckbox, 0, wxALL, 5 );
	
	drawHolesCheckbox = new wxCheckBox( this, wxID_ANY, wxT("holes in mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	PropGridSizer->Add( drawHolesCheckbox, 0, wxALL, 5 );
	
	triangleStripCheckbox = new wxCheckBox( this, wxID_ANY, wxT("triangle strips"), wxDefaultPosition, wxDefaultSize, 0 );
	triangleStripCheckbox->Enable( false );
	
	PropGridSizer->Add( triangleStripCheckbox, 0, wxALL, 5 );
	
	coordFrameCheckbox = new wxCheckBox( this, wxID_ANY, wxT("coord frame"), wxDefaultPosition, wxDefaultSize, 0 );
	coordFrameCheckbox->SetValue(true); 
	PropGridSizer->Add( coordFrameCheckbox, 0, wxALL, 5 );
	
	
	RootSizer->Add( PropGridSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* GameWindowSizer;
	GameWindowSizer = new wxBoxSizer( wxVERTICAL );
	
	GameWindowSizer->SetMinSize( wxSize( 1280,720 ) ); 
	gameWindowPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	gameWindowPanel->SetMinSize( wxSize( 1280,720 ) );
	gameWindowPanel->SetMaxSize( wxSize( 1280,720 ) );
	
	GameWindowSizer->Add( gameWindowPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	RootSizer->Add( GameWindowSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( RootSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( OpenFile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnOpenFile ) );
	this->Connect( Exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnExit ) );
	solidMeshCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagSolidMesh ), NULL, this );
	drawHolesCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagDrawHoles ), NULL, this );
	coordFrameCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagCoordFrame ), NULL, this );
}

AppWindow::~AppWindow()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnOpenFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnExit ) );
	solidMeshCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagSolidMesh ), NULL, this );
	drawHolesCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagDrawHoles ), NULL, this );
	coordFrameCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagCoordFrame ), NULL, this );
	
}
