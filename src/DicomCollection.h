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


#ifndef DICOMSEL_DICOMCOLLECTION_H
#define DICOMSEL_DICOMCOLLECTION_H

// wxWidgets
#ifndef WX_PRECOMP
# include <wx/string.h>
#endif

// Virtuals basic types
#include <Virtuals/Type.h>
#include <dicom/visitor/Dicom.h>


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

class DicomCollection : public dicom::visitor::CDicom
{
public:
    class Visitor
    {
    private:
	friend class DicomCollection;

	virtual void ParsingDirectory( void ) = 0;
	virtual bool BeginParsing( int total ) = 0;
	virtual void AddFile( const wxString& patient,
			      const wxString& study,
			      const wxString& series,
			      const wxString& file ) = 0;
	virtual bool ProgressParsing( void ) = 0;
	virtual void AbortParsing( void ) = 0;
	virtual void EndParsing( void ) = 0;
    };


    DicomCollection( Visitor* const visitor = NULL );
    virtual ~DicomCollection( void );

    void ScanDirectory( const wxString& name );

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

protected:
    const uint32 DefaultTagVisit( dicom::tag::CDicomTag& tag ) const;

private:
    Visitor* m_visitor;
    bool m_stop;

    wxString m_name, m_study, m_series;
};

} // namespace dicomsel

#endif // DICOMSEL_DICOMCOLLECTION_H

/* End of File */
