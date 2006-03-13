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
//#ifndef WX_PRECOMP
# include <wx/string.h>
//#endif

// Current module
#include <dicomsel/TagSet.h>

namespace dicomsel
{

struct TagSet::TagSpec
{
    const wxString id;
    const wxString name;
};

const TagSet::TagSpec TagSet::m_tagNames[TagSet::TAG__LAST + 1] =
{
    { wxT( "AcquisitionDate"     ), wxT( "Acquisition Date"     ) },
    { wxT( "AcquisitionNumber"   ), wxT( "Acquisition Number"   ) },
    { wxT( "AcquisitionTime"     ), wxT( "Acquisition Time"     ) },
    { wxT( "AcquisitionZone"     ), wxT( "Acquisition Zone"     ) },
    { wxT( "Birthday"            ), wxT( "Birthday"             ) },
    { wxT( "BitsAllocated"       ), wxT( "Allocated Bits"       ) },
    { wxT( "Columns"             ), wxT( "Columns (Width)"      ) },
    { wxT( "Hospital"            ), wxT( "Hospital"             ) },
    { wxT( "ImagePosition"       ), wxT( "Image Position"       ) },
    { wxT( "ImageType"           ), wxT( "Image Type"           ) },
    { wxT( "Modality"            ), wxT( "Modality"             ) },
    { wxT( "Name"                ), wxT( "Name"                 ) },
    { wxT( "PatientID"           ), wxT( "Patient ID"           ) },
// Unused: { wxT( "PixelData"           ), wxT( "Pixel Data (Useless)" ) },
    { wxT( "PixelRepresentation" ), wxT( "Pixel Representation" ) },
    { wxT( "PixelSpacing"        ), wxT( "Pixel Spacing"        ) },
    { wxT( "Rows"                ), wxT( "Rows (Height)"        ) },
    { wxT( "SeriesUID"           ), wxT( "Series UID"           ) },
    { wxT( "Sex"                 ), wxT( "Sex"                  ) },
    { wxT( "SliceThickness"      ), wxT( "Slice Thickness"      ) },
    { wxT( "StudyUID"            ), wxT( "Study UID"            ) },
    { wxT( "TransfertSyntax"     ), wxT( "Transfert Syntax"     ) },
    { wxEmptyString, wxEmptyString }
};

TagSet::TagSet( void )
:   m_cursor( TAG__FIRST )
{
    Clear();
}

const wxString& TagSet::GetTagID( TagID tag )
{
    return m_tagNames[(tag >= 0 || tag < TAG__LAST) ? tag : TAG__LAST].id;
}

const wxString& TagSet::GetTagName( TagID tag )
{
    return m_tagNames[(tag >= 0 || tag < TAG__LAST) ? tag : TAG__LAST].name;
}

void TagSet::Clear( void )
{
    m_count = 0;
    for( int i = TAG__FIRST; i < TAG__LAST; i++ ) m_tags[i] = false;
}

void TagSet::Add( TagID tag )
{
    if( tag >= TAG__FIRST && tag < TAG__LAST )
    {
	if( !m_tags[tag] )
	{
	    m_tags[tag] = true;
	    m_count++;
	}
    }
}

void TagSet::Remove( TagID tag )
{
    if( tag >= TAG__FIRST && tag < TAG__LAST )
    {
	if( m_tags[tag] )
	{
	    m_tags[tag] = false;
	    m_count--;
	}
    }
}

bool TagSet::IsIn( TagID tag ) const
{
    return (tag >= TAG__FIRST && tag < TAG__LAST) ? m_tags[tag] : false;
}

int TagSet::GetCount( void ) const
{
    return m_count;
}

void TagSet::Rewind( void )
{
    m_cursor = TAG__FIRST;
}

TagSet::TagID TagSet::GetNext( void )
{
    while( m_cursor < TAG__LAST && !m_tags[m_cursor] ) m_cursor++;
    return static_cast< TagID >( m_cursor < TAG__LAST ?
				 m_cursor++ : m_cursor );
}

} // namespace dicomsel

/* End of File */
