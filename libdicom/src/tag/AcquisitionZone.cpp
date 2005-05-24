#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/AcquisitionZone.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CAcquisitionZone::CAcquisitionZone(	const int32 _i32Group, const int32 _i32Element,
									const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0008 );
	assert(_i32Element == 0x1030 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CAcquisitionZone::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomAcquisitionZoneTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CAcquisitionZone::GetZone(void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
