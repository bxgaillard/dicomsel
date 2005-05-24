/**
 * Filename: DicomAbstractFactory.cpp
 *********************
 * Namespace : dicom
 *********************
 * Description: .
 */

//all
#include "tag/DicomTag.h"
//0x0002
#include "tag/TransfertSyntax.h"
//0x0008
#include "tag/ImageType.h"
#include "tag/Modality.h"
#include "tag/Hospital.h"
#include "tag/AcquisitionZone.h"
#include "tag/AcquisitionDate.h"
#include "tag/AcquisitionTime.h"
//0x0010
#include "tag/Name.h"
#include "tag/PatientID.h"
#include "tag/Birthday.h"
#include "tag/Sexe.h"
// 0x0018
#include "tag/SliceThickness.h"
//0x0020
#include "tag/StudyUID.h"
#include "tag/SeriesUID.h"
#include "tag/AcquisitionNumber.h"
#include "tag/ImagePosition.h"
//0x0028
#include "tag/Rows.h"
#include "tag/PixelSpacing.h"
#include "tag/Columns.h"
#include "tag/BitsAllocated.h"
#include "tag/PixelRepresentation.h"
//0X07f0
#include "tag/PixelData.h"

#include "io/DicomFile.h"
#include "visitor/AbstractDicomFile.h"
#include "visitor/CreateImage.h"

#include "DicomAbstractFactory.h"

#ifdef _DEBUG
#define VERIF_GROUP_ORDER(X)	dicom::visitor::CDicom::s_ui32GroupOrder |= X ; \
								assert ((dicom::visitor::CDicom::s_ui32GroupOrder & X) == X) ;
#endif

namespace dicom
{

tag::CDicomTag* CDicomAbstractFactory::CreateTag(const int32 _group, const int32 _element, const int32 _length,
                                                 dicom::io::CDicomFile & /*_file*/, dicom::visitor::CDicom & _visitor)
{
	dicom::tag::CDicomTag * pTag = NULL;

#ifdef _DEBUG
	static const bitGroup0x0002		= 0x01; // 0000 0001
	static const bitGroup0x0008		= 0x02; // 0000 0010
	static const bitGroup0x0010		= 0x04; // 0000 0100
	static const bitGroup0x0018		= 0x08; // 0000 1000
	static const bitGroup0x0020		= 0x10; // 0001 0000
	static const bitGroup0x0028		= 0x20; // 0010 0000

	static const SigmaGroup0x0002	= 0xFF; // 1111 1111
	static const SigmaGroup0x0008	= 0xFE; // 1111 1110
	static const SigmaGroup0x0010	= 0xFC; // 1111 1100
	static const SigmaGroup0x0018	= 0xF8; // 1111 1000
	static const SigmaGroup0x0020	= 0xF0; // 1111 0000
	static const SigmaGroup0x0028	= 0xE0; // 1110 0000
#endif

    switch (_group)
    {
        case 0x0002 :
            switch (_element)
            {
                case 0x10 :  pTag = new tag::CTransfertSyntax(_group, _element, _length );     break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0002
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0002)
#endif
            break;

        case 0x0008 :
            switch (_element)
            {
                case 0x08 :  pTag = new tag::CImageType(_group, _element, _length );           break;
                case 0x22 :  pTag = new tag::CAcquisitionDate(_group, _element, _length );     break;
                case 0x32 :  pTag = new tag::CAcquisitionTime(_group, _element, _length );     break;
                case 0x60 :  pTag = new tag::CModality(_group, _element, _length );			break;
                case 0x80 :  pTag = new tag::CHospital(_group, _element, _length );            break;
                case 0x1030 :  pTag = new tag::CAcquisitionZone(_group, _element, _length );   break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0008
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0008)
#endif
            break;

		case 0x0010 :
            switch (_element)
            {
                case 0x10 :  pTag = new tag::CName(_group, _element, _length );                break;
                case 0x20 :  pTag = new tag::CPatientID(_group, _element, _length );           break;
                case 0x30 :  pTag = new tag::CBirthday(_group, _element, _length );            break;
                case 0x40 :  pTag = new tag::CSexe(_group, _element, _length );                break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0010
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0010)
#endif
            break;

        case 0x0018 :
            switch (_element)
            {
                case 0x50 :  pTag = new tag::CSliceThickness(_group, _element, _length );	  break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0018
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0018)
#endif
            break;

        case 0x0020 :
            switch (_element)
            {
                case 0x0d :  pTag = new tag::CStudyUID(_group, _element, _length );            break;
                case 0x0e :  pTag = new tag::CSeriesUID(_group, _element, _length );           break;
                case 0x12 :  pTag = new tag::CAcquisitionNumber(_group, _element, _length );   break;
                case 0x32 :  pTag = new tag::CImagePosition(_group, _element, _length );       break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0020
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0020)
#endif
            break;

        case 0x0028 :
            switch (_element)
            {
                case 0x10 :  pTag = new tag::CRows(_group, _element, _length );                break;
                case 0x11 :  pTag = new tag::CColumns(_group, _element, _length );			break;
                case 0x30 :  pTag = new tag::CPixelSpacing(_group, _element, _length );        break;
                case 0x0100: pTag = new tag::CBitsAllocated(_group, _element, _length );       break;
                case 0x0103: pTag = new tag::CPixelRepresentation(_group, _element, _length ); break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } //case 0x0028
#ifdef _DEBUG
			VERIF_GROUP_ORDER(bitGroup0x0028)
#endif
            break;

        case 0x7FE0 :
            switch (_element)
            {
				case 0x10 :
							_visitor.SetPicturePresent(true);
                                 pTag = new tag::CPixelData(_group, _element, _length );
                             break;
                default :    pTag = new tag::CDicomTag(_group, _element, _length);                    break;
            } // case 0x7Fe0
            break;

        default : pTag = new tag::CDicomTag(_group, _element, _length);                               break;
    }

	assert(pTag != NULL);

    return pTag;
}

}// end namespace
