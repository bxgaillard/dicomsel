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

// wxWidgets
//#ifndef WX_PRECOMP
# include <wx/app.h>
# include <wx/gdicmn.h>
//#endif // !WX_PRECOMP

// Current module
#include <dicomsel/MainFrame.h>

namespace dicomsel
{

class DicomSelApp : public wxApp
{
public:
    virtual bool OnInit( void );
};


bool DicomSelApp::OnInit( void )
{
    MainFrame* frame = new MainFrame( wxT( "DicomSel" ), wxDefaultPosition,
				      wxSize( 600, 450 ) );
    frame->Show();

    return true;
}

} // namespace dicomsel

IMPLEMENT_APP( dicomsel::DicomSelApp )

/* End of File */
