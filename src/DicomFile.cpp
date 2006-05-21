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
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/bitmap.h>

// C standard library
#include <cstdlib>

#include <iostream>

// Current module
#include <dicomsel/DicomTree.h>
#include <dicomsel/TagSet.h>
#include <dicomsel/DicomFile.h>


namespace dicomsel
{

DicomFile::DicomFile( const wxString& filename )
:   m_filename( filename ),
    m_loaded  ( false ),
    m_valid   ( true ),
    m_tags    ( NULL ),
    m_size    ( wxDefaultSize ),
    m_frame   ( NULL )
{}

DicomFile::~DicomFile( void )
{
    FreeFrame();
    if( m_tags  != NULL ) delete[] m_tags;
}

const wxString& DicomFile::GetTagString( TagSet::TagID tag ) const
{
    static const wxString emptyString;

    if( tag >= TagSet::TAG__FIRST && tag < TagSet::TAG__LAST )
	return m_tags[tag];
    return emptyString;
}

wxImage DicomFile::GetImage( void )
{
    if( !m_loaded && !Read() ) return wxNullImage;

    // We use malloc() and not new, as required by the wxImage constructor
    unsigned char* const data = reinterpret_cast< unsigned char* >(
	std::malloc( m_size.GetWidth() * m_size.GetHeight() * 3 ) );
    if( data == NULL ) return wxNullImage;

    switch( m_format )
    {
	case FF_U8:
	    ConvertData< unsigned char >(
		data, reinterpret_cast< unsigned char* >( m_frame ) );
	    break;

	case FF_S8:
	    ConvertData< signed char >(
		data, reinterpret_cast< signed char* >( m_frame ) );
	    break;

	case FF_U16:
	    ConvertData< unsigned short >(
		data, reinterpret_cast< unsigned short* >( m_frame ) );
	    break;

	case FF_S16:
	    ConvertData< signed short >(
		data, reinterpret_cast< signed short* >( m_frame ) );
	    break;
    }

    return wxImage( m_size.GetWidth(), m_size.GetHeight(), data );
}

wxBitmap DicomFile::GetBitmap( void )
{
    return wxBitmap( GetImage() );
}

bool DicomFile::Read( void )
{
    if( m_loaded ) return m_valid;

    m_tags = new wxString[TagSet::TAG__LAST + 1];

    m_valid = ReadFile( m_filename ) &&
	      m_size.IsFullySpecified() && m_frame != NULL;
    m_loaded = true;

    return m_valid;
}

void DicomFile::Free( void )
{
    if( m_loaded )
    {
	m_loaded = false;
	m_valid  = true;
	m_size   = wxDefaultSize;

	FreeFrame();
	if( m_tags != NULL )
	{
	    delete[] m_tags;
	    m_tags = NULL;
	}
    }
}

template< class Type >
void DicomFile::ConvertData( unsigned char* data, const Type* frame ) const
{
    const int total = m_size.GetWidth() * m_size.GetHeight();
    Type min, max;

    min = max = *frame;

    for( int i = 0; i < total; i++ )
    {
	if( frame[i] < min ) min = frame[i];
	if( frame[i] > max ) max = frame[i];
    }

    const unsigned int diff = static_cast< unsigned int >( max - min );
    if( diff != 0 )
    {
	unsigned int val;

	for( int i = 0; i < total; i++ )
	{
	    for( int j = 0; j < 3; j++ )
	    {
		val = static_cast< unsigned int >( *frame - min );
		*(data++) = static_cast< unsigned char >(
		    ((val << 8) - val) / diff );
	    }
	    frame++;
	}
    }
}

} // namespace dicomsel

/* End of File */
