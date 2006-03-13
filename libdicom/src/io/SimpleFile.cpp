#include <sys/stat.h> // to use struct stat or _stat

#ifndef _WIN32 //assuming this means the os is unix
#include <unistd.h>
#endif

// Assertions
#include <cassert>

#include "Virtuals/Type.h"

#include "ExceptionDicom.h"
#include "io/SimpleFile.h"

namespace dicom
{

namespace io
{

///////////////////////////////////////////////////////////////
CSimpleFile::CSimpleFile( const std::string & _stringFullName ):
	m_stringFullName	( _stringFullName ),
    m_uiSize			( 0 )
{
    this->VerifyCreatePath();
}

///////////////////////////
CSimpleFile::~CSimpleFile()
{
    if ( this->m_iFile.is_open() ) this->m_iFile.close();
}

///////////////////////////////////////
void CSimpleFile::LoadInputFile( void )
{
    assert( !this->m_iFile.is_open() );

    this->m_iFile.open(this->m_stringFullName.c_str(), std::ios::binary);

	if(!this->m_iFile.is_open())
		throw CFileException(this->m_stringFullName,
							dicom::exception::StringException::FileNotFound);

	assert( this->m_iFile.is_open() );
    this->m_iFile.seekg(0,std::istream::end);			// Move at the file end
	this->m_uiSize = (uint32)this->m_iFile.tellg();		// Get the offset before the file begin
	this->m_iFile.seekg(0,std::istream::beg);			// Remove at the file begin

#if defined(_DEBUG) && defined(_CONSOLE)
	std::cout << "File '"<<this->m_stringFullName <<
				 "' ("<<this->m_uiSize*sizeof(uint8)<<" bytes) loaded ...\n";
#endif

	assert( this->m_iFile.is_open() );
	assert( std::istream::beg == this->m_iFile.tellg() );
}

///////////////////////////////////////////////////
const std::string& CSimpleFile::GetFullName( void ) const
{
	assert( this->m_stringFullName.length() > 0 );
    return this->m_stringFullName;
}

/////////////////////////////////////////
const uint32 CSimpleFile::GetSize( void ) const
{
    assert( (const_cast<CSimpleFile *>(this))->m_iFile.is_open() );
    return this->m_uiSize;
}

///////////////////////////////////////////
const uint32 CSimpleFile::GetOffset( void ) const
{
    assert( (const_cast<CSimpleFile *>(this))->m_iFile.is_open() );
    return (const_cast<CSimpleFile *>(this))->m_iFile.tellg();
}

//////////////////////////////////////
const bool CSimpleFile::IsOpen( void ) const
{
    return (const_cast<CSimpleFile *>(this))->m_iFile.is_open();
}

/////////////////////////////////////////////////////////
void CSimpleFile::SetOffset( const uint32 & _ui32Offset )
{
    assert( this->m_iFile.is_open() );
    //assert( _ui32Offset <= this->m_uiSize );
    if (_ui32Offset < this->m_uiSize)
    {
        this->m_iFile.seekg(_ui32Offset);
        //assert( _ui32Offset == this->m_iFile.tellg() );
    }
    else
    {
        this->m_iFile.seekg(this->m_uiSize);
    }
}

////////////////////////////////////////
void CSimpleFile::VerifyCreatePath(void) const
{
#ifdef _WIN32
    struct _stat buf;
    int bError = _stat(this->m_stringFullName.c_str(), &buf);
#else
    struct stat buf;
    int bError = stat(this->m_stringFullName.c_str(), &buf);
#endif


    if (bError == 0)
    {
#ifdef _WIN32
        if(!((buf.st_mode & _S_IFMT) != 0))
#else
        if(!((buf.st_mode & S_IFMT) != 0))
#endif
        {
            throw CFileException(this->m_stringFullName,
								dicom::exception::StringException::FileNotFound);
        }
    }
    else
    {
        throw CFileException(this->m_stringFullName,
							dicom::exception::StringException::FileNotFound);
    }
}

} // end namespace io

} // end namespace dicom
