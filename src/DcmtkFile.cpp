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

// DCMTK
#include <dicomsel/dcmtk.h>
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dctagkey.h>

// Current module
#include <dicomsel/DicomTree.h>
#include <dicomsel/TagSet.h>
#include <dicomsel/DicomFile.h>
#include <dicomsel/DcmtkFile.h>


namespace dicomsel
{

DcmtkFile::DcmtkFile( const wxString& filename )
:   DicomFile( filename )
{}

DcmtkFile::~DcmtkFile( void )
{}

DicomTree::Library DcmtkFile::GetLibrary( void )
{
    return DicomTree::LIB_DCMTK;
}

bool DcmtkFile::ReadFile( const wxString& filename )
{
    // File informations
    wxCharBuffer file = filename.fn_str();
    DcmFileFormat format;

    // Try to load a file
    if( format.loadFile( file ).good() )
    {
	DcmDataset* const dataset = format.getDataset();
	OFString str;

	for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; ++i )
	{
	    const TagSet::TagID tag = static_cast< TagSet::TagID >( i );
	    DcmTagKey key( TagSet::GetTagGroup( tag ),
			   TagSet::GetTagElement( tag ) );
	    if( dataset->findAndGetOFString( key, str ).good() )
	    {
		SetTagString( tag, wxString( str.c_str(), wxConvISO8859_1 ) );
	    }
	}

	// Free memory
	format.clear();
    }

    return true;
}

} // namespace dicomsel

/* End of File */
