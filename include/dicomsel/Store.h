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


#ifndef DICOMSEL_STORE_H
#define DICOMSEL_STORE_H

#include <wx/thread.h>
#include <wx/timer.h>
#include <wx/string.h>


namespace dicomsel
{

class MainFrame;

class Store: public wxThreadHelper, wxEvtHandler
{
private:
    bool        m_blnCancelled;
    int         Type;
    wxString    Address, Port, FileName;
    wxTimer*    m_tmrRunning;
    unsigned int Port_num;
    MainFrame *Frame;

public:
    Store(wxString Address_,wxString Port_,wxString FileName_, int type_ );
    virtual ~Store( void );

    void                       Run();
    virtual wxThread::ExitCode Entry();
    bool        IsRunning();
    void        OnTimer(wxTimerEvent &event);

protected:
    DECLARE_EVENT_TABLE()
};

} // namespace dicomsel

#endif // !DICOMSEL_STORE_H

/* End of File */
