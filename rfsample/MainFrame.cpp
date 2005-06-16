/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENCE >>
 *
 * ---------------------------------------------------------------------------
 */


#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/gdicmn.h>
# include <wx/event.h>
# include <wx/button.h>
# include <wx/ipc.h>
# include <wx/msgdlg.h>
#endif

#include <cstdio>

#include "../src/config.h"
#include "MainFrame.h"

namespace radiofrequency
{

class MainFrame::Connection : public wxConnection
{
public:
    virtual bool OnPoke( const wxString& topic, const wxString& item,
			 char* data, int size, wxIPCFormat format );
};

bool MainFrame::Connection::OnPoke( const wxString& topic,
				    const wxString& item,
				    wxChar* data, int size,
				    wxIPCFormat format )
{
    if( format == wxIPC_TEXT && size > 0 &&
	topic == wxT( IPC_TOPIC ) && item == wxT( IPC_ITEM ) )
    {
	data[size - 1] = wxT( '\0' );
	wxCharBuffer buffer = wxString( data ).mb_str( wxConvUTF8 );
	double x, y, z;
	sscanf(buffer, "%lf%lf%lf", &x, &y, &z);

	wxMessageBox( wxString::Format( wxT( "Position = (%g, %g, %g)" ),
					x, y, z ), wxT( "IPC" ),
		      wxOK | wxICON_INFORMATION, NULL );
	return true;
    }

    return false;
}

enum
{
    BTN_CLOSE = wxID_CLOSE
};

BEGIN_EVENT_TABLE( MainFrame, wxFrame )
    EVT_CLOSE( MainFrame::OnClose )
    EVT_BUTTON( BTN_CLOSE, MainFrame::OnBtnClose )
END_EVENT_TABLE()


MainFrame::MainFrame( const wxString& title, const wxPoint& pos,
		      const wxSize& size )
:   wxFrame( NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE )
{
    new wxButton( this, BTN_CLOSE, wxT( "&Close" ) );
#ifdef __WXMSW__
    wxServer::Create( wxT( IPC_SERVICE ) );
#else
    wxServer::Create( wxT( IPC_UNIXSOCKET ) );
#endif

    Fit();
}

MainFrame::~MainFrame( void )
{}

wxConnectionBase* MainFrame::OnAcceptConnection( const wxString& topic )
{
    if( topic == wxT( IPC_TOPIC ) ) return new Connection;
    return static_cast< wxConnectionBase* >( NULL );
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
