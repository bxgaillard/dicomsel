/** \file TypedFile.cpp
 *********************
 * Namespace : dicom
 *********************
 * This class enclose all functions of CSimpleFile,
 * add read function and the notion of endianess.
 */

#include "Virtuals/Type.h"

#include "io/SimpleFile.h"
#include "io/TypedFile.h"

namespace dicom
{

namespace io
{

#define SIMPLE_SIZE 256
#define DOUBLE_SIZE 65536
#define TRIPLE_SIZE 16777216

//////////////////////////////////////////////////////////
CTypedFile::CTypedFile( const std::string _stringFullName, 
						const bool _bLittleEndian ):
    dicom::io::CSimpleFile	( _stringFullName ),
    m_bLittleEndian			( _bLittleEndian )
{}

////////////////////////////////////////////
const uint8 CTypedFile::ReadAndMove8( void )
{
    assert( m_iFile.is_open() );
    static char iChar;
    this->m_iFile.read(&iChar, 1); // if sizeof(int8) == 1
    return iChar;
}

//////////////////////////////////////////////
const uint16 CTypedFile::ReadAndMove16( void )
{
    assert( m_iFile.is_open() );
    static char iChar[2];
    this->m_iFile.read(iChar, 2); // if 2*sizeof(int8) == 2

    if(this->m_bLittleEndian)
		return static_cast<uint8>(iChar[0]) +
               static_cast<uint8>(iChar[1])*SIMPLE_SIZE;
	else
		return static_cast<uint8>(iChar[0])*SIMPLE_SIZE +
               static_cast<uint8>(iChar[1]);
}

//////////////////////////////////////////////
const uint32 CTypedFile::ReadAndMove32( void )
{
    assert( m_iFile.is_open() );
    static char iChar[4];
    this->m_iFile.read(iChar, 4); // if 4*sizeof(int8) == 4

	if ( this->m_bLittleEndian )
		return static_cast<uint8>(iChar[0]) +
               static_cast<uint8>(iChar[1])*SIMPLE_SIZE +
               static_cast<uint8>(iChar[2])*DOUBLE_SIZE +
               static_cast<uint8>(iChar[3])*TRIPLE_SIZE;
	else
		return static_cast<uint8>(iChar[0])*TRIPLE_SIZE +
               static_cast<uint8>(iChar[1])*DOUBLE_SIZE +
               static_cast<uint8>(iChar[2])*SIMPLE_SIZE +
               static_cast<uint8>(iChar[3]);
}

/////////////////////////////////////////////////////////////////////
void CTypedFile::ReadAndMove( char * _pBuff, const int32 _i32Length )
{
    assert( m_iFile.is_open() );
    assert( _pBuff!=NULL );
    this->m_iFile.read(_pBuff, _i32Length); // if sizeof(int8) == 1
}

///////////////////////////////////////////////////
const bool CTypedFile::IsLittleEndian( void ) const
{
    return this->m_bLittleEndian;
}

} // end namespace io

} // end namespace dicom
