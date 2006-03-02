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


#ifndef DICOMSEL_DICOMTREE_H
#define DICOMSEL_DICOMTREE_H

// wxWidgets
#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/gdicmn.h>
# include <wx/window.h>
# include <wx/validate.h>
# include <wx/treectrl.h>
#endif // !WX_PRECOMP

// Current module
#include <dicomsel/DicomCollection.h>

class wxImage;
class wxBitmap;
class wxProgressDialog;

namespace dicomsel
{

class DicomFile;

class DicomTree : public wxTreeCtrl, public DicomCollection::Visitor
{
public:
    DicomTree( wxWindow* parent, wxWindowID id,
	       const wxPoint& pos = wxDefaultPosition,
	       const wxSize& size = wxDefaultSize,
	       long style = wxSUNKEN_BORDER | wxTR_HAS_BUTTONS,
	       const wxValidator& validator = wxDefaultValidator,
	       const wxString& name = wxT( "DicomTree" ) );
    virtual ~DicomTree( void );

    bool OpenDirectory( const wxString& name );
    void CloseDirectory( void );
    wxImage GetImage( void );
    wxBitmap GetBitmap( void );
    const DicomFile& GetFile( void ) { return *m_currentFile; };

protected:
    virtual int OnCompareItems( const wxTreeItemId& item1,
				const wxTreeItemId& item2 );

    virtual void ParsingDirectory( void );
    virtual bool BeginParsing( int total );
    virtual void AddFile( const wxString& patient, const wxString& study,
			  const wxString& series, const wxString& file );
    virtual bool ProgressParsing( void );
    virtual void AbortParsing( void );
    virtual void EndParsing( void );

private:
    class Files;
    template< class Type > class TreeMap;

    TreeMap< TreeMap< TreeMap< Files > > >* m_files;
    DicomFile* m_currentFile;
    int m_total, m_progress;
    wxProgressDialog* m_progressDialog;

    // No copy/affectation
    DicomTree( const DicomTree& );
    DicomTree& operator =( const DicomTree& );

    void OnSelectionChanged( wxTreeEvent& event );

    DECLARE_EVENT_TABLE()
};

} // namespace dicomsel

#endif // DICOMSEL_DICOMTREE_H

/* End of File */
