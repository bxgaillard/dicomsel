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


#include <wx/event.h>
#include <wx/string.h>
#include <wx/timer.h>
#include <wx/msgdlg.h>

#include <dicomsel/MainFrame.h>
#include <dicomsel/Store.h>

#include <dicomsel/StoreSCP.h>
#include <dicomsel/StoreSCU.h>
bool m_blnFinished;

namespace dicomsel
{

static bool send_ok;
static const char send_error_msg[] = "";
#define main_storescp server

#define server main_storescp
#define send main_storescu

BEGIN_EVENT_TABLE(Store, wxEvtHandler)
    EVT_TIMER(wxID_ANY,         Store::OnTimer)
END_EVENT_TABLE()

// constructor for receive
Store::Store( wxString WXUNUSED( Port ), MainFrame* WXUNUSED( frame_ ),
	      int WXUNUSED( type_ ), bool WXUNUSED( timeFile_ ) )
{
    abort();
}


//constructor for send
Store::Store(wxString Address_,wxString Port_,wxString FileName_,int type_)
{
    Type=type_;
    Address=(char*)malloc(Address_.Len());
    strcpy( Address, (const char*)Address_.mb_str(wxConvUTF8) );
    Port=(char*)malloc(Port_.Len());
    strcpy( Port, (const char*)Port_.mb_str(wxConvUTF8) );
    FileName=(char*)malloc(FileName_.Len());
    strcpy( FileName, (const char*)FileName_.mb_str(wxConvUTF8) );
    m_tmrRunning = new wxTimer(this);
}



wxThread::ExitCode Store::Entry()
{
    char* argv[] = { "storescu", Address, Port, FileName };
    send_ok = StoreSCU::Main( sizeof( argv ) / sizeof( *argv ), argv )
	    == 0;
    m_blnFinished = true;

    return reinterpret_cast< wxThread::ExitCode >( 0 );
}

void Store::Run()
{
    if( !IsRunning() )
    {
	m_blnFinished = false;      // flag we are not done
	m_tmrRunning->Start( 100 ); // start timer
	wxThreadHelper::Create();   // create thread
	GetThread()->Run();         // run...
    }
}

bool Store::IsRunning()
{
    const wxThread* const currentThread = wxThreadHelper::GetThread();
    return currentThread != NULL && currentThread->IsRunning();
}

void Store::OnTimer( wxTimerEvent& WXUNUSED( event ) )
{
    if( m_blnFinished )
    {
	if( send_ok )
	{
	    m_tmrRunning->Stop();
	    m_blnFinished = false;
	    wxMessageBox( wxT( "File sent." ), wxT( "DICOM Store" ) );
	}
	else
	{
	    m_tmrRunning->Stop();
	    wxMessageBox( wxString( send_error_msg, *wxConvCurrent ),
			    wxT( "DICOM Store Error" ) );
	}
    }
}

} // namespace dicomsel

/* End of File */
