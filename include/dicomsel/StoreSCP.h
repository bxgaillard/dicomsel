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


#ifndef DICOMSEL_STORESCP_H
#define DICOMSEL_STORESCP_H

#include <wx/thread.h>
#include <wx/string.h>

class wxEvtHandler;

namespace dicomsel
{

DECLARE_EVENT_TYPE( EVT_FILE_RECEIVED, )

class StoreSCP : public wxThreadHelper
{
public:
    StoreSCP( const wxString& directory, const unsigned short port,
	      const bool time_name, const bool change_dir,
	      wxEvtHandler* const evtHandler );
    virtual ~StoreSCP( void );

    bool Run( void );
    bool Stop( void );
    bool IsRunning( void );
    void OnAcknowledgeRelease( void );

protected:
    virtual wxThread::ExitCode Entry( void );

private:
    wxString       m_directory;
    unsigned short m_port;
    bool           m_tname, m_chdir;
    wxEvtHandler*  m_evtHandler;
    int            m_lastCount;
};

} // namespace dicomsel

#endif // !DICOMSEL_STORESCP_H

/* End of File */
