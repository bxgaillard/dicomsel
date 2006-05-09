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

// Current module
#include <dicomsel/TagSet.h>

namespace dicomsel
{

struct TagSpec
{
private:
    friend class TagSet;
    const wxString id;
    const wxString name;

public:
    TagSpec( const wxString& tagId, const wxString& tagName )
    :   id( tagId ), name( tagName )
    {}
};

struct TagSet::TagUID
{
    unsigned short group;
    unsigned short element;
};

static const TagSpec m_tagNames[TagSet::TAG__LAST + 1] =
{
    TagSpec( wxT( "AcquisitionDate"     ), wxT( "Acquisition Date"     ) ),
    TagSpec( wxT( "AcquisitionNumber"   ), wxT( "Acquisition Number"   ) ),
    TagSpec( wxT( "AcquisitionTime"     ), wxT( "Acquisition Time"     ) ),
    TagSpec( wxT( "AcquisitionZone"     ), wxT( "Acquisition Zone"     ) ),
    TagSpec( wxT( "Birthday"            ), wxT( "Birthday"             ) ),
    TagSpec( wxT( "BitsAllocated"       ), wxT( "Allocated Bits"       ) ),
    TagSpec( wxT( "Columns"             ), wxT( "Columns (Width)"      ) ),
    TagSpec( wxT( "Hospital"            ), wxT( "Hospital"             ) ),
    TagSpec( wxT( "ImagePosition"       ), wxT( "Image Position"       ) ),
    TagSpec( wxT( "ImageType"           ), wxT( "Image Type"           ) ),
    TagSpec( wxT( "Modality"            ), wxT( "Modality"             ) ),
    TagSpec( wxT( "Name"                ), wxT( "Name"                 ) ),
    TagSpec( wxT( "PatientID"           ), wxT( "Patient ID"           ) ),
// Unused: ( wxT( "PixelData"           ), wxT( "Pixel Data (Useless)" ) ),
    TagSpec( wxT( "PixelRepresentation" ), wxT( "Pixel Representation" ) ),
    TagSpec( wxT( "PixelSpacing"        ), wxT( "Pixel Spacing"        ) ),
    TagSpec( wxT( "Rows"                ), wxT( "Rows (Height)"        ) ),
    TagSpec( wxT( "SeriesUID"           ), wxT( "Series UID"           ) ),
    TagSpec( wxT( "Sex"                 ), wxT( "Sex"                  ) ),
    TagSpec( wxT( "SliceThickness"      ), wxT( "Slice Thickness"      ) ),
    TagSpec( wxT( "StudyUID"            ), wxT( "Study UID"            ) ),
    TagSpec( wxT( "TransfertSyntax"     ), wxT( "Transfert Syntax"     ) ),
    TagSpec( *wxEmptyString, *wxEmptyString )
};

const TagSet::TagUID TagSet::TagUIDS[TagSet::TAG__LAST + 1] =
{
    { 0x0008, 0x0022 }, // TAG_ACQUISITION_DATE
    { 0x0020, 0x0012 }, // TAG_ACQUISITION_NUMBER
    { 0x0008, 0x0032 }, // TAG_ACQUISITION_TIME
    { 0x0008, 0x1030 }, // TAG_ACQUISITION_ZONE
    { 0x0010, 0x0030 }, // TAG_BIRTHDAY
    { 0x0028, 0x0100 }, // TAG_BITS_ALLOCATED
    { 0x0028, 0x0011 }, // TAG_COLUMNS
    { 0x0008, 0x0080 }, // TAG_HOSPITAL
    { 0x0020, 0x0032 }, // TAG_IMAGE_POSITION
    { 0x0008, 0x0008 }, // TAG_IMAGE_TYPE
    { 0x0008, 0x0060 }, // TAG_MODALITY
    { 0x0010, 0x0010 }, // TAG_NAME
    { 0x0010, 0x0020 }, // TAG_PATIENT_ID
// Unused: { 0x7FE0, 0x0010 }, TAG_PIXEL_DATA
    { 0x0028, 0x0103 }, // TAG_PIXEL_REPRESENTATION
    { 0x0028, 0x0030 }, // TAG_PIXEL_SPACING
    { 0x0028, 0x0010 }, // TAG_ROWS
    { 0x0020, 0x000E }, // TAG_SERIES_UID
    { 0x0010, 0x0040 }, // TAG_SEX
    { 0x0018, 0x0050 }, // TAG_SLICE_THICKNESS
    { 0x0020, 0x000D }, // TAG_STUDY_UID
    { 0x0002, 0x0010 }, // TAG_TRANSFERT_SYNTAX
    { 0xFFFF, 0xFFFF }
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

const unsigned short TagSet::GetTagGroup( TagID tag )
{
    return TagUIDS[(tag >= 0 || tag < TAG__LAST) ? tag : TAG__LAST].group;
}

const unsigned short TagSet::GetTagElement( TagID tag )
{
    return TagUIDS[(tag >= 0 || tag < TAG__LAST) ? tag : TAG__LAST].element;
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
