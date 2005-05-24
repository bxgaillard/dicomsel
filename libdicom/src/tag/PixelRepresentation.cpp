#include "Virtuals/Type.h"

#include "io/DicomFile.h"
#include "visitor/AbstractDicomFile.h"
#include "tag/PixelRepresentation.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CPixelRepresentation::CPixelRepresentation(	const int32 _i32Group, const int32 _i32Element,
											const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0028 );
	assert(_i32Element == 0x0103 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelRepresentation::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomPixelRepresentation(*this);
}

////////////////////////////////////////////////////////////////////////////////
const bool CPixelRepresentation::isUnsignedFlag(void) const
{
	return this->uiUnsignedFlag;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelRepresentation::SetValue( dicom::io::CDicomFile & _file)
{
    assert(this->m_iLength == 2);
    this->uiUnsignedFlag = (_file.ReadAndMove16())?false:true;

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
