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


#ifndef DICOMSEL_DCMTKCOLLECTION_H
#define DICOMSEL_DCMTKCOLLECTION_H

// Current module
#include <dicomsel/DicomCollection.h>


namespace dicomsel
{

class DcmtkCollection : public DicomCollection
{
public:
    DcmtkCollection( Visitor* const visitor = NULL );
    virtual ~DcmtkCollection( void );

    virtual bool ScanDirectory( const wxString& name );

private:
    bool m_stop;
};

} // namespace dicomsel

#endif // DICOMSEL_DCMTKCOLLECTION_H

/* End of File */
