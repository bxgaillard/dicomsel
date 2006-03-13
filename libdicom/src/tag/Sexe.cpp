// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/Sexe.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CSexe::CSexe(	const int32 _i32Group, const int32 _i32Element,
				const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0010 );
	assert(_i32Element == 0x0040 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CSexe::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomSexeTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const int8 CSexe::GetSexePatient(void) const
{
	return this->m_stringData[0];
}

}// end namespace tag

}// end namespace
