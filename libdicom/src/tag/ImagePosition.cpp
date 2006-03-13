// Assertions
#include <cassert>

#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "tag/ImagePosition.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CImagePosition::CImagePosition(		const int32 _i32Group, const int32 _i32Element,
									const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length),
    m_vImagePosition(std::vector<double>(3))
{
	assert(_i32Group == 0x0020 );
	assert(_i32Element == 0x0032 );

}

////////////////////////////////////////////////////////////////////////////////
const uint32 CImagePosition::Visit(dicom::visitor::CAbstractDicomFile * const _pVisitor)
{
	assert(_pVisitor != NULL);
    return _pVisitor->VisitDicomImagePositionTag(*this);
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<double> CImagePosition::GetPosition(void) const
{
	return this->m_vImagePosition;
}

////////////////////////////////////////////////////////////////////////////////
const double CImagePosition::GetPositionX(void) const
{
	return this->m_vImagePosition[0];
}

////////////////////////////////////////////////////////////////////////////////
const double CImagePosition::GetPositionY(void) const
{
	return this->m_vImagePosition[1];
}

////////////////////////////////////////////////////////////////////////////////
const double CImagePosition::GetPositionZ(void) const
{
	return this->m_vImagePosition[2];
}

////////////////////////////////////////////////////////////////////////////////
const uint32 CImagePosition::SetValue( dicom::io::CDicomFile & _file)
{
    char* pi8Buff = new char[this->m_iLength+1]; assert(pi8Buff != NULL);
	_file.ReadAndMove(pi8Buff, this->m_iLength);
    pi8Buff[this->m_iLength]= '\0';

    m_vImagePosition[0] = 10000.0;
    m_vImagePosition[1] = 10000.0;
    m_vImagePosition[2] = 10000.0;
    for (uint8 j=0, k=0;j<3;j++,k++)
    {
        std::string buffTemp("");
        while(pi8Buff[k]!='\\' && k<strlen(pi8Buff))
        {
            buffTemp += pi8Buff[k];
            k++;
        }
        this->m_vImagePosition[j] = strtod(/**(char*)*/buffTemp.c_str(), (char **)NULL);
    }
    assert(m_vImagePosition[0] != 10000.0 && m_vImagePosition[1] != 10000.0 && m_vImagePosition[2] != 10000.0);
    delete[] pi8Buff;

	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
