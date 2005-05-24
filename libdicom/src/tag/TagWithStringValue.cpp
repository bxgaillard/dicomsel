#include "Virtuals/Type.h"

#include "visitor/AbstractDicomFile.h"
#include "io/DicomFile.h"

#include "tag/TagWithStringValue.h"

namespace dicom
{

namespace tag
{

////////////////////////////////////////////////////////////////////////////////
CTagWithStringValue::CTagWithStringValue(	const int32 _i32Group, const int32 _i32Element,
													const int32 _i32Length ):
    CDicomTag(_i32Group, _i32Element, _i32Length)
{}

////////////////////////////////////////////////////////////////////////////////
const uint32 CTagWithStringValue::SetValue( dicom::io::CDicomFile & _file)
{
	assert( _file.IsOpen() );
	assert( _file.GetOffset() + this->GetLength() < _file.GetSize() );   
	this->ReadMeIntoFile(_file);
	assert( _file.GetOffset() + this->GetLength() <= _file.GetSize() );
	return _file.GetOffset();
}

}// end namespace tag

}// end namespace
