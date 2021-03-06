// Sexe.h
#ifndef DICOM_SEXE_TAG_H
#define DICOM_SEXE_TAG_H

#include "Virtuals/Type.h"
#include "TagWithStringValue.h"

namespace dicom
{

namespace tag
{

class CSexe : public CTagWithStringValue
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
	 * @pre Group == 0x0010 && Element 0x0040
	 *********************
	 * @see dicom::tag::CDicomTag
	 */
    explicit				CSexe									( const int32 _i32Group, const int32 _i32Element, const int32 _i32Length );

    const int8				GetSexePatient							( void ) const ;

	/**
	 * DP VISITOR.
	 * According to the visitor (image, list, etc...) the Time used or not.
	 * If this tag used by the visitor, the SetValue() function called.
	 *********************
	 * @param _pVisitor [dicom::visitor::CAbstractDicomFile * const] : Vistor
	 *********************
	 * @pre visitor not null
	 *********************
	 * @return const uint32 : final offset after read
	 */
    const uint32			Visit									( dicom::visitor::CAbstractDicomFile * const _pVisitor ) ;
};

}// end namespace tag

}// end namespace

#endif // DICOM_SEXE_TAG_H
