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


#ifndef DICOMSEL_MAINFRAME_H
#define DICOMSEL_MAINFRAME_H

// wxWidgets
//#ifndef WX_PRECOMP
# include <wx/frame.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
#include <wx/event.h>

//#endif // !WX_PRECOMP

// Current module
#include <dicomsel/TagSet.h>
#include <dicomsel/StoreDialog.h>
#include <dicomsel/Store.h>

class wxMenuItem;
class wxPanel;
class wxStaticBoxSizer;
class wxFlexGridSizer;
class wxStaticText;
class wxCloseEvent;
class wxCommandEvent;
class wxTreeEvent;


namespace dicomsel
{

class DicomTree;
class BitmapPanel;
class Store;

//const wxEventType EVT_GAME_NEW = wxNewEventType(); 


class MainFrame : public wxFrame
{

public:
    MainFrame( const wxString& title = wxT( "DicomSel" ),
	       const wxPoint& pos = wxDefaultPosition,
	       const wxSize& size = wxDefaultSize );
    virtual ~MainFrame( void );
	
    void SetDirPath(char *Dir);

private:
    wxMenuItem*       m_closeMenu, * m_exportMenu, * m_sendMenu, * m_recoverMenu;
    DicomTree*        m_tree;
    wxPanel*          m_panel;
    BitmapPanel*      m_bitmap;
    wxStaticBoxSizer* m_infoSizer;
    wxFlexGridSizer*  m_grid;
    wxString          m_dirPath;
    wxString	      Address;
    wxString	      PortRcp;
    wxString	      PortSend;
    bool	      Check;
    TagSet m_displayedTags;
    TagSet m_exportedTags;
    struct
    {
	wxStaticText* name;
	wxStaticText* value;
    }
    m_labels[TagSet::TAG__LAST];

    Store *serverStore;



    // No copy/affectation
    MainFrame( const MainFrame& );
    MainFrame& operator =( const MainFrame& );

    void LoadConfig( void );
    void SaveConfig( void ) const;

    void RefreshDisplayedTags( void );
    void RefreshLabels( void );

    void OnClose            ( wxCloseEvent&   event );
    void OnMenuOpenDir      ( wxCommandEvent& event );
    void OnMenuCloseDir     ( wxCommandEvent& event );
    void OnMenuExport       ( wxCommandEvent& event );
    void OnMenuSend         ( wxCommandEvent& event );
    void OnMenuQuit         ( wxCommandEvent& event );
    void OnMenuDisplayedTags( wxCommandEvent& event );
    void OnMenuExportedTags ( wxCommandEvent& event );
    void OnMenuAbout        ( wxCommandEvent& event );
    void OnSelectionChanged ( wxTreeEvent&    event );
    void OnMenuRecoverFile  ( wxCommandEvent& event );
    void OnThreadStart	    ( wxCommandEvent &event );
    void OnThreadFinished   ( wxCommandEvent &event ); 


    DECLARE_EVENT_TABLE()
};

} // namespace dicomsel

#endif // DICOMSEL_MAINFRAME_H

/* End of File */
