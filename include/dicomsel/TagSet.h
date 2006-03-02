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


#ifndef DICOMSEL_TAGSET_H
#define DICOMSEL_TAGSET_H

class wxString;


namespace dicomsel
{

class TagSet
{
public:
    enum TagID
    {
	TAG__FIRST = 0,
	TAG_ACQUISITION_DATE = 0,
	TAG_ACQUISITION_NUMBER,
	TAG_ACQUISITION_TIME,
	TAG_ACQUISITION_ZONE,
	TAG_BIRTHDAY,
	TAG_BITS_ALLOCATED,
	TAG_COLUMNS,
	TAG_HOSPITAL,
	TAG_IMAGE_POSITION,
	TAG_IMAGE_TYPE,
	TAG_MODALITY,
	TAG_NAME,
	TAG_PATIENT_ID,
	// Unused: TAG_PIXEL_DATA,
	TAG_PIXEL_REPRESENTATION,
	TAG_PIXEL_SPACING,
	TAG_ROWS,
	TAG_SERIES_UID,
	TAG_SEX,
	TAG_SLICE_THICKNESS,
	TAG_STUDY_UID,
	TAG_TRANSFERT_SYNTAX,
	TAG__LAST
    };

    TagSet( void );

    static const TagID     GetTagCount( void ) { return TAG__LAST; }
    static const wxString& GetTagID    ( TagID tag );
    static const wxString& GetTagName  ( TagID tag );

    void Clear( void );
    void Add( TagID tag );
    void Remove( TagID tag );
    bool IsIn( TagID tag ) const;

    int GetCount( void ) const;
    void Rewind( void );
    TagID GetNext( void );

private:
    struct TagSpec;

    bool m_tags[TAG__LAST];
    int  m_count;
    int  m_cursor;
    static const TagSpec m_tagNames[];
};

} // namespace dicomsel 

#endif // DICOMSEL_TAGSET_H

/* End of File */
