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


#ifndef DICOMSEL_DCMTKFILE_H
#define DICOMSEL_DCMTKFILE_H

// Current module
#include <dicomsel/DicomTree.h>
#include <dicomsel/DicomFile.h>


class wxString;


namespace dicomsel
{

class DcmtkFile : public DicomFile
{
public:
    DcmtkFile( const wxString& filename );
    virtual ~DcmtkFile( void );

    virtual DicomTree::Library GetLibrary( void );

protected:
    virtual bool ReadFile( const wxString& filename );
};

} // namespace dicomsel

#endif // DICOMSEL_DCMTKFILE_H

/* End of File */
