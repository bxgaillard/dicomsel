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


// wxWidgets
#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
//#ifndef WX_PRECOMP
# include <wx/dialog.h>
# include <wx/window.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
# include <wx/sizer.h>
# include <wx/statbox.h>
# include <wx/checkbox.h>
//#endif

// Current module
#include <dicomsel/TagSet.h>
#include <dicomsel/TagDialog.h>


namespace dicomsel
{

TagDialog::TagDialog( wxWindow* parent, wxWindowID id, const wxString& title )
:   wxDialog( parent, id, title, wxDefaultPosition, wxDefaultSize,
	      wxDEFAULT_DIALOG_STYLE, wxT( "TagDialogBox" ) )
{
    wxBoxSizer* const sizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( sizer );

    wxStaticBox* const box =
	new wxStaticBox( this, -1, wxT( "Select Tags" ) );
    wxStaticBoxSizer* const boxSizer =
	new wxStaticBoxSizer( box, wxVERTICAL );
    sizer->Add( boxSizer, 1, wxALL | wxEXPAND, 4 );

    const int count = TagSet::GetTagCount();
    wxFlexGridSizer* const grid =
	new wxFlexGridSizer( (count + 9) / 10, 4, 16 );
    boxSizer->Add( grid );

    for( int i = 0; i < count; i++ )
    {
	wxCheckBox* const checkBox =
	    new wxCheckBox( this, -1,
			    TagSet::GetTagName(
				static_cast< TagSet::TagID >( i ) ) );
	m_checkBoxes[i] = checkBox;
	grid->Add( checkBox, 0, wxEXPAND );
    }

    sizer->Add( CreateStdDialogButtonSizer( wxOK | wxCANCEL ), 0,
		wxALL ^ wxTOP | wxEXPAND, 4 );
    Fit();
}

TagDialog::~TagDialog( void )
{}

void TagDialog::SelectTag( TagSet::TagID tag, bool selected ) const
{
    if( tag >= 0 && tag < TagSet::TAG__LAST )
    {
	m_checkBoxes[tag]->SetValue( selected );
    }
}

void TagDialog::UnselectTag( TagSet::TagID tag ) const
{
    SelectTag( tag, false );
}

bool TagDialog::IsTagSelected( TagSet::TagID tag ) const
{
    return (tag >= 0 && tag < TagSet::TAG__LAST) ?
	m_checkBoxes[tag]->GetValue() : false;
}

void TagDialog::SetTags( const TagSet& tags )
{
    for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; i++ )
    {
	m_checkBoxes[i]->SetValue(
	    tags.IsIn( static_cast< TagSet::TagID >( i ) ) );
    }
}

void TagDialog::GetTags( TagSet& tags ) const
{
    tags.Clear();
    for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; i++ )
    {
	if( m_checkBoxes[i]->GetValue() )
	{
	    tags.Add( static_cast< TagSet::TagID >( i ) );
	}
    }
}

} // namespace dicomsel

/* End of File */
