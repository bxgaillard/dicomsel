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
#include <wx/dir.h>

// DCMTK
#include <dicomsel/dcmtk.h>
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

// Current module
#include <dicomsel/DicomCollection.h>
#include <dicomsel/DcmtkCollection.h>


namespace dicomsel
{

DcmtkCollection::DcmtkCollection( DicomCollection::Visitor* const visitor )
:   DicomCollection( visitor )
{}

DcmtkCollection::~DcmtkCollection( void )
{}

bool DcmtkCollection::ScanDirectory( const wxString& name )
{
    if( m_visitor != NULL ) m_visitor->ParsingDirectory();

    wxArrayString files;
    if( wxDir::GetAllFiles( name, &files ) == 0 ) return false;
    files.Sort();

    const int count = static_cast< int >( files.GetCount() );
    bool stop = false;

    // Open progress dialog
    if( m_visitor != NULL && ! m_visitor->BeginParsing( count ) )
    {
	stop = true;
    }
    else
    {
	// Parse files
	for( int i = 0; i < count; ++i )
	{
	    // File informations
	    wxCharBuffer filename = files[i].fn_str();
	    DcmFileFormat format;
	    OFString name, study, series;

	    // Try to load a file
	    if( format.loadFile( filename ).good() )
	    {
		DcmDataset* const dataset = format.getDataset();

		// Try to get basic tags
		if( dataset->findAndGetOFString( DCM_PatientsName,
						 name ).good() &&
		    dataset->findAndGetOFString( DCM_StudyInstanceUID,
						 study ).good() &&
		    dataset->findAndGetOFString( DCM_SeriesInstanceUID,
						 series).good() )
		{
		    // Read ok, add this file
		    m_visitor->AddFile( wxString( name.c_str(),
						  wxConvISO8859_1 ),
					wxString( study.c_str(),
						  wxConvISO8859_1 ),
					wxString( series.c_str(),
						  wxConvISO8859_1 ),
					files[i] );
		}

		// Free memory
		format.clear();
	    }

	    // Make the progress bar advance
	    if( m_visitor != NULL && !m_visitor->ProgressParsing() )
	    {
		stop = true;
		break;
	    }
	}
    }

    // Close progress dialog
    if( m_visitor != NULL )
    {
	if( stop ) m_visitor->AbortParsing();
	m_visitor->EndParsing();
    }

    return !m_stop;
}

} // namespace dicomsel

/* End of File */
