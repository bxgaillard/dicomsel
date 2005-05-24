#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/BitsAllocated.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CBitsAllocated::CBitsAllocated(	const int32 _i32Group, const int32 _i32Element,
								const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0028 );
	assert(_i32Element == 0x0100 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CBitsAllocated::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomBitsAllocatedTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const uint8 CBitsAllocated::GetBitsPerPixel(void) const
{
	return this->uiBytesPerPixel;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CBitsAllocated::SetValue( dicom::io::CDicomFile & _file)
{
	assert(this->m_iLength == 2);
	assert( _file.IsOpen() );
	assert( _file.GetOffset() + this->GetLength() < _file.GetSize() );

	this->uiBytesPerPixel  = static_cast<uint8>(_file.ReadAndMove16()); // 8 or 16 bits
	assert( this->uiBytesPerPixel == 8 || this->uiBytesPerPixel == 16 );

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
