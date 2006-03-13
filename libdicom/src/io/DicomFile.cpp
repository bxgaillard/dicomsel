// Assertions
#include <cassert>

// VIRTUALS basic type
#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "io/DicomFile.h"

namespace dicom
{

namespace io
{

#define SIMPLE_SIZE 256
#define DOUBLE_SIZE 65536
#define TRIPLE_SIZE 16777216

/////////////////////////////////////////////////////////
CDicomFile::CDicomFile( const std::string & _sFullName ):
    dicom::io::CTypedFile	( _sFullName, true ),
    m_bExplicitVR			( true )        //(default is implicit)
{}

////////////////////////////////////////////////////////////////////////////////////////
void CDicomFile::CheckByteOrder(int32& _i32Group, int32& _i32Element, int32& _i32Length)
{
    if (_i32Group != 0 && _i32Element == 0 && _i32Length == 4)
    {
        // OK
    }
    else if (!m_bLittleEndian && _i32Length == 0x04000000)
    {
        // Possible byte ordering problem - switching!"
        m_bLittleEndian   = true;
        this->SetOffset(0);
        this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
    }
    else if (m_bLittleEndian && _i32Length == 0x04000000)
    {
        // Possible byte ordering problem - switching!"
        m_bLittleEndian   = false;
        this->SetOffset(0);
        this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
    }
    else
    {
        // try DICOM part 10 i.e. a 128 byte file preamble
        // followed by "DICM"
        uint8 uiHeaderDICM[4];

        // skip the preamble - next 4 bytes should be "DICM"
        this->SetOffset(128);
        uiHeaderDICM[0]=this->ReadAndMove8();
        uiHeaderDICM[1]=this->ReadAndMove8();
        uiHeaderDICM[2]=this->ReadAndMove8();
        uiHeaderDICM[3]=this->ReadAndMove8();

        if (!IsHeaderDicom(uiHeaderDICM))
        {
            //it's not proper part 10 - try w/out
            //the 128 byte preamble
            this->SetOffset(0);
            uiHeaderDICM[0]=this->ReadAndMove8();
            uiHeaderDICM[1]=this->ReadAndMove8();
            uiHeaderDICM[2]=this->ReadAndMove8();
            uiHeaderDICM[3]=this->ReadAndMove8();
        }

        if (!IsHeaderDicom(uiHeaderDICM))
        {
            //Not a proper DICOM part 10 file
            if ( IsAcrNema2File(_i32Group, _i32Element, _i32Length) )
            {
                // Try to read at the file begin
                // Old dicom files are writing with that.
                this->SetOffset(0);
                this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
            }
            else
            {
                // Cannot read the file.
                // It's not a dicom file good formatted
                throw CFileException(this->m_stringFullName,
                                dicom::exception::StringException::FormatNotDefined);
            }
        }
        else
        {
            // This is a DICOM 3.0 file
            this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
            this->m_bExplicitVR = CheckLengthElement(_i32Length);

            //do we still have the byte ordering problem?
            if (!m_bLittleEndian && _i32Length == 0x04000000)
            {
                // Possible byte ordering problem - switching!"
                m_bLittleEndian = true;
                this->SetOffset(0);
                this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
            }
            else if (m_bLittleEndian && _i32Length == 0x04000000)
            {
                // Possible byte ordering problem - switching!"
                m_bLittleEndian = false;
                this->SetOffset(0);
                this->ReadHeaderLine(_i32Group, _i32Element, _i32Length);
            }

        }
    }
}

////////////////////////////////////////////////////////////
const bool CDicomFile::CheckLengthElement(int32& _i32Length)
{
    char* pvr = (char*)&_i32Length;
	bool bRes = false;

    // these explicit vr's have a 32-bit length. therefore,
    // we'll need to read another 16-bits &
    // then include these addition 16-bits in the current
    // 16-bit length.
    if ( IsVRExplicite32_1(pvr) )
    {
        _i32Length = this->ReadAndMove32();
        bRes = true;
    }
    // these explicit vr's have a 32-bit length. therefore,
    // we'll need to read another 16-bits &
    // then include these addition 16-bits in
    // the current 16-bit length.
    else if ( IsVRExplicite32_2(pvr) )
    {
        _i32Length = this->ReadAndMove32();
        bRes = true;
    }
    // these explicit vr's have a 16-bit length.
    // this allows them to fit into the same space as implicit vr.
    else if ( IsVRExplicite16_1(pvr) )
    {
        _i32Length &= 0xffff0000;
        _i32Length >>= 16;
        bRes = true;
    }
    // these explicit vr's have a 16-bit length.
    // this allows them to fit into the same space as implicit vr.
    else if ( IsVRExplicite16_2(pvr) )
    {
        _i32Length = 256*pvr[0] + pvr[1];
        bRes = true;
    }
    return bRes;
}

////////////////////////////////////////////////////////////////////////////
void CDicomFile::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
    assert(_pVisitor!=NULL);
    _pVisitor->VisitDicomFile(*this);
}

////////////////////////////////////////////////////////////
void CDicomFile::ReadTag( int32& _i32Group, int32& _i32Element,
                          int32& _i32Length, bool& _bFirstOne)
{
    ReadHeaderLine(_i32Group, _i32Element, _i32Length);

    // check and see if assumed byte order is not correct
    // & fix if necessary
    if ( _bFirstOne )
    {
        CheckByteOrder(_i32Group, _i32Element, _i32Length);
        _bFirstOne = false;
    }
    else //this isn't the first one so check the value representation
    {
        if ( IsExpliciteVR() )
        {
            CheckLengthElement(_i32Length);
        }
    }
}

//////////////////////////////////////////
const bool CDicomFile::IsExpliciteVR(void) const
{
    return this->m_bExplicitVR;
}

////////////////////////////////////////////////////////////////
const bool CDicomFile::IsHeaderDicom(const uint8 * _ui8HeaderDICM) const
{
	//assert( _ui8HeaderDICM must be a tab[4] -> It's ok );
    return (
            _ui8HeaderDICM[0] == 'D' && _ui8HeaderDICM[1] == 'I' &&
            _ui8HeaderDICM[2] == 'C' && _ui8HeaderDICM[3] == 'M'
           );
}

////////////////////////////////////////////////////////////
const bool CDicomFile::IsAcrNema2File(const int32& _i32Group,
                                      const int32& _i32Element,
                                      const int32& _i32Length) const
{
    return (
            (_i32Group == 0 && _i32Element == 0 && _i32Length == 4) ||
            (_i32Group == 8 && _i32Element == 1) ||
            (_i32Group == 8 && _i32Element == 5) ||
            (_i32Group == 8 && _i32Element == 8) ||
            (_i32Group == 8 && _i32Element == 0x16)
           );
}

////////////////////////////////////////////////////////////////////
const bool CDicomFile::IsVRExplicite32_1 (const char * const _pBuff)
{
	//assert( ::strlen(_pBuff) == 2 );
	return (
             (_pBuff[0]=='O' && _pBuff[1]=='B') ||
             (_pBuff[0]=='O' && _pBuff[1]=='W') ||
             (_pBuff[0]=='S' && _pBuff[1]=='Q')	||
			 (_pBuff[0]=='U' && _pBuff[1]=='N')
           );
}

////////////////////////////////////////////////////////////////////
const bool CDicomFile::IsVRExplicite32_2 (const char * const _pBuff)
{
	//assert( ::strlen(_pBuff) == 4 );
	return (
             (_pBuff[3]=='O' && _pBuff[2]=='B') ||
             (_pBuff[3]=='O' && _pBuff[2]=='W') ||
             (_pBuff[3]=='S' && _pBuff[2]=='Q')	||
			 (_pBuff[3]=='U' && _pBuff[2]=='N')
           );
}

////////////////////////////////////////////////////////////////////
const bool CDicomFile::IsVRExplicite16_1 (const char * const _pBuff)
{
	//assert( ::strlen(_pBuff) == 2 );
	return (
              (_pBuff[0]=='A' && _pBuff[1]=='E') ||
              (_pBuff[0]=='A' && _pBuff[1]=='S') ||
              (_pBuff[0]=='A' && _pBuff[1]=='T') ||
              (_pBuff[0]=='C' && _pBuff[1]=='S') ||
              (_pBuff[0]=='D' && _pBuff[1]=='A') ||
              (_pBuff[0]=='D' && _pBuff[1]=='S') ||
              (_pBuff[0]=='D' && _pBuff[1]=='T') ||
              (_pBuff[0]=='F' && _pBuff[1]=='L') ||
              (_pBuff[0]=='F' && _pBuff[1]=='D') ||
              (_pBuff[0]=='I' && _pBuff[1]=='S') ||
              (_pBuff[0]=='L' && _pBuff[1]=='O') ||
              (_pBuff[0]=='L' && _pBuff[1]=='T') ||
              (_pBuff[0]=='P' && _pBuff[1]=='N') ||
              (_pBuff[0]=='S' && _pBuff[1]=='H') ||
              (_pBuff[0]=='S' && _pBuff[1]=='L') ||
              (_pBuff[0]=='S' && _pBuff[1]=='S') ||
              (_pBuff[0]=='S' && _pBuff[1]=='T') ||
              (_pBuff[0]=='T' && _pBuff[1]=='M') ||
              (_pBuff[0]=='U' && _pBuff[1]=='I') ||
              (_pBuff[0]=='U' && _pBuff[1]=='L') ||
              (_pBuff[0]=='U' && _pBuff[1]=='S')
            );
}

////////////////////////////////////////////////////////////////////
const bool CDicomFile::IsVRExplicite16_2 (const char * const _pBuff)
{
	//assert( ::strlen(_pBuff) == 4 );
	return (
              (_pBuff[3]=='A' && _pBuff[2]=='E') ||
              (_pBuff[3]=='A' && _pBuff[2]=='S') ||
              (_pBuff[3]=='A' && _pBuff[2]=='T') ||
              (_pBuff[3]=='C' && _pBuff[2]=='S') ||
              (_pBuff[3]=='D' && _pBuff[2]=='A') ||
              (_pBuff[3]=='D' && _pBuff[2]=='S') ||
              (_pBuff[3]=='D' && _pBuff[2]=='T') ||
              (_pBuff[3]=='F' && _pBuff[2]=='L') ||
              (_pBuff[3]=='F' && _pBuff[2]=='D') ||
              (_pBuff[3]=='I' && _pBuff[2]=='S') ||
              (_pBuff[3]=='L' && _pBuff[2]=='O') ||
              (_pBuff[3]=='L' && _pBuff[2]=='T') ||
              (_pBuff[3]=='P' && _pBuff[2]=='N') ||
              (_pBuff[3]=='S' && _pBuff[2]=='H') ||
              (_pBuff[3]=='S' && _pBuff[2]=='L') ||
              (_pBuff[3]=='S' && _pBuff[2]=='S') ||
              (_pBuff[3]=='S' && _pBuff[2]=='T') ||
              (_pBuff[3]=='T' && _pBuff[2]=='M') ||
              (_pBuff[3]=='U' && _pBuff[2]=='I') ||
              (_pBuff[3]=='U' && _pBuff[2]=='L') ||
              (_pBuff[3]=='U' && _pBuff[2]=='S')
           );
}

////////////////////////////////////////////////////////////////////
void CDicomFile::ReadHeaderLine(int32& _i32Group, int32& _i32Element,
                                int32& _i32Length)
{
    assert( m_iFile.is_open() );
    static char iChar[8];
    this->m_iFile.read(iChar, 8); // if 8*sizeof(int8) == 8 (16+16+32)

	if( this->m_bLittleEndian )
	{
		_i32Group   =	static_cast<uint8>(iChar[0]) +
						static_cast<uint8>(iChar[1])*SIMPLE_SIZE;
		_i32Element =	static_cast<uint8>(iChar[2]) +
						static_cast<uint8>(iChar[3])*SIMPLE_SIZE;
		_i32Length  =	static_cast<uint8>(iChar[4]) +
						static_cast<uint8>(iChar[5])*SIMPLE_SIZE +
						static_cast<uint8>(iChar[6])*DOUBLE_SIZE +
						static_cast<uint8>(iChar[7])*TRIPLE_SIZE;
	}
	else
	{
		_i32Group   =	static_cast<uint8>(iChar[0])*SIMPLE_SIZE +
						static_cast<uint8>(iChar[1]);
		_i32Element =	static_cast<uint8>(iChar[2])*SIMPLE_SIZE +
						static_cast<uint8>(iChar[3]);
		_i32Length  =	static_cast<uint8>(iChar[4])*TRIPLE_SIZE +
						static_cast<uint8>(iChar[5])*DOUBLE_SIZE +
						static_cast<uint8>(iChar[6])*SIMPLE_SIZE +
						static_cast<uint8>(iChar[7]);
	}
}

} // end namespace io

} // end namespace dicom
