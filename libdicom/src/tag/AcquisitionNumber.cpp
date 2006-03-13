// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/AcquisitionNumber.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CAcquisitionNumber::CAcquisitionNumber(	const int32 _i32Group, const int32 _i32Element,
										const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0020 );
	assert(_i32Element == 0x0012 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CAcquisitionNumber::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomAcquisitionNumberTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const int32 CAcquisitionNumber::GetNumber(void) const
{
	return this->m_i32AcquisitionIndex;
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CAcquisitionNumber::SetValue( dicom::io::CDicomFile & _file)
{
	assert( _file.IsOpen() );
	assert( _file.GetOffset() + this->GetLength() < _file.GetSize() );

    this->ReadMeIntoFile(_file);
    this->m_i32AcquisitionIndex = strtol((char*)(this->m_stringData.c_str()), (char **)NULL, 10);
	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
