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


#ifndef RADIOFREQUENCY_MAINFRAME_H
#define RADIOFREQUENCY_MAINFRAME_H

// wxWidgets
#ifndef WX_PRECOMP
# include <wx/frame.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
# include <wx/ipc.h>
#endif // !WX_PRECOMP

namespace radiofrequency
{

class MainFrame : public wxFrame, public wxServer
{
public:
    MainFrame( const wxString& title = wxT( "Radiofrequency" ),
	       const wxPoint& pos = wxDefaultPosition,
	       const wxSize& size = wxDefaultSize );
    virtual ~MainFrame( void );

    virtual wxConnectionBase* OnAcceptConnection( const wxString& topic );

private:
    class Connection;

    void OnClose   ( wxCloseEvent&   event );
    void OnBtnClose( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()
};

} // namespace radiofrequency

#endif // RADIOFREQUENCY_MAINFRAME_H

/* End of File */
