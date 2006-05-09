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


#ifndef DICOMSEL_DICOMCOLLECTION_H
#define DICOMSEL_DICOMCOLLECTION_H

class wxString;


namespace dicomsel
{

class DicomCollection
{
public:
    class Visitor
    {
    public:
	virtual ~Visitor( void ) = 0;

    private:
	friend class LibDicomCollection;
	friend class DcmtkCollection;

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


    virtual ~DicomCollection( void ) = 0;

    virtual bool ScanDirectory( const wxString& name ) = 0;

protected:
    DicomCollection( Visitor* const visitor )
    :   m_visitor( visitor )
    {}

    Visitor* m_visitor;
};

} // namespace dicomsel

#endif // DICOMSEL_DICOMCOLLECTION_H

/* End of File */
