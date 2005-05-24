// DicomTagWithStringValue.h
#ifndef DICOM_TAGWITHSTRINGVALUE_TAG_H
#define DICOM_TAGWITHSTRINGVALUE_TAG_H

#include "Virtuals/Type.h"
#include "DicomTag.h"

#include <string>

namespace dicom
{

namespace tag
{

class CTagWithStringValue : public CDicomTag
{
public :
	/**
	 * Constructor.
	 * Call the root constructor.
	 *********************
	 * @param _i32Group [const int32]	: tag's group.
	 * @param _i32Element [const int32] : tag's element.
	 * @param _i32Length [const int32]	: tag's length.
	 *********************
	 * @pre Group == 0x0008 && Element 0x0022
	 *********************
	 * @see dicom::tag::CDicomTag
	 */
    explicit				CTagWithStringValue						( const int32 _i32Group, const int32 _i32Element, const int32 _i32Length );

	/**
	 * Read value and update position in input file.	 
	 *********************
	 * @param _File [dicom::io::CDicomFile &] : Full name of the parsed file	 
	 *********************
	 * @pre file's offset + tag's length < file's length.
	 * @pre file open.
	 * @post file's offset <= file's length.	 
	 ****************************************
	 * @exception std::exception for basic_istream::read	 
	 ****************************************
	 * @see dicom::tag::CDicomTag::ReadMeIntoFile	 
	 ****************************************
	 * @return const uint32 : final offset after read
	 */
	const uint32			SetValue								( dicom::io::CDicomFile & _File );
};

}// end namespace tag

}// end namespace

#endif // DICOM_TAGWITHSTRINGVALUE_TAG_H
