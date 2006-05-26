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

// STL
#include <string>

// C standard library
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <sys/types.h>

// Global
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

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

// OpenJPEG
#undef OPENJPEG_VERSION
#include <openjpeg.h>

// Current module
#include <dicomsel/DicomTree.h>
#include <dicomsel/TagSet.h>
#include <dicomsel/DicomFile.h>
#include <dicomsel/LibDicomFile.h>


namespace dicomsel
{

LibDicomFile::LibDicomFile( const wxString& filename )
:   DicomFile( filename ),
    m_bitsPerPixel( 0 ),
    m_unsigned( true ),
    m_littleEndian( true )
{}

LibDicomFile::~LibDicomFile( void )
{}

DicomTree::Library LibDicomFile::GetLibrary( void )
{
    return DicomTree::LIB_LIBDICOM;
}

bool LibDicomFile::ReadFile( const wxString& filename )
{
    m_valid = true, m_frame = NULL;

    try
    {
	m_pFile = new dicom::io::CDicomFile(
	    std::string( filename.fn_str() ) );
	m_pFile->LoadInputFile();

	// Locale modification is needed because strtod(), which is used in
	// libdicom, depends on the current locale; this causes decimal part
	// of the numbers to be ignored if "." isn't used as a decimal point.
	// The problem is the same with printf() where something other than a
	// "." may be issued for floating point numbers.
	std::setlocale( LC_NUMERIC, "C" );
	ParseFile( *m_pFile );
	std::setlocale( LC_NUMERIC, "" );
    }
    catch( dicom::exception::CBase& )
    {
	m_valid = false;
    }

    if( m_frame != NULL )
    {
	if( m_valid )
	{
	    if( m_bitsPerPixel == 16 &&
		m_pFile->IsLittleEndian() != (BYTE_ORDER == LITTLE_ENDIAN) )
	    {
		const int size = m_size.GetHeight() * m_size.GetWidth()
			       * (m_bitsPerPixel / 8);
		char tmp;

		for( int i = 0; i < size; i += 2 )
		{
		    tmp = m_frame[i];
		    m_frame[i] = m_frame[i + 1];
		    m_frame[i + 1] = tmp;
		}
	    }

	    const FrameFormat format = m_bitsPerPixel == 8 ?
		(m_unsigned ? FF_U8  : FF_S8 ) :
		(m_unsigned ? FF_U16 : FF_S16);

	    SetFrame( m_frame, format );
	}
	else
	{
	    delete[] m_frame;
	}
    }

    delete m_pFile;
    return m_valid;
}

const bool LibDicomFile::IsCreateImage( void ) const
{
    return true;
}

void LibDicomFile::VisitDicomDirectory( dicom::io::CSimpleDirectory&
				     WXUNUSED( dir ) )
{}

void LibDicomFile::VisitDicomFile( dicom::io::CDicomFile& WXUNUSED( file ) )
{}

const uint32 LibDicomFile::VisitDicomBitsAllocatedTag(
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

const uint32 LibDicomFile::VisitDicomPixelRepresentation(
    dicom::tag::CPixelRepresentation& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    m_unsigned = tag.isUnsignedFlag();
    m_tags[TagSet::TAG_PIXEL_REPRESENTATION]
	= m_unsigned ? wxT( "Unsigned" ) : wxT( "Signed" );

    return pos;
}

const uint32 LibDicomFile::VisitDicomColumnsTag( dicom::tag::CColumns& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    const int width = tag.GetWidth();
    if( m_size.GetWidth() < 0 )           m_size.SetWidth( width );
    else if( m_size.GetWidth() != width ) m_valid = false;

    m_tags[TagSet::TAG_COLUMNS].Printf( wxT( "%d" ), width );

    return pos;
}

const uint32 LibDicomFile::VisitDicomRowsTag( dicom::tag::CRows& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );

    const int height = tag.GetHeight();
    if( m_size.GetHeight() < 0 )            m_size.SetHeight( height );
    else if( m_size.GetHeight() != height ) m_valid = false;

    m_tags[TagSet::TAG_ROWS].Printf( wxT( "%d" ), height );

    return pos;
}

#if 0
extern "C" {
/**
sample error callback expecting a FILE* client object
 */
void error_callback(const char *msg, void *client_data) {
    FILE *stream = (FILE*)client_data;
    fprintf(stream, "[ERROR] %s", msg);
}
/**
sample warning callback expecting a FILE* client object
 */
void warning_callback(const char *msg, void *client_data) {
    FILE *stream = (FILE*)client_data;
    fprintf(stream, "[WARNING] %s", msg);
}
/**
sample debug callback expecting no client object
 */
void info_callback(const char *msg, void *client_data) {
    fprintf(stdout, "[INFO] %s", msg);
}
}
#endif

const uint32 LibDicomFile::VisitDicomPixelDataTag(
    dicom::tag::CPixelData& tag )
{
    const bool canRead = m_size.IsFullySpecified() && m_bitsPerPixel != 0;
    uint32 pos;

    if( canRead )
    {
	int size = tag.GetLength();
	if( size < 0 )
	{
	    size = m_pFile->GetSize() - m_pFile->GetOffset();
	}
	tag.SetSize( size );
	pos = tag.SetValue( *this->m_pFile );

	if( m_frame == NULL )
        {
	    const wxString& syntax = m_tags[TagSet::TAG_TRANSFERT_SYNTAX];

	    if( syntax == "1.2.840.10008.1.2.4.90" ||
		syntax == "1.2.840.10008.1.2.4.91" )
	    {
		opj_dinfo_t* const dinfo = opj_create_decompress( CODEC_J2K );

		/*
		opj_event_mgr_t event_mgr;
		std::memset( &event_mgr, 0, sizeof( opj_event_mgr_t ) );
		event_mgr.error_handler = error_callback;
		event_mgr.warning_handler = warning_callback;
		event_mgr.info_handler = info_callback;
		opj_set_event_mgr( reinterpret_cast< opj_common_ptr >( dinfo ),
				   &event_mgr, stderr );
		*/

		opj_dparameters_t parameters;
		opj_set_default_decoder_parameters( &parameters );
		opj_setup_decoder( dinfo, &parameters );
		opj_cio_t* const cio =
			opj_cio_open( reinterpret_cast< opj_common_ptr >( dinfo ),
				      reinterpret_cast< unsigned char* >( tag.GetBuffer()) ,
				      size );

		opj_image_t* const image = opj_decode( dinfo, cio );

		if( image != NULL )
		{
		    opj_cio_close(cio);
		    delete[] tag.GetBuffer();
		    m_frame = new char[m_size.GetHeight() * m_size.GetWidth()
			    * (m_bitsPerPixel / 8)];
		    std::cout << "valid!!" << std::endl;
		}
		else
		{
		    opj_destroy_decompress(dinfo);
		    opj_cio_close(cio);
		    delete[] tag.GetBuffer();

		    m_valid = false;
		}
	    }
	    else
	    {
		if( size == m_size.GetHeight() * m_size.GetWidth()
			    * (m_bitsPerPixel / 8) )
		{
		    m_frame = tag.GetBuffer();
		}
		else m_valid = false;
	    }
        }
	else m_valid = false;
    }
    else pos = tag.SetValue( *this->m_pFile );

    // Unused: m_tags[TagSet::TAG_PIXEL_DATA] = wxT( "<Binary Data>" );

    return pos;
}

const uint32 LibDicomFile::VisitDicomImagePositionTag(
    dicom::tag::CImagePosition& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_IMAGE_POSITION].Printf( wxT( "%.10g %.10g %.10g" ),
					       tag.GetPositionX(),
					       tag.GetPositionY(),
					       tag.GetPositionZ() );
    return pos;
}

const uint32 LibDicomFile::VisitDicomPixelSpacingTag(
    dicom::tag::CPixelSpacing& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_PIXEL_SPACING].Printf( wxT( "%.10g %.10g" ),
					      tag.GetSpacingWidth(),
					      tag.GetSpacingHeight() );
    return pos;
}

const uint32 LibDicomFile::VisitDicomSliceThicknessTag(
    dicom::tag::CSliceThickness& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_tags[TagSet::TAG_SLICE_THICKNESS].Printf( wxT( "%.10g" ),
						tag.GetThickness() );
    return pos;
}

// Macro for generating default-behaving string tag handlers
#define STRING_TAG_VISIT( name, suffix, id, function ) \
    const uint32 LibDicomFile::VisitDicom ## name ## suffix(          \
	dicom::tag::C ## name& tag )                                  \
    {                                                                 \
	const uint32 pos = tag.SetValue( *m_pFile );                  \
	m_tags[TagSet::id] = wxString( tag.Get ## function().c_str(), \
				       wxConvISO8859_1 );             \
	return pos;                                                   \
    }

// Macro for generating default-behaving integer tag handlers
#define INTEGER_TAG_VISIT( name, id, function ) \
    const uint32 LibDicomFile::VisitDicom ## name ## Tag(  \
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

} // namespace dicomsel

/* End of File */
