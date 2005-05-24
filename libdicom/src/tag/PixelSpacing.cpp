#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/PixelSpacing.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CPixelSpacing::CPixelSpacing(	const int32 _i32Group, const int32 _i32Element,
								const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length),
    m_vPixelSpacing(std::vector<double>(2))
{
	assert(_i32Group == 0x0028 );
	assert(_i32Element == 0x0030 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelSpacing::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomPixelSpacingTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<double> CPixelSpacing::GetSpacing(void) const
{
	return this->m_vPixelSpacing;
}

////////////////////////////////////////////////////////////////////////////////
const double CPixelSpacing::GetSpacingHeight(void) const
{
	return this->m_vPixelSpacing[0];
}

////////////////////////////////////////////////////////////////////////////////
const double CPixelSpacing::GetSpacingWidth(void) const
{
	return this->m_vPixelSpacing[1];
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CPixelSpacing::SetValue( dicom::io::CDicomFile & _file)
{
    char* pi8Buff = new char[this->m_iLength+1]; assert(pi8Buff != NULL);
	_file.ReadAndMove(pi8Buff, this->m_iLength);
    pi8Buff[this->m_iLength]= '\0';

    m_vPixelSpacing[0] = 10000.0;
    m_vPixelSpacing[1] = 10000.0;
    for (uint8 j=0, k=0;j<2;j++,k++)
    {
        std::string buffTemp("");
        while(pi8Buff[k]!='\\' && k<strlen(pi8Buff))
        {
            buffTemp += pi8Buff[k];
            k++;
        }
        this->m_vPixelSpacing[j] = strtod(/**(char*)*/buffTemp.c_str(), (char **)NULL);
    }
    assert(m_vPixelSpacing[0] != 10000.0 && m_vPixelSpacing[1] != 10000.0);
    delete[] pi8Buff;

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
