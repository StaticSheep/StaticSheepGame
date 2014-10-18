///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "API.h"

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
	propGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	propGrid->SetMinSize( wxSize( 150,-1 ) );
	
	modelFile = propGrid->Append( new wxStringProperty( wxT("file"), wxT("file") ) ); 
	triCount = propGrid->Append( new wxUIntProperty( wxT("tris"), wxT("tris") ) ); 
	vertCount = propGrid->Append( new wxUIntProperty( wxT("verts"), wxT("verts") ) ); 
	PropGridSizer->Add( propGrid, 0, wxALL, 5 );
	
	coordFrameCheckbox = new wxCheckBox( this, wxID_ANY, wxT("coord frame"), wxDefaultPosition, wxDefaultSize, 0 );
	coordFrameCheckbox->SetValue(true); 
	PropGridSizer->Add( coordFrameCheckbox, 0, wxALL, 5 );
	
	drawAabbCheckbox = new wxCheckBox( this, wxID_ANY, wxT("draw aabb"), wxDefaultPosition, wxDefaultSize, 0 );
	PropGridSizer->Add( drawAabbCheckbox, 0, wxALL, 5 );
	
	drawBsCheckbox = new wxCheckBox( this, wxID_ANY, wxT("draw bs"), wxDefaultPosition, wxDefaultSize, 0 );
	PropGridSizer->Add( drawBsCheckbox, 0, wxALL, 5 );
	
	wxString bsMethodListboxChoices[] = { wxT("aabb"), wxT("centroid"), wxT("ritters"), wxT("larsen") };
	int bsMethodListboxNChoices = sizeof( bsMethodListboxChoices ) / sizeof( wxString );
	bsMethodListbox = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, bsMethodListboxNChoices, bsMethodListboxChoices, 0 );
	bsMethodListbox->SetSelection( 0 );
	PropGridSizer->Add( bsMethodListbox, 0, wxALL, 5 );
	
	
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
	propGrid->Connect( wxEVT_PG_CHANGING, wxPropertyGridEventHandler( AppWindow::OnPropGridChanging ), NULL, this );
	coordFrameCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagCoordFrame ), NULL, this );
	drawAabbCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnCheckDrawAABB ), NULL, this );
	drawBsCheckbox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnCheckDrawBS ), NULL, this );
	bsMethodListbox->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppWindow::OnSelectBSMethod ), NULL, this );
}

AppWindow::~AppWindow()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnOpenFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppWindow::OnExit ) );
	propGrid->Disconnect( wxEVT_PG_CHANGING, wxPropertyGridEventHandler( AppWindow::OnPropGridChanging ), NULL, this );
	coordFrameCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnFlagCoordFrame ), NULL, this );
	drawAabbCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnCheckDrawAABB ), NULL, this );
	drawBsCheckbox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppWindow::OnCheckDrawBS ), NULL, this );
	bsMethodListbox->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppWindow::OnSelectBSMethod ), NULL, this );
	
}
