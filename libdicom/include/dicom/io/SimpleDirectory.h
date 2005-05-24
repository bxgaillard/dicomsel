// SimpleDirectory.h
#ifndef SIMPLEDirectory_H
#define SIMPLEDirectory_H

#include <string>
#include <vector>
#include <algorithm>
#include "DicomFile.h"
#include "visitor/AbstractDicomFile.h"
#include "Virtuals/Type.h"

namespace dicom
{

namespace io
{

/*! \class CSimpleDirectory
 *********************
 * Namespace : dicom::io
 *********************
 * This class can load a directory. For each file,
 * if this file is interesting (>128, no text or exec...),
 * it adds the file in the file list.
 * This is the file list which visited later.
 * @see dicom::visitor::CAbstractDicomFile
 *********************
 * TO DO : Fix Unix parse directory !
 *********************
 * @see dicom::visitor::CAbstractDicomFile
 */
class CSimpleDirectory
{
protected:
    /**
     * directory's fullname.
     */
    std::string				m_stringFullName;

    /**
     * List of fullname files of the directory.
     */
    std::vector<dicom::io::CDicomFile*> m_vectorFile;

public:
    /**
     * Constructor with one parameter.
     *********************
     * @param _stringFullName [const std::string &]  : directory's fullname
     */
    explicit				CSimpleDirectory						( const std::string & _stringFullName) ;

    /**
     * Destructeur.
     */
    virtual					~CSimpleDirectory						() ;

    /**
     * Add all file of the current directory in a list.
     *********************
     * @exception CSimpleFile::CFileException with dicom::exception::FileNotFound parameter
     */
    void					LoadDirectory							( void ) ;

#ifdef _DEBUG
    void					Dump									( void ) ;
#endif

    /**
     * Return the fullname of the directory
     *********************
     * @return const std::string : the fullname of the directory
     */
    const std::string &		GetFullName								( void ) const ;

    /**
     * Return the number of file loaded (the number can different
     * of file number indeed in the folder)
     *********************
     * @return const uint32 : the number of file loaded in this directory
     */
    const uint32			GetSize									( void ) const ;

    /**
     * Specify if the list is empty or not.
     *********************
     * @return const bool : true if the list is not empty, false otherwise
     */
    const bool				IsNotEmpty								( void ) const ;

    /**const std::string stdTemp = 
     * Return the last file of the file list.
     *********************
     * @pre Number file not null.
     *********************
     * @return dicom::CDicomFile* : the last file
     */
    dicom::io::CDicomFile*	GetLastFile								( void ) ;

    /**
     * Remove the last file of the file list.
     *********************
     * @pre Number file not null.
     *********************
     * @return dicom::CDicomFile* : the last file or NULL if the list is empty
     */
    dicom::io::CDicomFile*	RemoveLastFile							( void ) ;

    /**
     * Function used by the Design Pattern VISITOR, to navigate in data structure.
     *********************
     * @pre visitor not null.
     *********************
     * @param _pVisitor [CVisitorAbstractDicomFile *] : Design Pattern's visitor (a dicom::visitor::CCreateList eg.)
     */
    void					Visit									( dicom::visitor::CAbstractDicomFile * const _pVisitor ) ;

private:
	/** @name Copy 
	 * Copy operator and constructor & default constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 */
   	explicit				CSimpleDirectory						( const CSimpleDirectory & ) ;

	/**
	 * Not used.
	 */
   	explicit				CSimpleDirectory						( void ) ;

	/**
	 * Not used.
	 */
    CSimpleDirectory &		operator=								( const CSimpleDirectory & ) ;

	//@}


	/**
	 * Attach filter on loading of file.
	 * No "txt", "exe", ".", "..", "dir" and "dicomdir"
	 * No file < 128 octets (size of header dicom)
	 * This method accelerates the reading and loading.
	 *********************
	 * @param [_stringFolder] const std::string& : the fullname of the folder.
	 * @param [_stringRelatifName] const std::string& : the fullname of the file.
	 * @param [_stringName] const std::string& : the fullname of the file.
	 *********************
	 * @exception CSimpleFile::CFileException with dicom::exception::FileNotFound parameter
	 */
    const std::string		addFile									(	const std::string _stringFolder, 
																		const std::string _stringRelatifName, 
																		const std::string _stringName ) ;

	/**
	 * Verify if the fullname is good formated
	 *********************
	 * @param [_stringPath] const std::string& : fullname of the directory
	 *********************
	 * @exception CSimpleFile::CFileException with dicom::exception::FileNotFound parameter
	 */
    void					VerifyCreatePath						( const std::string _stringPath) const ;

    /**
     * Add all file of the directory process in parameter in a list.
     *********************
     * @param _stringPath [const std::string &] : the fullname of the directory
     *********************
     * @exception CSimpleFile::CFileException with dicom::exception::FileNotFound parameter
     */
    void					LoadDirectory							( const std::string _stringPath ) ;
};

}// end namespace io

}// end namespace dicom


#endif // SIMPLEDirectory_H
