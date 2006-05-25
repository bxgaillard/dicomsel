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


#include <cstdio>
#include <cstring>
#include <time.h>
#include <wx/string.h>
#include <wx/buffer.h>
#include <wx/arrstr.h>
#include <wx/event.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <dicomsel/dcmtk.h>
#include <dcmtk/dcmnet/assoc.h>

#include <dicomsel/StoreSCP.h>


static dicomsel::StoreSCP* ptrStoreSCP;

static OFCondition
storescp_ASC_acknowledgeRelease( T_ASC_Association* association )
{
    const OFCondition cond = ASC_acknowledgeRelease( association );
    ptrStoreSCP->OnAcknowledgeRelease();
    return cond;
}

#define ASC_acknowledgeRelease storescp_ASC_acknowledgeRelease
#define opt_acse_timeout       storescp_opt_acse_timeout
#define opt_dimse_timeout      storescp_opt_dimse_timeout
#define opt_blockMode          storescp_opt_blockMode
#define main                   storescp_main

static int main( int argc, char *argv[] );

#ifdef _MSC_VER
# pragma warning( disable: 4267 4100 )
#endif // _MSC_VER
#include "dcmtkapp/storescp.cc"
#ifdef _MSC_VER
# pragma warning( default: 4267 4100 )
#endif // _MSC_VER

#undef ASC_acknowledgeRelease
#undef opt_acse_timeout
#undef opt_dimse_timeout
#undef opt_blockMode
#undef main

#if defined( _WIN32 ) && (defined( _MSC_VER ) || defined( __MINGW32__ ))
namespace std { using ::_snprintf; }
# define snprintf _snprintf
#endif // _WIN32 && (_MSC_VER || __MINGW32__)

namespace dicomsel
{

DEFINE_EVENT_TYPE( EVT_FILE_RECEIVED )

StoreSCP::StoreSCP( const wxString& directory, const unsigned short port,bool time_name,bool change_dir,
		    wxEvtHandler* const evtHandler )
:    m_port( port ), m_tname(time_name),m_chdir(change_dir),m_evtHandler( evtHandler )
{



    const wxCharBuffer direct = directory.fn_str();
    m_directory = new char[std::strlen( direct ) + 1 +8];
    std::strcpy(m_directory, direct);

}




StoreSCP::~StoreSCP( void )
{
    Stop();
}

bool StoreSCP::Run( void )
{
    if( GetThread() == NULL && wxThreadHelper::Create() != wxTHREAD_NO_ERROR )
    {
	return false;
    }
    return GetThread()->Run() == wxTHREAD_NO_ERROR;
}

bool StoreSCP::Stop( void )
{

    wxThread* const thread = GetThread();
    return thread != NULL && thread->IsAlive() &&
	   thread->Kill() == wxTHREAD_NO_ERROR;

}

bool StoreSCP::IsRunning( void )
{
    const wxThread* const thread = GetThread();
    return thread != NULL && thread->IsAlive();
}

void StoreSCP::OnAcknowledgeRelease( void )
{
    OFListIterator( OFString ) last = outputFileNameArray.end();
    const int count =
	    static_cast< int >( outputFileNameArray.size() ) - m_lastCount;
    wxArrayString array;
    array.Alloc( count );

    for( int i = count - 1; i >= 0; --i )
    {
	array[i] = wxString( (*--last).c_str(), *wxConvFileName );
    }

    m_lastCount = static_cast< int >( outputFileNameArray.size() );

    // TODO: do something useful here
    for( int i = 0; i < count; ++i )
    {
	std::puts( array[i].fn_str() );
    }

    wxCommandEvent event( EVT_FILE_RECEIVED );
    m_evtHandler->AddPendingEvent( event );
}

wxThread::ExitCode StoreSCP::Entry( void )
{
    m_lastCount = 0;
    ptrStoreSCP = this;

    char port[6];
    std::snprintf( port, sizeof( port ) / sizeof( *port ),
		   "%u", static_cast< unsigned int >( m_port ) );

    char* options = new char[10];
    if (m_tname) std::strcpy( options,"-tn");
    else  std::strcpy( options,"-uf");

    if(m_chdir) udir=true;
    else udir=false;


    char *argv[] = { "storescp","-od", m_directory,options, port };


    const int code = storescp_main( sizeof( argv ) / sizeof( *argv ), argv );

    // Cast to unsigned long long makes MSVC happier
    return reinterpret_cast< wxThread::ExitCode >
	    ( static_cast< unsigned long long >( code ) );
}
 

} // namespace dicomsel

/* End of File */
