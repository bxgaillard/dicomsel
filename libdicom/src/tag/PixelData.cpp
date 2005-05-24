#include "Virtuals/Type.h"

#include "io/DicomFile.h"
#include "visitor/AbstractDicomFile.h"
#include "tag/PixelData.h"

namespace dicom
{

namespace tag
{

    //int8* i8EndOfFile = &m_pBuffer[0] + ui32Size;
	//-------------------
	// Old Method
	//-------------------
    //read the actual pixel data
    //while(m_pBuffer < i8EndOfFile)
    //    *(m_pBuffer++) = _file->ReadAndMove8();
    //m_pBuffer-=ui32Size;
	//-------------------

////////////////////////////////////////////////////////////////////////////////
CPixelData::CPixelData(	const int32 _i32Group, const int32 _i32Element,
						const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length),
    m_bLastPicture	( true ),
	m_ui32Size		( 0 )
{
	assert(_i32Group == 0x7FE0 );
	assert(_i32Element == 0x0010 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelData::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomPixelDataTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const bool CPixelData::IsLastPicture(void) const
{
	return this->m_bLastPicture;
}

////////////////////////////////////////////////////////////////////////////////
char* CPixelData::GetBuffer() const
{
	assert(this->m_pBuffer != NULL);
	return this->m_pBuffer;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelData::SetValue( dicom::io::CDicomFile & _file)
{
    this->m_pBuffer = new char[this->m_ui32Size];
	assert(m_pBuffer != NULL);

	try
	{
		_file.ReadAndMove(m_pBuffer, this->m_ui32Size);
	}
	catch(const std::ios::failure & )
	{
		assert(true);
	}
	catch(const std::exception & )
	{
		assert(true);
	}
	catch(...)
	{
		assert(true);
	}

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
