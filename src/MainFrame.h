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


#ifndef DICOMSEL_MAINFRAME_H
#define DICOMSEL_MAINFRAME_H

#ifndef WX_PRECOMP
# include <wx/frame.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
#endif

#include "TagSet.h"

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

class MainFrame : public wxFrame
{
public:
    MainFrame( const wxString& title = wxT( "DicomSel" ),
	       const wxPoint& pos = wxDefaultPosition,
	       const wxSize& size = wxDefaultSize );
    virtual ~MainFrame( void );

    static const wxString UniString( const wchar_t* const str )
    {
	return wxString( str, *wxConvCurrent );
    }

private:
    wxMenuItem*       m_closeMenu;
    wxMenuItem*       m_exportMenu;
    DicomTree*        m_tree;
    wxPanel*          m_panel;
    BitmapPanel*      m_bitmap;
    wxStaticBoxSizer* m_infoSizer;
    wxFlexGridSizer*  m_grid;
    wxString          m_dirPath;

    TagSet m_displayedTags;
    TagSet m_exportedTags;
    struct
    {
	wxStaticText* name;
	wxStaticText* value;
    }
    m_labels[TagSet::TAG__LAST];

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
    void OnMenuQuit         ( wxCommandEvent& event );
    void OnMenuDisplayedTags( wxCommandEvent& event );
    void OnMenuExportedTags ( wxCommandEvent& event );
    void OnMenuAbout        ( wxCommandEvent& event );
    void OnSelectionChanged ( wxTreeEvent&    event );

    DECLARE_EVENT_TABLE()
};

} // namespace dicomsel

#endif // DICOMSEL_MAINFRAME_H

/* End of File */
