/*! \file DicomFile.h
 *********************
 * Namespace : dicom
 *********************
 * This file can work on dicom file. The CDicomFile
 * class inherits to CTypedFile (and CSimpleFile). It adds
 * byte orded functions and check the value representation.
 * It's this class which parse the dicom file (with @see
 * dicom::visitor::dicom).
 * It's a orginal idea of George J. Grevera (dicom2pgm.cpp).
 * The original file doesn't recopy, this program give only
 * the idea  of this parse.
 * Click <a href="http://www.rad.upenn.edu/~grevera/LICENSE">here</a>
 * for a copy of dicom2pgm.cpp's license.
 */
 
// DicomFile.h
#ifndef DICOMFILE_H
#define DICOMFILE_H

#include "Virtuals/Type.h"
#include "TypedFile.h"
#include "ExceptionDicom.h"

namespace dicom
{

// Template class name used
class CDicomGroup;
namespace tag
{
    class CDicomTag;
}
namespace visitor
{
    class CAbstractDicomFile;
}

namespace io
{

/** \class CDicomFile
 *********************
 * Namespace : dicom::io
 *********************
 * This file can work on dicom file. The CDicomFile
 * class inherits to CTypedFile (and CSimpleFile). It adds
 * byte orded functions and check the value representation.
 */
class CDicomFile:public dicom::io::CTypedFile
{
public :

    /**
     * Constructor with one parameter.
     * Initialize the VR member in implicit mode.
     *********************
	 * @param _stringName [const std::string &] : name of the file
     */
	explicit				CDicomFile								( const std::string & _stringName ) ;

    /**
     * Return the value representation of the file
     *********************
     * @return const bool : true if the file use an explicite
     *                      value representation, false otherwise
     */
    const bool				IsExpliciteVR							( void ) const ;

    /**
     * Read a partial tag (group, element, lenght) and check
     * its format.
	 *********************
     * @param _i32Group [int32 &] : index of the group
     * @param _i32Element [int32 &] : index of the element
     * @param _i32Length [int32 &] : size of the data
     * @param _bFirstOne [bool&] : flag indicating the number of crossing
     *********************
	 * @exception std::exception on ifstream::read
     */
    void					ReadTag									(	int32 & _i32Group, int32 & _i32Element, 
																		int32 & _i32Length, bool & _bFirstOne ) ;

    /**
     * Function used by the Design Pattern VISITOR,
     * to navigate in data structure.
	 *********************
	 * @pre visitor allocated
     *********************
     * @param _pVisitor [dicom::visitor::CVisitorAbstractDicomFile * const]
     */
    void					Visit									( dicom::visitor::CAbstractDicomFile * const _pVisitor ) ;

private :

    /**
     * explicit value representation flag
     */
    bool					m_bExplicitVR ;

	/**
	 * Check if the file use a "DICM" header
	 *********************
	 * @pre Buffer size is superior or equals at 4
	 *********************
	 * @param _ui8HeaderDICM [const uint8 *] : a buffer of 4 byte
	 *********************
	 * @return const bool : true if the file use a "DICM" header,
	 *                      false otherwise
	 */
    const bool				IsHeaderDicom							( const uint8 * _ui8HeaderDICM ) const ;

	/**
	 * Check if the file use a Acr/Nema 2 format
	 *********************
	 * @param _i32Group [const int32 &] : index of the new group
	 * @param _i32Element [const int32 &] : index of the new element
	 * @param _i32Length [const int32 &] : new size of the data
	 *********************
	 * @return const bool : true if the file use a
	 *                      Acr/Nema 2 format, false otherwise
	 */
    const bool				IsAcrNema2File							(	const int32 & _i32Group, const int32 & _i32Element, 
																		const int32 & _i32Length) const ;

	/**
	 * Check the value representation.
	 * The size buffer is always 4. But, the size string can
	 * be less (eg : "xx\0").
	 *********************
	 * @pre Buffer size is superior or equals at 2
	 *********************
	 * @param _pBuff [const char * const] : The analysed buffer
	 *********************
	 * @return const bool : true if the tag use explicate 32 bits
	 *                      value representation, false otherwise
	 */
    const bool				IsVRExplicite32_1						( const char * const _pBuff ) ;

	/**
	 * Check the value representation.
	 * The size buffer is always 4. But, the size string can
	 * be less (eg : "xx\0").
	 *********************
	 * @pre Buffer size is superior or equals at 4
	 *********************
	 * @param _pBuff [const char * const] : The analysed buffer
	 *********************
	 * @return const bool : true if the tag use explicate 32 bits
	 *                      value representation, false otherwise
	 */
    const bool				IsVRExplicite32_2						( const char * const _pBuff ) ;

	/**
	 * Check the value representation.
	 * The size buffer is always 4. But, the size string can
	 * be less (eg : "xx\0").
	 *********************
	 * @pre Buffer size is superior or equals at 2
	 *********************
	 * @param _pBuff [const char * const] : The analysed buffer
	 *********************
	 * @return const bool : true if the tag use explicate 16 bits
	 *                      value representation, false otherwise
	 */
    const bool				IsVRExplicite16_1						( const char * const _pBuff ) ;

	/**
	 * Check the value representation.
	 * The size buffer is always 4. But, the size string can
	 * be less (eg : "xx\0").
	 *********************
	 * @pre Buffer size is superior or equals at 4
	 *********************
	 * @param _pBuff [const char * const] : The analysed buffer
	 *********************
	 * @return const bool : true if the tag use explicate 16 bits
	 *                      value representation, false otherwise
	 */
    const bool				IsVRExplicite16_2						( const char * const _pBuff ) ;

	/**
	 * Read a partial tag (group, element, lenght)
	 *********************
	 * @pre file opened
	 *********************
	 * @param _i32Group [int32 &] : index of the new group
	 * @param _i32Element [int32 &] : index of the new element
	 * @param _i32Length [int32 &] : new size of the data
	 *********************
	 * @exception std::exception on ifstream::read
	 *********************
	 * @see ifstream::read
	 */
    void					ReadHeaderLine							(	int32 & _i32Group, int32 & _i32Element, 
																		int32 & _i32Length) ;

    /**
     * After check byte order in the file, we must check
     * the length of the element read.
     *********************
	 * @param _i32Group [int32 &] : index of the new group
	 * @param _i32Element [int32 &] : index of the new element
	 * @param _i32Length [int32 &] : new size of the data
     *********************
     * @return const bool : true if the lenght is good, false otherwise
     *********************
	 * @exception std::exception on ifstream::read
	 *********************
     * @see CDicomFile::CheckByteOrder
     */
    void					CheckByteOrder							(	int32 & _i32Group, int32 & _i32Element, 
																		int32 & _i32Length) ;

	/**
	 * After check byte order in the file, we must check
	 * the length of the element read.
	 *********************
	 * @param [_i32Length] int32& : length supposed of the element
	 *********************
	 * @return const bool : true if the lenght is good, false otherwise
	 *********************
	 * @see CDicomFile::CheckByteOrder
	 */
    const bool				CheckLengthElement						( int32 & _i32Length ) ;

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 */
   	explicit				CDicomFile								( const CDicomFile & ) ;

	/**
	 * Not used.
	 */
    CDicomFile &			operator=								( const CDicomFile & ) ;

	//@}

};

}// end namespace io

}// end namespace dicom

#endif // DICOMFILE_H
