// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/SliceThickness.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CSliceThickness::CSliceThickness(	const int32 _i32Group, const int32 _i32Element,
									const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0018 );
	assert(_i32Element == 0x0050 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CSliceThickness::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomSliceThicknessTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const double CSliceThickness::GetThickness(void) const
{
	return this->m_sSliceThickness;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CSliceThickness::SetValue( dicom::io::CDicomFile & _file)
{
    char* pi8Buff = new char[this->m_iLength+1]; assert(pi8Buff != NULL);
	_file.ReadAndMove(pi8Buff, this->m_iLength);
    pi8Buff[this->m_iLength]= '\0';

    this->m_sSliceThickness = strtod((char*)pi8Buff, (char **)NULL);

    delete[] pi8Buff;

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
