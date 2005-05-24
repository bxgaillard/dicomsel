// SimpleFile.h
#ifndef SIMPLEFILE_H
#define SIMPLEFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "Virtuals/Type.h"
#include "ExceptionDicom.h"

namespace dicom
{

namespace io
{

/*! \class CSimpleFile
 *********************
 * Namespace : dicom::io
 *********************
 * This class is used to work with an abstract file.
 * It initializes fullname and size of the file. A flag indicates
 * if the file is loaded.
 *********************
 */
class CSimpleFile
{
protected:
	/**
	 * Full name of the file.
	 */
	std::string				m_stringFullName;

	/**
	 * Size of the file.
	 */
	uint32					m_uiSize;

	/**
	 * Stream of the file.
	 */
	std::ifstream			m_iFile;

public:

	/** \class CFileException
	 *********************
	 * Namespace : dicom
	 *********************
	 * Description : Basic exception in used of dicom file.
	 *********************
	 * @see VerifyCreatePath
	 * @see LoadInputFile
	 *********************
	 */
	class CFileException : public dicom::exception::CBase
	{
		public:
			/**
			 * Constructor with two parameter
			 *********************
			 * @param _stringFile : (const string) Full name of the file.
			 * @param _stringError : (const string) Error string.
			 */
			CFileException(const std::string _stringFile, const std::string _stringError):
				dicom::exception::CBase(_stringFile + " : " + _stringError)
			{}
	};

	/**
	 * Constructor with one parameter.
	 *********************
	 * @param _fullName [const std::string &] : Full name of the file
	 */
	explicit				CSimpleFile								( const std::string & _fullName ) ;

	/**
	 * Destructeur.
	 */
	virtual					~CSimpleFile							();

	/**
	 * Open the file and get its size.
	 * If an error occur, exceptions are throw.
	 *********************
	 * @pre input stream file not loaded.
	 *********************
	 * @post input stream open
	 * @post offset is set on beginning
	 *********************
	 * @exception CFileException with dicom::exception::FileNotFound message
	 */
	void					LoadInputFile							( void ) ;

	/** @name Accessor (Get) */
	//@{

	/**
	 * Full name of the file.
	 *********************
	 * @pre valid filename defined.
	 *********************
	 * @return const std::string
	 */
	const std::string&		GetFullName								( void ) const ;

	/**
	 * Size of the file.
	 *********************
	 * @pre input stream file loaded.
	 *********************
	 * @return const uint32
	 */
	const uint32			GetSize									( void ) const ;

	/**
	 * Position in the file.
	 *********************
	 * @pre input stream file loaded.
	 *********************
	 * @return const uint32
	 */
	const uint32			GetOffset								( void ) const ;

	//@}
	/** @name Accessor (Boolean) */
	//@{

	/**
	 * Accessor.
	 *********************
	 * @return bool
	 */
	const bool				IsOpen									( void ) const ;

	//@}
	/** @name Accessor (Set) */
	//@{

	/**
	 * Set offset in the input stream file
	 *********************
	 * @pre input stream file loaded.
	 * @pre the new offset must not above the file size.
	 *********************
	 * @post file offset move to the value parameter
	 *********************
	 * @param _ui32Offset [const uint32 &]  : new offset for the input file stream.
	 */
	void					SetOffset								( const uint32 & _ui32Offset ) ;

	//@}

#ifdef _DEBUG
	/**
	 * Debug.
	 */
	void					Dump									( void ) ;
#endif

private :
	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 */
	explicit				CSimpleFile								( const CSimpleFile & ) ;

	/**
	 * Not used.
	 */
	CSimpleFile &			operator=								( const CSimpleFile & ) ;

	//@}


	/**
	 * Verify the fullpath and return exception if not valid
	 *********************
	 * @exception CFileException with dicom::exception::FileNotFound message.
	 */
	void					VerifyCreatePath						( void ) const ;
};

}// end namespace io

}// end namespace dicom

#endif // SIMPLEFILE_H
