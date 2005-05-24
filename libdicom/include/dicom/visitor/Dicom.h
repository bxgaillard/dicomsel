// CDicom.h
#ifndef VISITOR_DICOMFILE_H
#define VISITOR_DICOMFILE_H

#include "Virtuals/Type.h"
#include "visitor/AbstractDicomFile.h"

#include <list>
#include <vector>
#include "ExceptionDicom.h"

#include "Virtuals/mfo/mfo.h"

namespace mfo
{
	class CPatient;
}

namespace dicom
{

namespace io
{
	class CDicomFile;
	class CSimpleDirectory;
}

namespace visitor
{

/**
 * CDicom
 *********************
 * Namespace : dicom
 *********************
 * Description:
 *
 *********************
 */
class CDicom : public dicom::visitor::CAbstractDicomFile
{

public:

	/**
	 * Constructor.
	 */
    explicit				CDicom									( void );

	/**
	 * Destructor.
	 * Each patient saved in the list of this visitor is deleting.
	 * All delete finished, the vector is clear.
	 */
	virtual					~CDicom                                 ();

	/**
	 * @return const bool _pTag : true if picture is present
	 */
    const bool				IsPicturePresent						( void ) const ;

	/**
	 * @param _bPicturePresent [const bool] : set the member m_bPicturePresent 
	 * in true if the picture tag is present
	 */
    void					SetPicturePresent						( const bool _bPicturePresent ) ;

	/**
	 * @return const bool std::vector<mfo::CPatient*> * : Patient vector of this visitor
	 */
    std::vector<mfo::CPatient*> * GetVectorPatient					( void ) ;

	/**
	 * list all elements to an output stream.
	 *********************
	 * @param _os [std::ostream &] : output stream which call this function (eg : std::cout)
	 * @param _visitor [dicom::visitor::CDicom&] : visitor which parsed files
	 *********************
	 * @return std::ostream& : output stream which will process
	 */
    friend std::ostream&	operator <<								( std::ostream & _os, dicom::visitor::CDicom & _visitor ) ;

protected:

	dicom::tag::CDicomTag *	m_pTag;
    bool					m_bPicturePresent;
    std::string				m_sFullPath;
    dicom::io::CDicomFile *	m_pFile;
    std::vector<mfo::CPatient*>	m_vectorPatient;


	/**
	 * When an unknown tag read, it's this function 
	 * which is called.
	 ****************************************
	 * @param _Tag [dicom::tag::CDicomTag &] : the unknown tag
	 ****************************************
	 * @return the size of this tag
	 */
	const uint32			VisitUnknownDicomTag					( dicom::tag::CDicomTag & _Tag );

	/**
	 * This function can parse a directory. For each file, it calls the parsing file.
	 *********************
	 * @param _Directory [dicom::CSimpleDirectory &] : the directory analysed
	 *********************
	 * @exception dicom::exception::CBase
	 */
    void					ParseDirectory							( dicom::io::CSimpleDirectory & _Directory) ;
    
	/**
	 * This functions sifts throught the input dicom data.  
	 * It's the heart of the program.
	 * While the read of file is not of bound, 
	 * next tag is reading and creating. After, the tag read his 
	 * data element and call the actual visitor (to list,
	 * create an inrimage, etc ...).
	 *
	 * Dictionary type is not used actually but, it will
	 * can be modified is a next time (eg : create a DTD with XML... More article)
	 * dicom::CDicomDictionary::sDicomTypes dtType = CDicomDictionary::unknown;
	 * assert(dtType==CDicomDictionary::unknown);
	 *
	 *********************
	 * @param _file [dicom::CDicom &] : the file analysed
	 *********************
	 * @see dicom::tag::CDicomTag::visit
	 * @see dicom::tag::CName::visit
	 *********************
	 * @exception dicom::io::CDicomFile::CFileException : Format Not Defined
	 */
	void					ParseFile								( dicom::io::CDicomFile & _file) ;

private:

	/**
	 * Try to read a tag in the dicom file. If
	 * the first read don't process, we try 
	 * with an inverse read (uniquely possible for
	 * the first read).
	 ****************************************
	 * @param _file [dicom::io::CDicomFile &] :
	 * @param _i32Group [int32 &] :
	 * @param _i32Element [int32 &] :
	 * @param _i32ElementLength [int32 &] :
	 * @param _bFirstOne [bool &] :
	 ****************************************
	 * @see dicom::io::CDicomFile::ReadTag
	 ****************************************
	 * @exception dicom::io::CDicomFile::CFileException : Format Not Defined
	 */
	void					ReadHeaderOfMyTag						(	dicom::io::CDicomFile & _file, 
																		int32 & _i32Group, int32 & _i32Element,
																		int32 & _i32ElementLength, bool & _bFirstOne );

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 */
   	explicit				CDicom									( const CDicom & ) ;

	/**
	 * Not used.
	 */
    CDicom &				operator =								( const CDicom & ) ;

	//@}

public:

    /**
     * CVisitorException
     *********************
     * Namespace : dicom
     *********************
     */
    class CVisitorException : public dicom::exception::CBase
    {
		public:
			/**
			 * Constructor with two parameter
			 *********************
			 * @param _stringFile [const string &] : Full name of the file
			 * @param _stringError [const string &] : Error string
			 */
			CVisitorException(const std::string _stringFile, const std::string _stringError):
				dicom::exception::CBase(_stringFile + " : " + _stringError)
			{}
	};


#ifdef _DEBUG
public:
	/**
	 * Default function when the parser find a tag not used.
	 *********************
	 * @param _Tag [const dicom::tag::CDicomTag &] : actual tag parsed
	 */
    void					VisitDicomTag							( dicom::tag::CDicomTag & _Tag);
//private:
	static uint32			dicom::visitor::CDicom::s_ui32GroupOrder ;
#endif

};

}// end namespace visitor

}// end namespace

#endif // VISITOR_DICOMFILE_H
