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
//#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/gdicmn.h>
# include <wx/treectrl.h>
# include <wx/image.h>
# include <wx/bitmap.h>
//#endif // !WX_PRECOMP

// Virtuals basic types
#include <Virtuals/Type.h>

// DICOM visitor
#include <dicom/visitor/Dicom.h>

// Current module
#include <dicomsel/TagSet.h>


class wxImage;
class wxBitmap;

namespace dicom
{
    namespace io
    {
	class CSimpleDirectory;
	class CDicomFile;
    } // namespace io

    namespace tag
    {
	class CDicomTag;
	class CAcquisitionDate;
	class CAcquisitionNumber;
	class CAcquisitionTime;
	class CAcquisitionZone;
	class CBirthday;
	class CBitsAllocated;
	class CColumns;
	class CHospital;
	class CImagePosition;
	class CImageType;
	class CModality;
	class CName;
	class CPatientID;
	class CPixelData;
	class CPixelRepresentation;
	class CPixelSpacing;
	class CRows;
	class CSeriesUID;
	class CSexe;
	class CSliceThickness;
	class CStudyUID;
	class CTransfertSyntax;
    } // namespace tag
} // namespace dicom


namespace dicomsel
{

class DicomFile : public wxTreeItemData, public dicom::visitor::CDicom
{
public:
    DicomFile( const wxString& filename );
    virtual ~DicomFile( void );

    const wxString& getFilename( void ) const { return m_filename; }
    const wxSize&   getSize( void )     const { return m_size; }

    wxImage GetImage( void );
    wxBitmap GetBitmap( void );

    const wxString& GetTagString( TagSet::TagID tag ) const;

    bool Read( void );
    void Free( void );

    virtual const bool IsCreateImage( void ) const;
    virtual void VisitDicomDirectory( dicom::io::CSimpleDirectory& dir );
    virtual void VisitDicomFile( dicom::io::CDicomFile& file );

// Macro for generating tag visitors
#define DECLARE_TAG_VISITOR( name, suffix ) \
    virtual const uint32 VisitDicom ## name ## suffix( \
	dicom::tag::C ## name& tag )

// Some implementation flaws? ;-)
#define DECLARE_TAG_VISITOR1( name ) DECLARE_TAG_VISITOR( name, Tag )
#define DECLARE_TAG_VISITOR2( name ) DECLARE_TAG_VISITOR( name,     )

    DECLARE_TAG_VISITOR1( AcquisitionDate     );
    DECLARE_TAG_VISITOR1( AcquisitionNumber   );
    DECLARE_TAG_VISITOR1( AcquisitionTime     );
    DECLARE_TAG_VISITOR1( AcquisitionZone     );
    DECLARE_TAG_VISITOR1( Birthday            );
    DECLARE_TAG_VISITOR1( BitsAllocated       );
    DECLARE_TAG_VISITOR1( Columns             );
    DECLARE_TAG_VISITOR1( Hospital            );
    DECLARE_TAG_VISITOR1( ImagePosition       );
    DECLARE_TAG_VISITOR1( ImageType           );
    DECLARE_TAG_VISITOR1( Modality            );
    DECLARE_TAG_VISITOR1( Name                );
    DECLARE_TAG_VISITOR1( PatientID           );
    DECLARE_TAG_VISITOR1( PixelData           );
    DECLARE_TAG_VISITOR2( PixelRepresentation );
    DECLARE_TAG_VISITOR1( PixelSpacing        );
    DECLARE_TAG_VISITOR1( Rows                );
    DECLARE_TAG_VISITOR1( SeriesUID           );
    DECLARE_TAG_VISITOR1( Sexe                );
    DECLARE_TAG_VISITOR1( SliceThickness      );
    DECLARE_TAG_VISITOR1( StudyUID            );
    DECLARE_TAG_VISITOR2( TransfertSyntax     );

// Remove macros to avoid potential conflict with other modules defines
#undef DECLARE_TAG_VISITOR
#undef DECLARE_TAG_VISITOR1
#undef DECLARE_TAG_VISITOR2

private:
    wxString      m_filename;
    bool          m_loaded;
    bool          m_valid;
    unsigned char m_bitsPerPixel;
    bool          m_unsigned;
    bool          m_littleEndian;
    wxSize        m_size;
    char*         m_frame;

    wxString*     m_tags;

    template< class Type >
    void ConvertData( unsigned char* data, const Type* frame );
};

} // namespace dicomsel

#endif // DICOMSEL_DICOMFILE_H

/* End of File */
