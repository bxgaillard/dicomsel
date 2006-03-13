// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/AcquisitionDate.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CAcquisitionDate::CAcquisitionDate(	const int32 _i32Group, const int32 _i32Element,
									const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0008 );
	assert(_i32Element == 0x0022 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CAcquisitionDate::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomAcquisitionDateTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CAcquisitionDate::GetAcquisitionDate  (void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
