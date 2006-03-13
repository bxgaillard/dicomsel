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


#ifndef DICOMSEL_TAGDIALOG_H
#define DICOMSEL_TAGDIALOG_H

// wxWidgets
//#ifndef WX_PRECOMP
# include <wx/dialog.h>
# include <wx/window.h>
//#endif // !WX_PRECOMP

// Current module
#include <dicomsel/TagSet.h>

class wxString;
class wxCheckBox;

namespace dicomsel
{

class TagDialog : public wxDialog
{
public:
    TagDialog( wxWindow* parent, wxWindowID id, const wxString& title );
    virtual ~TagDialog( void );

    void SelectTag    ( TagSet::TagID tag, bool selected = true ) const;
    void UnselectTag  ( TagSet::TagID tag ) const;
    bool IsTagSelected( TagSet::TagID tag ) const;

    void SetTags( const TagSet& tags );
    void GetTags(       TagSet& tags ) const;

private:
    wxCheckBox* m_checkBoxes[TagSet::TAG__LAST];

    // No copy/affectation
    TagDialog( const TagDialog& );
    TagDialog& operator =( const TagDialog& );
};

} // namespace dicomsel

#endif // DICOMSEL_TAGDIALOG_H

/* End of File */
