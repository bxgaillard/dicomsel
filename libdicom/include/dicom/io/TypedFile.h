// TypedFile.h
#ifndef TYPEDFILE_H
#define TYPEDFILE_H

#include "Virtuals/Type.h"
#include "SimpleFile.h"

namespace dicom
{

namespace io
{

/** \class CTypedFile
 *********************
 * Namespace : dicom::io
 *********************
 * This class enclose all functions of CSimpleFile,
 * add read function and the notion of endianess.
 *********************
 */
class CTypedFile : public dicom::io::CSimpleFile
{

protected :
    /**
     * Boolean for pre-swapped.
     */
    bool m_bLittleEndian;

public :
/**
 * Read one byte in the input file stream and move its offset.
 *********************
 * @pre input stream file loaded.
 *********************
 * @return uint8 : One byte (char) in the good endianess
 */
    /**
     * Initialize the data member. But not load the file
     *********************
     * @param _stringFullName [const std::string] : The fullname of the file
     * @param _bLittleEndian [const bool] : Flag to indicated the endianess
     */
	explicit				CTypedFile								( const std::string _stringFullName, const bool _bLittleEndian = true ) ;

    /**
     * Read one byte in the input file stream and move its offset.
     *********************
     * @pre input stream file loaded.
     *********************
     * @return const uint8 : One byte (char) in the good endianess
     */
    const uint8				ReadAndMove8							( void ) ;

	/**
	 * Read two byte in the input file stream and move its offset.
	 *
	 * Another way (but slower) :  
	 *		return ( (iChar[0] & 0x00FF) | ( (iChar[1] & 0x00FF) << 8 ) );
	 *		return ( ( (iChar[0] & 0x00FF) << 8 ) | (iChar[1] & 0x00FF) );
	 *********************
	 * @pre input stream file loaded.
	 *********************
	 * @return const uint16 : Two byte (integer) in the good endianess
	 */
    const uint16			ReadAndMove16							( void ) ;

	/**
	 * Read four byte in the input file stream and move its offset.
	 *
	 * Another way (but slower) :  
	 *		return  (
	 *						  ( iChar[0] & 0x000000FF )
	 *					|	( ( iChar[1] & 0x000000FF ) << 8  )
	 *					|	( ( iChar[2] & 0x000000FF ) << 16 )
	 *					|	( ( iChar[3] & 0x000000FF ) << 32 )
	 *				);
	 * and
	 *		return	(
	 *						( ( iChar[0] & 0x000000FF) << 32 )
	 *					|	( ( iChar[1] & 0x000000FF) << 16 )
	 *					|	( ( iChar[2] & 0x000000FF) << 8  )
	 *					|	  ( iChar[3] & 0x000000FF)
	 *				);
	 *********************
	 * @pre input stream file loaded.
	 *********************
	 * @return const uint32 : Four byte (long integer) in the good endianess
	 */
    const uint32			ReadAndMove32							( void ) ;

    /**
     * Read a buffer and move the offset of the input file stream.
     *********************
     * @pre buffer not null.
     *********************
     * @param _pBuff [int8 *] : buffer
     * @param _i32Length [const int32] : buffer's length
     */
    void					ReadAndMove								( char * _pBuff, const int32 _i32Length ) ;

    /**
     * @return const bool : A flag for the endianess
     */
    const bool				IsLittleEndian							( void ) const ;

private :

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 */
   	explicit				CTypedFile								( const CTypedFile & ) ;

	/**
	 * Not used.
	 */
    CTypedFile &			operator=								( const CTypedFile & ) ;

	//@}

};

}// end namespace io

}// end namespace dicom

#endif // TYPEDFILE_H
