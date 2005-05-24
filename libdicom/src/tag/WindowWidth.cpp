#include "io/DicomFile.h"

#include "Virtuals/Type.h"

#include "tag/WindowWidth.h"

namespace dicom
{

namespace tag
{
/*
CWindowWidth::CWindowWidth(	const int32 _i32Group, const int32 _i32Element,
							const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length),
    m_bWindowWidthSpecified(true),
    m_dWindowWidth(0.0)
{}

CWindowWidth::~CWindowWidth()
{}

const uint32 CWindowWidth::Visit(dicom::visitor::CAbstractDicomFile* _file)
{
    // Nothing --> Not like used
	return _file.GetOffset();
}

const double CWindowWidth::GetWindowWidth(void) const
{
    assert(this->m_bWindowWidthSpecified);
    return this->m_dWindowWidth;
}

const bool CWindowWidth::isWindowWidthSpecified(void) const
{
	return this->m_bWindowWidthSpecified;
}

const uint32 CWindowWidth::SetValue( dicom::io::CDicomFile & _file)
{
//    this->ReadMeIntoFile(_file);
    char* pi8Buff = new char[this->m_iLength]; assert(pi8Buff != NULL);
	_file.ReadAndMove(pi8Buff, this->m_iLength);

    char* endptr;
    this->m_dWindowWidth = strtod(pi8Buff, &endptr);
    //check for a conversion error
    if (endptr == pi8Buff)
    {
        this->m_bWindowWidthSpecified = false;
        this->m_dWindowWidth = 0.0;
    }
    delete[] pi8Buff;

	return _file.GetOffset();
}
*/
}// end namespace tag

}// end namespace
