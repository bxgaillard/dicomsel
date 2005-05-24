#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/SeriesUID.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CSeriesUID::CSeriesUID(	const int32 _i32Group, const int32 _i32Element,
						const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0020 );
	assert(_i32Element == 0x000e );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CSeriesUID::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomSeriesUIDTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CSeriesUID::GetUID(void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
