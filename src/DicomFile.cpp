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


// wxWidgets
#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/gdicmn.h>
# include <wx/image.h>
# include <wx/bitmap.h>
#endif

// STL
#include <string>

// C standard library
#include <cstdlib>
#include <clocale>
#include <sys/types.h>

// Virtuals basic types
#include <Virtuals/Type.h>

// DICOM I/O
#include <dicom/io/DicomFile.h>

// DICOM tags
#include <dicom/tag/DicomTag.h>
#include <dicom/tag/AcquisitionDate.h>
#include <dicom/tag/AcquisitionNumber.h>
#include <dicom/tag/AcquisitionTime.h>
#include <dicom/tag/AcquisitionZone.h>
#include <dicom/tag/Birthday.h>
#include <dicom/tag/BitsAllocated.h>
#include <dicom/tag/Columns.h>
#include <dicom/tag/Hospital.h>
#include <dicom/tag/ImagePosition.h>
#include <dicom/tag/ImageType.h>
#include <dicom/tag/Modality.h>
#include <dicom/tag/Name.h>
#include <dicom/tag/PatientID.h>
#include <dicom/tag/PixelData.h>
#include <dicom/tag/PixelRepresentation.h>
#include <dicom/tag/PixelSpacing.h>
#include <dicom/tag/Rows.h>
#include <dicom/tag/SeriesUID.h>
#include <dicom/tag/Sexe.h>
#include <dicom/tag/SliceThickness.h>
#include <dicom/tag/StudyUID.h>
#include <dicom/tag/TransfertSyntax.h>

// DICOM parser exceptions
#include <dicom/ExceptionDicom.h>

// Current module
#include "DicomTree.h"
#include "TagSet.h"
#include "DicomFile.h"

namespace dicomsel
{

DicomFile::DicomFile( const ::wxString& filename )
:   m_filename( filename ),
    m_loaded( false ),
    m_valid( true ),
    m_bitsPerPixel( 0 ),
    m_unsigned( true ),
    m_littleEndian( true ),
    m_size( wxDefaultSize ),
    m_frame( NULL ),
    m_tags( NULL )
{}

DicomFile::~DicomFile( void )
{
    if( m_frame != NULL ) delete[] m_frame;
    if( m_tags  != NULL ) delete[] m_tags;
}

wxImage DicomFile::GetImage( void )
{
    if( !m_loaded && !Read() ) return wxNullImage;

    unsigned char* const data = reinterpret_cast< unsigned char* >(
	std::malloc( m_size.GetWidth() * m_size.GetHeight() * 3 ) );
    if( data == NULL ) return wxNullImage;

    if( m_bitsPerPixel == 8 )
    {
	if( m_unsigned )
	{
	    ConvertData< unsigned char >(
		data, reinterpret_cast< unsigned char* >( m_frame ) );
	}
	else
	{
	    ConvertData< signed char >(
		data, reinterpret_cast< signed char* >( m_frame ) );
	}
    }
    else
    {
	if( m_unsigned )
	{
	    ConvertData< unsigned short >(
		data, reinterpret_cast< unsigned short* >( m_frame ) );
	}
	else
	{
	    ConvertData< signed short >(
		data, reinterpret_cast< signed short* >( m_frame ) );
	}
    }

    return wxImage( m_size.GetWidth(), m_size.GetHeight(), data );
}

wxBitmap DicomFile::GetBitmap( void )
{
    return wxBitmap( GetImage() );
}

bool DicomFile::Read( void )
{
    if( m_loaded )
	return m_valid;

    m_tags = new wxString[TagSet::TAG__LAST];

    m_valid = true;
    try
    {
	m_pFile = new dicom::io::CDicomFile(
	    std::string( m_filename.mb_str( *wxConvFileName ) ) );
	m_pFile->LoadInputFile();

	// Locale modification is needed because strtod(), which is used in
	// libdicom, depends on the current locale; this causes decimal part
	// of the numbers to be ignored if "." isn't used as a decimal point.
	// The problem is the same with printf() where something other than a
	// "." may be issued for floating point numbers.
	std::setlocale( LC_NUMERIC, "C" );
	ParseFile( *m_pFile );
	std::setlocale( LC_NUMERIC, "");
    }
    catch( dicom::exception::CBase& )
    {
	m_valid = false;
    }
    delete m_pFile;

    if( m_valid )
    {
	if( m_valid = m_size.IsFullySpecified() )
	{
	    if( m_valid = m_frame != NULL ) m_loaded = true;
	}
    }

    return m_loaded;
}

const wxString& DicomFile::GetTagString( TagSet::TagID tag ) const
{
    return m_tags[(tag >= 0 || tag < TagSet::TAG__LAST) ?
		  tag : TagSet::TAG__LAST];
}

void DicomFile::Free( void )
{
    if( m_loaded )
    {
	m_loaded       = false;
	m_valid        = true;
	m_bitsPerPixel = 0;
	m_size         = wxDefaultSize;

	if( m_frame != NULL )
	{
	    delete[] m_frame;
	    m_frame = NULL;
	}

	if( m_tags != NULL )
	{
	    delete[] m_tags;
	    m_tags = NULL;
	}
    }
}

const bool DicomFile::IsCreateImage( void ) const
{
    return true;
}

void DicomFile::VisitDicomDirectory( dicom::io::CSimpleDirectory&
				     WXUNUSED( dir ) )
{}

void DicomFile::VisitDicomFile( dicom::io::CDicomFile& WXUNUSED( file ) )
{}

const uint32 DicomFile::VisitDicomBitsAllocatedTag(
    dicom::tag::CBitsAllocated& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    const unsigned char bpp = tag.GetBitsPerPixel();
    if( bpp == 8 || bpp == 16 )
    {
	if( m_bitsPerPixel == 0 )        m_bitsPerPixel = bpp;
	else if( m_bitsPerPixel != bpp ) m_valid = false;
    }
    else m_valid = false;

    m_tags[TagSet::TAG_BITS_ALLOCATED].Printf( wxT( "%d" ),
					       static_cast< int >( bpp ) );

    return pos;
}

const uint32 DicomFile::VisitDicomPixelRepresentation(
    dicom::tag::CPixelRepresentation& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    m_unsigned = tag.isUnsignedFlag();
    m_tags[TagSet::TAG_PIXEL_REPRESENTATION]
	= m_unsigned ? wxT( "Unsigned" ) : wxT( "Signed" );

    return pos;
}

const uint32 DicomFile::VisitDicomColumnsTag( dicom::tag::CColumns& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    const int width = tag.GetWidth();
    if( m_size.GetWidth() < 0 )           m_size.SetWidth( width );
    else if( m_size.GetWidth() != width ) m_valid = false;

    m_tags[TagSet::TAG_COLUMNS].Printf( wxT( "%d" ), width );

    return pos;
}

const uint32 DicomFile::VisitDicomRowsTag( dicom::tag::CRows& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    const int height = tag.GetHeight();
    if( m_size.GetHeight() < 0 )            m_size.SetHeight( height );
    else if( m_size.GetHeight() != height ) m_valid = false;

    m_tags[TagSet::TAG_ROWS].Printf( wxT( "%d" ), height );

    return pos;
}

const uint32 DicomFile::VisitDicomPixelDataTag( dicom::tag::CPixelData& tag )
{
    const bool canRead = m_size.IsFullySpecified() && m_bitsPerPixel != 0;
    uint32 pos;

    if( canRead )
    {
	const int size = m_size.GetHeight() * m_size.GetWidth()
			 * (m_bitsPerPixel / 8);
	tag.SetSize( size );
	pos = tag.SetValue( *this->m_pFile );

	if( m_frame == NULL )
	{
	    m_frame = tag.GetBuffer();

	    if( m_bitsPerPixel == 16 &&
		m_pFile->IsLittleEndian() != (BYTE_ORDER == LITTLE_ENDIAN) )
	    {
		char tmp;
		for( int i = 0; i < size; i += 2 )
		{
		    tmp = m_frame[i];
		    m_frame[i] = m_frame[i + 1];
		    m_frame[i + 1] = tmp;
		}
	    }
	}
	else m_valid = false;
    }
    else pos = tag.SetValue( *this->m_pFile );

    // Unused: m_tags[TagSet::TAG_PIXEL_DATA] = wxT( "<Binary Data>" );

    return pos;
}

const uint32 DicomFile::VisitDicomImagePositionTag(
    dicom::tag::CImagePosition& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_IMAGE_POSITION].Printf( wxT( "%.10g %.10g %.10g" ),
					       tag.GetPositionX(),
					       tag.GetPositionY(),
					       tag.GetPositionZ() );
    return pos;
}

const uint32 DicomFile::VisitDicomPixelSpacingTag(
    dicom::tag::CPixelSpacing& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_PIXEL_SPACING].Printf( wxT( "%.10g %.10g" ),
					      tag.GetSpacingWidth(),
					      tag.GetSpacingHeight() );
    return pos;
}

const uint32 DicomFile::VisitDicomSliceThicknessTag(
    dicom::tag::CSliceThickness& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_SLICE_THICKNESS].Printf( wxT( "%.10g" ),
						tag.GetThickness() );
    return pos;
}

// Macro for generating default-behaving string tag handlers
#define STRING_TAG_VISIT( name, suffix, id, function ) \
    const uint32 DicomFile::VisitDicom ## name ## suffix(             \
	dicom::tag::C ## name& tag )                                  \
    {                                                                 \
	const uint32 pos = tag.SetValue( *m_pFile );                  \
	m_tags[TagSet::id] = wxString( tag.Get ## function().c_str(), \
				       *wxConvCurrent );              \
	return pos;                                                   \
    }

// Macro for generating default-behaving integer tag handlers
#define INTEGER_TAG_VISIT( name, id, function ) \
    const uint32 DicomFile::VisitDicom ## name ## Tag(     \
	dicom::tag::C ## name& tag )                       \
    {                                                      \
	const uint32 pos = tag.SetValue( *m_pFile );       \
	m_tags[TagSet::id].Printf( wxT( "%d" ),            \
	    static_cast< int >( tag.Get ## function() ) ); \
	return pos;                                        \
    }

// Some implementation flaws? ;-)
#define STRING_TAG_VISIT1( name, id, function ) \
    STRING_TAG_VISIT( name, Tag, id, function )
#define STRING_TAG_VISIT2( name, id, function ) \
    STRING_TAG_VISIT( name,    , id, function )

// String tag handlers
STRING_TAG_VISIT1( AcquisitionDate, TAG_ACQUISITION_DATE, AcquisitionDate )
STRING_TAG_VISIT1( AcquisitionTime, TAG_ACQUISITION_TIME, AcquisitionTime )
STRING_TAG_VISIT1( AcquisitionZone, TAG_ACQUISITION_ZONE, Zone            )
STRING_TAG_VISIT1( Birthday,        TAG_BIRTHDAY,         BirthdayPatient )
STRING_TAG_VISIT1( Hospital,        TAG_HOSPITAL,         HospitalPatient )
STRING_TAG_VISIT1( ImageType,       TAG_IMAGE_TYPE,       Type            )
STRING_TAG_VISIT1( Modality,        TAG_MODALITY,         ModalityPatient )
STRING_TAG_VISIT1( Name,            TAG_NAME,             NamePatient     )
STRING_TAG_VISIT1( PatientID,       TAG_PATIENT_ID,       ID              )
STRING_TAG_VISIT1( SeriesUID,       TAG_SERIES_UID,       UID             )
STRING_TAG_VISIT1( StudyUID,        TAG_STUDY_UID,        UID             )
STRING_TAG_VISIT2( TransfertSyntax, TAG_TRANSFERT_SYNTAX, TransfertSyntax )

// Integer tag handlers
INTEGER_TAG_VISIT( Sexe,              TAG_SEX,                SexePatient )
INTEGER_TAG_VISIT( AcquisitionNumber, TAG_ACQUISITION_NUMBER, Number      )

template< class Type >
void DicomFile::ConvertData( unsigned char* data, const Type* frame )
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

} // namespace dicomsel

/* End of File */
