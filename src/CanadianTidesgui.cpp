///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CanadianTidesgui.h"

///////////////////////////////////////////////////////////////////////////

DlgDef::DlgDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerDateTime;
	sbSizerDateTime = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Important") ), wxHORIZONTAL );

	m_stTimeZone = new wxStaticText( sbSizerDateTime->GetStaticBox(), wxID_ANY, _("All times UTC"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_stTimeZone->Wrap( -1 );
	m_stTimeZone->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	m_stTimeZone->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );

	sbSizerDateTime->Add( m_stTimeZone, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( sbSizerDateTime, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Days ahead: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );

	bSizer5->Add( m_staticText9, 0, wxALL, 5 );

	wxString m_choice3Choices[] = { _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7") };
	int m_choice3NChoices = sizeof( m_choice3Choices ) / sizeof( wxString );
	m_choice3 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice3NChoices, m_choice3Choices, 0 );
	m_choice3->SetSelection( 0 );
	bSizer5->Add( m_choice3, 0, wxALL, 5 );


	bSizerMain->Add( bSizer5, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerFolder;
	sbSizerFolder = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Stations") ), wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText91 = new wxStaticText( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Region:       "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	m_staticText91->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );

	bSizer3->Add( m_staticText91, 0, wxALL, 5 );

	wxString m_choice31Choices[] = { _("PAC"), _("CNA"), _("ATL"), _("QUE") };
	int m_choice31NChoices = sizeof( m_choice31Choices ) / sizeof( wxString );
	m_choice31 = new wxChoice( sbSizerFolder->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice31NChoices, m_choice31Choices, 0 );
	m_choice31->SetSelection( 0 );
	m_choice31->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	bSizer3->Add( m_choice31, 0, wxALL, 5 );


	sbSizerFolder->Add( bSizer3, 1, wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( sbSizerFolder->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sbSizerFolder->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_buttonDownload = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _(" Download  "), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerFolder->Add( m_buttonDownload, 0, wxALL|wxEXPAND, 5 );

	m_stUKDownloadInfo = new wxStaticText( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Status:   Standby"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stUKDownloadInfo->Wrap( -1 );
	m_stUKDownloadInfo->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );

	sbSizerFolder->Add( m_stUKDownloadInfo, 0, wxALL|wxEXPAND, 5 );

	m_buttonSaved = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Show Saved"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerFolder->Add( m_buttonSaved, 0, wxALL|wxEXPAND, 5 );

	m_buttonSaved1 = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Remove Icons \n OTHER THAN saved"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerFolder->Add( m_buttonSaved1, 0, wxALL|wxEXPAND, 5 );

	m_buttonSaved11 = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Remove ALL Icons"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerFolder->Add( m_buttonSaved11, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( sbSizerFolder, 0, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgDef::OnClose ) );
	m_buttonDownload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnDownload ), NULL, this );
	m_buttonSaved->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnGetSavedTides ), NULL, this );
	m_buttonSaved1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::DoRemovePortIcons ), NULL, this );
	m_buttonSaved11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::DoRemoveAllPortIcons ), NULL, this );
}

DlgDef::~DlgDef()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgDef::OnClose ) );
	m_buttonDownload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnDownload ), NULL, this );
	m_buttonSaved->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnGetSavedTides ), NULL, this );
	m_buttonSaved1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::DoRemovePortIcons ), NULL, this );
	m_buttonSaved11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::DoRemoveAllPortIcons ), NULL, this );

}
