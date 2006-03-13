// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/Rows.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CRows::CRows(	const int32 _i32Group, const int32 _i32Element,
				const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0028 );
	assert(_i32Element == 0x0010 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CRows::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomRowsTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const uint16 CRows::GetHeight(void) const
{
	return this->m_ui32Height;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CRows::SetValue( dicom::io::CDicomFile & _file)
{
    assert(this->m_iLength == 2);
    this->m_ui32Height  = _file.ReadAndMove16();

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
