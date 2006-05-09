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


// wxWidgets
#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
#include <wx/app.h>
#include <wx/gdicmn.h>

// Standard C Library
#include <cstdio>
#include <clocale>

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
    // Prevent DCMTK code from outputting error messages
    std::fclose( stderr );

    // Make default dialog buttons display in English
#ifdef LC_MESSAGES
    std::setlocale( LC_MESSAGES, "C" );
#endif // LC_MESSAGES

    MainFrame* frame = new MainFrame( wxT( "DicomSel" ), wxDefaultPosition,
				      wxSize( 600, 450 ) );
    frame->Show();

    return true;
}

} // namespace dicomsel

IMPLEMENT_APP( dicomsel::DicomSelApp )

/* End of File */
