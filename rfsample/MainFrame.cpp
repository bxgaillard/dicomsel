/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005, 2006 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENSE >>
 *
 * ---------------------------------------------------------------------------
 */


#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
//#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/gdicmn.h>
# include <wx/event.h>
# include <wx/panel.h>
# include <wx/sizer.h>
# include <wx/stattext.h>
# include <wx/button.h>
# include <wx/ipc.h>
//#endif

#include <cstdio>
#include <clocale>

#include <dicomsel/ipc.h>
#include "MainFrame.h"

namespace radiofrequency
{

class MainFrame::Connection : public wxConnection
{
public:
    Connection( MainFrame& frame ) : m_frame( frame ) {}
    virtual bool OnPoke( const wxString& topic, const wxString& item,
			 wxChar* data, int size, wxIPCFormat format );

private:
    MainFrame& m_frame;
};

bool MainFrame::Connection::OnPoke( const wxString& topic,
				    const wxString& item,
				    wxChar* data, int size,
				    wxIPCFormat format )
{
    if( format == wxIPC_TEXT && size > 0 &&
	topic == wxT( IPC_TOPIC ) && item == wxT( IPC_ITEM ) )
    {
	char* const charData = reinterpret_cast< char* >( data );
	charData[size - 1] = '\0';
	double x, y, z;

	std::setlocale( LC_NUMERIC, "C" ); // See ../src/DicomFile.cpp
	const int read = std::sscanf( charData, "%lf%lf%lf", &x, &y, &z );
	std::setlocale( LC_NUMERIC, "" );

	if( read == 3 )
	{
	    m_frame.SetLabel( wxString::Format(
		wxT( "Position: (%.10g; %.10g; %.10g)" ), x, y, z ) );
	    return true;
	}
    }

    return false;
}

enum
{
    ID_PANEL = wxID_HIGHEST + 1,
    ID_TEXT,
    ID_BUTTON = wxID_CLOSE
};

BEGIN_EVENT_TABLE( MainFrame, wxFrame )
    EVT_CLOSE( MainFrame::OnClose )
    EVT_BUTTON( ID_BUTTON, MainFrame::OnBtnClose )
END_EVENT_TABLE()


MainFrame::MainFrame( const wxString& title, const wxPoint& pos,
		      const wxSize& size )
:   wxFrame( NULL, -1, title, pos, size,
	     wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxRESIZE_BORDER )
{
    m_panel = new wxPanel( this, ID_PANEL );

    wxBoxSizer* const sizer = new wxBoxSizer( wxVERTICAL );
    m_panel->SetSizer( sizer );

    m_text = new wxStaticText( m_panel, ID_TEXT,
			       wxT( "No position sent yet." ),
			       wxDefaultPosition, wxDefaultSize,
			       wxALIGN_CENTRE );
    sizer->Add( m_text, 0, wxALL | wxEXPAND, 4 );

    wxButton* const button = new wxButton( m_panel, ID_BUTTON,
					   wxT( "&Close" ) );
    sizer->Add( button, 1, wxALL | wxEXPAND, 4 );

    m_panel->Fit();
    Fit();

#ifdef __WXMSW__
    wxServer::Create( wxT( IPC_SERVICE ) );
#else
    wxServer::Create( wxT( IPC_UNIXSOCKET ) );
#endif
}

MainFrame::~MainFrame( void )
{}

void MainFrame::SetLabel( const wxString& label )
{
    m_text->SetLabel( label );
    m_panel->Fit();
    Fit();
}

wxConnectionBase* MainFrame::OnAcceptConnection( const wxString& topic )
{
    if( topic == wxT( IPC_TOPIC ) ) return new Connection( *this );
    return NULL;
}

void MainFrame::OnClose( wxCloseEvent& event )
{
    event.Skip();
}

void MainFrame::OnBtnClose( wxCommandEvent& WXUNUSED( event ) )
{
    Close();
}

} // namespace radiofrequency

/* End of File */
