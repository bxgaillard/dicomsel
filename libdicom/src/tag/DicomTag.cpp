#include "Virtuals/Type.h"

// used to visit a tag
#include "visitor/AbstractDicomFile.h"
// tag used
#include "tag/DicomTag.h"
#include "tag/PixelData.h"
//
#include "io/DicomFile.h"

namespace dicom
{

namespace tag
{

#ifdef _DEBUG
CDicomTag::CDicomTag(	const int32 _i32Group, const int32 _i32Element, const int32 _i32Length )
#else
CDicomTag::CDicomTag(	const int32, const int32, const int32 _i32Length )
#endif
:
    m_iLength	( _i32Length )
#ifdef _DEBUG
    ,m_iGroup	( _i32Group )
    ,m_iElement	( _i32Element )
#ifdef _DEBUG_CONSOLE_VERBOSE
    ,m_sName(dicom::CDicomDictionary::GetNameElement(_i32Group, _i32Length))
    ,m_sBuffer	( _i32Length )
    ,m_ssBuffer	( m_sBuffer )
	,m_dtTypes	( dicom::CDicomDictionary::GetType(_i32Group, _i32Element) )
#endif
#endif
{}

CDicomTag::~CDicomTag()
{}

const uint32 CDicomTag::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitUnknownDicomTag(*this);
}

#ifdef _DEBUG_CONSOLE_VERBOSE

const std::string& CDicomTag::GetName(void) const
{
    return this->m_sName;
}
const dicom::CDicomDictionary::sDicomTypes CDicomTag::GetType(void) const
{
    return this->m_dtTypes;
}

#endif

#ifdef _DEBUG
const int32 CDicomTag::GetGroup(void) const
{
    return this->m_iGroup;
}

const int32 CDicomTag::GetElement(void) const
{
    return this->m_iElement;
}
#endif

const int32 CDicomTag::GetLength(void) const
{
    return this->m_iLength;
}

void CDicomTag::ReadMeIntoFile( dicom::io::CDicomFile & _File )
{
	assert( _File.IsOpen() );
	assert( _File.GetOffset() + this->GetLength() < _File.GetSize() );

    char* pi8Buff = new char[this->m_iLength+1]; 
	assert(pi8Buff != NULL);

	_File.ReadAndMove(pi8Buff, this->m_iLength);
    pi8Buff[this->m_iLength]= '\0';
    this->m_stringData = pi8Buff;
    delete[] pi8Buff;

#ifdef _DEBUG_CONSOLE_VERBOSE
	this->m_ssBuffer << this->m_stringData;
#endif
}

#ifdef _DEBUG_CONSOLE_VERBOSE
const std::string CDicomTag::GetValue(void) const
{
    return this->m_ssBuffer.str();
}
#endif

const uint32 CDicomTag::SetValue(dicom::io::CDicomFile & _File)
{
	uint32 ui32Pos = (uint32) -1;
    if (this->m_iLength>0 //&&
        //(typeid(*this) != typeid(dicom::tag::CPixelData) && ((dicom::tag::CPixelData*)this)->IsLastPicture()) // If the picture tag
        )
    {
#ifdef _DEBUG_CONSOLE_VERBOSE
        int8* pBuffer = new int8[this->m_iLength];  assert(pBuffer != NULL);
        _File.ReadAndMove(pBuffer, this->m_iLength);
        switch (this->m_dtTypes)
        {
            case CDicomDictionary::unknown :
                switch (this->m_iLength)
                {
                    case 1 :
                    {
                        int iValGood = (unsigned int) pBuffer[0];
                        this->m_ssBuffer << iValGood;
                    }
                    break;
                    case 2 :
                    {
                        int iValGood = ((_File.IsLittleEndian()))?pBuffer[0] + 256*pBuffer[1]:pBuffer[0]*256 + pBuffer[1];
                        this->m_ssBuffer << iValGood;
                    }
                    break;
                    case 4 :
                    {
                        int iValGood = (_File.IsLittleEndian())?
                            pBuffer[0] + 256*pBuffer[1] + 256*256*pBuffer[2] + 256*256*256*pBuffer[3]:
                            pBuffer[0]*256*256*256 + pBuffer[1]*256*256 + pBuffer[2]*256 + pBuffer[3];
                        this->m_ssBuffer << iValGood;
                    }
                    break;
                    default:
                    {
                        for (int iter = 0; iter < this->m_iLength; iter++)
                        {
                              if (isprint(pBuffer[iter]))   this->m_ssBuffer << pBuffer[iter];
                              else                          this->m_ssBuffer << '.';
                        }
                    }
                    break;
                }
                break;
            case CDicomDictionary::ui8     :
                {
                    for (int32 iter = 0; iter < this->m_iLength; iter++)  this->m_ssBuffer << (pBuffer[iter]&0xff);
                }
            break;
            case CDicomDictionary::i8  :
            case CDicomDictionary::i16 :
            case CDicomDictionary::i32 :
            case CDicomDictionary::ui16:
            case CDicomDictionary::ui32:
            case CDicomDictionary::string:
                {
                    for (int32 iter = 0; iter < this->m_iLength; iter++)  this->m_ssBuffer << pBuffer[iter];
                }
                break;
        }  //end switch tag type
        delete[] pBuffer;
		ui32Pos = _File.GetOffset();
#else
		ui32Pos = _File.GetOffset()+this->m_iLength;
		_File.SetOffset(ui32Pos);
#endif
   }
    else
    {
#ifdef _DEBUG_CONSOLE_VERBOSE
        this->m_ssBuffer << "-1";
#endif
		ui32Pos = _File.GetOffset();
    }
    return ui32Pos;
}

}// end namespace tag

}// end namespace
