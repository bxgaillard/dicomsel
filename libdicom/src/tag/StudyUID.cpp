// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/StudyUID.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CStudyUID::CStudyUID(	const int32 _i32Group, const int32 _i32Element,
						const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0020 );
	assert(_i32Element == 0x000d );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CStudyUID::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomStudyUIDTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CStudyUID::GetUID(void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
