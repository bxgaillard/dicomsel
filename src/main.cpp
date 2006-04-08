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
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
    std::setlocale(LC_ALL, "C");

    MainFrame* frame = new MainFrame( wxT( "DicomSel" ), wxDefaultPosition,
				      wxSize( 600, 450 ) );
    frame->Show();

    return true;
}

} // namespace dicomsel

IMPLEMENT_APP( dicomsel::DicomSelApp )

/* End of File */
