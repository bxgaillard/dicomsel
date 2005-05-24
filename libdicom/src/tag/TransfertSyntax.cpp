#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "io/DicomFile.h"

#include "tag/TransfertSyntax.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CTransfertSyntax::CTransfertSyntax(	const int32 _i32Group, const int32 _i32Element,
									const int32 _i32Length ):
    CTagWithStringValue(_i32Group, _i32Element, _i32Length)
{
	assert(_i32Group == 0x0002 );
	assert(_i32Element == 0x0010 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CTransfertSyntax::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomTransfertSyntax(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& CTransfertSyntax::GetTransfertSyntax(void) const
{
	return this->m_stringData;
}

}// end namespace tag

}// end namespace
