#ifdef _WIN32
# undef HAVE_CONFIG_H
#endif // _WIN32

#include <cstdio>
#include <cstring>

#include <wx/string.h>
#include <wx/buffer.h>
#include <wx/arrstr.h>
#include <wx/event.h>

#include <dcmtk/dcmnet/assoc.h>

#include <dicomsel/StoreSCP.h>

static dicomsel::StoreSCP* ptrStoreSCP;

static OFCondition
storescp_ASC_acknowledgeRelease( T_ASC_Association* association )
{
    const OFCondition cond = ASC_acknowledgeRelease( association );
    ptrStoreSCP->OnAcknowledgeRelease();
}

#define ASC_acknowledgeRelease storescp_ASC_acknowledgeRelease
#define opt_acse_timeout       storescp_opt_acse_timeout
#define opt_dimse_timeout      storescp_opt_dimse_timeout
#define opt_blockMode          storescp_opt_blockMode
#define main                   storescp_main

static int main( int argc, char *argv[] );
#include "dcmtkapp/storescp.cc"

#undef ASC_acknowledgeRelease
#undef opt_acse_timeout
#undef opt_dimse_timeout
#undef opt_blockMode
#undef main

namespace dicomsel
{

DEFINE_EVENT_TYPE( EVT_FILE_RECEIVED )

StoreSCP::StoreSCP( const wxString& directory, const unsigned short port,
		    wxEvtHandler* const evtHandler )
:   m_directory( directory ), m_port( port ), m_evtHandler( evtHandler )
{}

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
    const int count = outputFileNameArray.size() - m_lastCount;
    wxArrayString array;
    array.Alloc( count );

    for( int i = count - 1; i >= 0; --i )
    {
	array[i] = wxString( (*--last).c_str(), *wxConvFileName );
    }

    m_lastCount = outputFileNameArray.size();

    // TODO: do something useful here
    for( unsigned int i = 0; i < array.GetCount(); ++i )
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

    const wxCharBuffer directory = m_directory.fn_str();
    char* const dir = new char[std::strlen( directory ) + 1];
    std::strcpy( dir, directory );

    char port[6];
    std::snprintf( port, sizeof( port ) / sizeof( *port ),
		   "%u", static_cast< unsigned int >( m_port ) );
    char *argv[] = { "storescp", "-od", dir, port };

    const int code = storescp_main( sizeof( argv ) / sizeof( *argv ), argv );
    delete dir;
    return reinterpret_cast< wxThread::ExitCode >( code );
}

} // namespace dicomsel
