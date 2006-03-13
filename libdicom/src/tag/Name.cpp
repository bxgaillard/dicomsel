// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/Name.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CName::CName(	const int32 _i32Group, const int32 _i32Element,
				const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0010 );
	assert(_i32Element == 0x0010 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CName::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomNameTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CName::GetNamePatient(void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
