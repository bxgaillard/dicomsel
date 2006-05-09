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


#ifndef DICOMSEL_DICOMFILE_H
#define DICOMSEL_DICOMFILE_H

// wxWidgets
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/treectrl.h>
#include <wx/image.h>
#include <wx/bitmap.h>

// Current module
#include <dicomsel/DicomTree.h>
#include <dicomsel/TagSet.h>


class wxImage;
class wxBitmap;


namespace dicomsel
{

class DicomFile : public wxTreeItemData
{
public:
    DicomFile( const wxString& filename );
    virtual ~DicomFile( void ) = 0;

    virtual DicomTree::Library GetLibrary( void ) = 0;

    const wxString& GetFilename( void ) const { return m_filename; }
    const wxSize&   GetSize( void )     const { return m_size; }
    const wxString& GetTagString( TagSet::TagID tag ) const;
    wxImage GetImage( void );
    wxBitmap GetBitmap( void );

    bool Read( void );
    void Free( void );

protected:
    enum FrameFormat { FF_U8, FF_S8, FF_U16, FF_S16 };

    void SetTagString( const TagSet::TagID tag, const wxString& str )
    {
	if( tag >= TagSet::TAG__FIRST && tag < TagSet::TAG__LAST )
	{
	    m_tags[tag] = str;
	}
    }

    void SetSize( const int width, const int height )
    {
	m_size.Set( width, height );
    }

    void SetFrame( char* const frame, const FrameFormat format )
    {
	FreeFrame();
	m_frame = frame;
	m_format = format;
    }

    virtual bool ReadFile( const wxString& filename ) = 0;

private:
    wxString    m_filename;
    bool        m_loaded;
    protected:
    bool        m_valid;
    wxString*   m_tags;
    wxSize      m_size;
    private:
    FrameFormat m_format;
    char*       m_frame;

    void FreeFrame( void )
    {
	if( m_frame != NULL ) delete[] m_frame;
	m_frame = NULL;
    }

    template< class Type >
    void ConvertData( unsigned char* data, const Type* frame ) const;
};

} // namespace dicomsel

#endif // DICOMSEL_DICOMFILE_H

/* End of File */
