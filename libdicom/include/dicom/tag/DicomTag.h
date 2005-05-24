// DicomTag.h
#ifndef DICOM_TAG_H
#define DICOM_TAG_H

#include "Virtuals/Type.h"
#include "ExceptionDicom.h"
#include "DicomDictionary.h"
#include <string>
#include <sstream>

namespace dicom
{

namespace io
{
	class CDicomFile;
}

namespace visitor
{
    class CAbstractDicomFile;
}

namespace tag
{

class CDicomTag
{

protected:

#ifdef _DEBUG_CONSOLE_VERBOSE

    /**
     * Name (string) of the tag. "unknown" string is used if the tag is unknown
     */
    std::string				m_sName;

    /**
     * This string is used by the ostringstream.
     * Don't touch this var...
     */
    std::string				m_sBuffer;

    /**
     * This is a stream which contains the value of the tag.
     */
    std::ostringstream		m_ssBuffer;

    /**
     * Type of this tag
     */
    dicom::CDicomDictionary::sDicomTypes m_dtTypes;

#endif

#ifdef _DEBUG
    /**
     * Group of this tag (int32)
     */
    int32					m_iGroup;

    /**
     * Number of this tag (int32)
     */
    int32					m_iElement;
#endif

    /**
     * Size of this tag (int32). -1 if the size is unknown
     */
    int32					m_iLength;

	/**
	 * Data extracted from dicom file
	 */
    std::string				m_stringData ;

public :

	/**
	 * Constructor.
	 * Initialized the data member.
	 *********************
	 * @param _i32Group [const int32]	: tag's group.
	 * @param _i32Element [const int32] : tag's element.
	 * @param _i32Length [const int32]	: tag's length.
	 *********************
	 * @see dicom::tag::CDicomTag
	 */
	explicit				CDicomTag								( const int32 _i32Group, const int32 _i32Element, const int32 _i32Length );

#ifdef _DEBUG_CONSOLE_VERBOSE
    const    std::string&	GetName									( void ) const ;
    const CDicomDictionary::sDicomTypes GetType						( void ) const ;
    virtual const std::string GetValue								( void ) const ;
#endif

#ifdef _DEBUG
    const int32				GetGroup								( void ) const ;
    const int32				GetElement								( void ) const ;
#endif

    const int32				GetLength								( void ) const ;

    virtual					~CDicomTag								() ;

	/**
	 * @param _File [dicom::io::CDicomFile &] : Full name of the parsed file
	 *********************
	 * @pre file's offset + tag's length < file's length.
	 * @pre file open.
	 */
	void					ReadMeIntoFile							( dicom::io::CDicomFile & _File ) ;

	/**
	 * DP VISITOR.
	 * According to the visitor (image, list, etc...) the Time used or not.
	 * If this tag used by the visitor, the SetValue() function called.
	 *********************
	 * @param _pVisitor [dicom::visitor::CAbstractDicomFile * const] : Vistor
	 *********************
	 * @pre visitor not null
	 *********************
	 * @return const uint32 : final offset after read
	 */
   virtual const uint32		Visit									( dicom::visitor::CAbstractDicomFile * const _pVisitor ) ;

	/**
	 * Update position in file after read.
	 *********************
	 * @param _File [dicom::io::CDicomFile &] : Full name of the parsed file
	 *********************
	 * @return const uint32 : final offset after read
	 */
	virtual const uint32	SetValue								( dicom::io::CDicomFile & _File ) ;

private :

	explicit				CDicomTag								( const CDicomTag & ) ;
    CDicomTag &				operator =								( const CDicomTag & ) ;


public :

    class CTagExceptionFatal : public dicom::exception::CBase
    {
		public :
			/**
			 * Constructor with two parameter
			 *********************
			 * @param _stringFile [const string &] : Full name of the file
			 * @param _stringError [const string &] : Error string
			 */
			CTagExceptionFatal(const std::string _stringFile, const std::string _stringError):
				dicom::exception::CBase(_stringFile + " : " + _stringError)
			{}
    };
};

}// end namespace tag

}// end namespace

#endif // DICOM_TAG_H
