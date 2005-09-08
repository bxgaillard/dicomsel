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


#include <wx/wxprec.h>

#ifdef __BORLANDC__
# pragma hdrstop
#endif

#ifndef WX_PRECOMP
# include <wx/app.h>
# include <wx/frame.h>
#endif

#include "MainFrame.h"

namespace radiofrequency
{

class RadiofrequencyApp : public wxApp
{
public:
    virtual bool OnInit( void );
};

bool RadiofrequencyApp::OnInit( void )
{
    (new MainFrame( wxT( "Radiofrequency" ) ))->Show();
    return true;
}

} // namespace dicomsel

IMPLEMENT_APP( radiofrequency::RadiofrequencyApp )

/* End of File */
