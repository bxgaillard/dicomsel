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
#include <wx/strconv.h>

// STL
#include <string>

// Virtuals basic types
#include <Virtuals/Type.h>

// DICOM I/O
#include <dicom/io/SimpleDirectory.h>
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
#include <dicomsel/DicomTree.h>
#include <dicomsel/DicomCollection.h>
#include <dicomsel/LibDicomCollection.h>


namespace dicomsel
{

LibDicomCollection::LibDicomCollection(
    DicomCollection::Visitor* const visitor )
:   DicomCollection( visitor )
{}

LibDicomCollection::~LibDicomCollection( void )
{}

bool LibDicomCollection::ScanDirectory( const wxString& name )
{
    // FIXME: conversion from wxString to std::string might be loosy.
    // This can only be fixed by redesigning DICOM library string handling...
    // Note though that this isn't a problem under recent systems which use
    // UTF-8 for filename encoding.

    // Create DICOM directory parser
    dicom::io::CSimpleDirectory dir( std::string( name.fn_str() ) );
    m_stop = false;

    // Display progress dialog and load directory
    if( m_visitor != NULL )
    {
	m_visitor->ParsingDirectory();
	dir.LoadDirectory();
	if( ! m_visitor->BeginParsing( dir.GetSize() ) ) m_stop = true;
    }

    // Visit this directory
    if( !m_stop )
    {
	dir.Visit( this );

	// Free memory
	m_name.Clear();
	m_study.Clear();
	m_series.Clear();
    }

    // Close progress dialog
    if( m_visitor != NULL )
    {
	if( m_stop ) m_visitor->AbortParsing();
	m_visitor->EndParsing();
    }

    return !m_stop;
}

const bool LibDicomCollection::IsCreateImage( void ) const
{
    // We aren't creating images for the time being
    return false;
}

void LibDicomCollection::VisitDicomDirectory(
    dicom::io::CSimpleDirectory& dir )
{
    // Visit this directory
    ParseDirectory( dir );
}

void LibDicomCollection::VisitDicomFile( dicom::io::CDicomFile& file )
{
    if( file.IsOpen() && !m_stop )
    {
	// Reset properties
	m_name.Clear();
	m_study.Clear();
	m_series.Clear();

	// Parse file
	m_pFile = &file;
	try
	{
	    ParseFile( file );
	    if( m_visitor != NULL )
	    {
		m_visitor->AddFile( m_name, m_study, m_series,
				    wxString( file.GetFullName().c_str(),
					      *wxConvFileName ) );
	    }
	}
	catch( dicom::exception::CBase& )
	{}
	m_pFile = NULL;

	// Update progress dialog
	if( m_visitor != NULL && !m_visitor->ProgressParsing() )
	{
	    m_stop = true;
	}
    }
}

const uint32 LibDicomCollection::VisitDicomNameTag( dicom::tag::CName& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_name = wxString( tag.GetNamePatient().c_str(), *wxConvCurrent );
    return pos;
}

const uint32 LibDicomCollection::VisitDicomStudyUIDTag(
    dicom::tag::CStudyUID& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_study = wxString( tag.GetUID().c_str(), *wxConvCurrent );
    return pos;
}

const uint32 LibDicomCollection::VisitDicomSeriesUIDTag(
    dicom::tag::CSeriesUID& tag )
{
    const uint32 pos = tag.SetValue( *m_pFile );
    m_series = wxString( tag.GetUID().c_str(), *wxConvCurrent );
    return pos;
}

// Macro for generating default-behaving tag handlers
#define DEFAULT_TAG_VISIT( name, suffix ) \
    const uint32 LibDicomCollection::VisitDicom ## name ## suffix( \
	dicom::tag::C ## name& tag )                               \
    {                                                              \
	return DefaultTagVisit( tag );                             \
    }

// Some implementation flaws? ;-)
#define DEFAULT_TAG_VISIT1( name ) DEFAULT_TAG_VISIT( name, Tag )
#define DEFAULT_TAG_VISIT2( name ) DEFAULT_TAG_VISIT( name,     )

// Default behaviour for unhandled DICOM tags
DEFAULT_TAG_VISIT1( AcquisitionDate     )
DEFAULT_TAG_VISIT1( AcquisitionNumber   )
DEFAULT_TAG_VISIT1( AcquisitionTime     )
DEFAULT_TAG_VISIT1( AcquisitionZone     )
DEFAULT_TAG_VISIT1( Birthday            )
DEFAULT_TAG_VISIT1( BitsAllocated       )
DEFAULT_TAG_VISIT1( Columns             )
DEFAULT_TAG_VISIT1( Hospital            )
DEFAULT_TAG_VISIT1( ImagePosition       )
DEFAULT_TAG_VISIT1( ImageType           )
DEFAULT_TAG_VISIT1( Modality            )
DEFAULT_TAG_VISIT1( PatientID           )
DEFAULT_TAG_VISIT1( PixelData           )
DEFAULT_TAG_VISIT2( PixelRepresentation )
DEFAULT_TAG_VISIT1( PixelSpacing        )
DEFAULT_TAG_VISIT1( Rows                )
DEFAULT_TAG_VISIT1( Sexe                )
DEFAULT_TAG_VISIT1( SliceThickness      )
DEFAULT_TAG_VISIT2( TransfertSyntax     )

const uint32 LibDicomCollection::DefaultTagVisit( dicom::tag::CDicomTag& tag )
    const
{
    // Pass this tag
    uint32 pos;
    if( tag.GetLength() >= 0) pos = m_pFile->GetOffset() + tag.GetLength();
    else                      pos = m_pFile->GetSize();
    m_pFile->SetOffset( pos );

    // Return new offset in file
    return pos;
}

} // namespace dicomsel

/* End of File */
