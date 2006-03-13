// VIRTUALS basic type
#include "Virtuals/Type.h"

// STL's stream
#include <iostream>
#include <exception>

// Assertions
#include <cassert>

//      TAG     //
//////////////////
// all
#include "tag/DicomTag.h"
// 0x0002
#include "tag/TransfertSyntax.h"
// 0x0008
#include "tag/ImageType.h"
#include "tag/Modality.h"
#include "tag/Hospital.h"
#include "tag/AcquisitionZone.h"
#include "tag/AcquisitionDate.h"
#include "tag/AcquisitionTime.h"
// 0x0010
#include "tag/Name.h"
#include "tag/PatientID.h"
#include "tag/Birthday.h"
#include "tag/Sexe.h"
// 0x0018
#include "tag/SliceThickness.h"
// 0x0020
#include "tag/StudyUID.h"
#include "tag/SeriesUID.h"
#include "tag/AcquisitionNumber.h"
#include "tag/ImagePosition.h"
// 0x0028
#include "tag/Rows.h"
#include "tag/PixelSpacing.h"
#include "tag/Columns.h"
#include "tag/BitsAllocated.h"
#include "tag/PixelRepresentation.h"
// 0X07f0
#include "tag/PixelData.h"
//////////////////

// IO
#include "io/DicomFile.h"
#include "io/SimpleDirectory.h"
// Base file
#include "Virtuals/mfo/mfo.h"
// Factory
#include "DicomAbstractFactory.h"
//
#include "visitor/CreateImage.h"
#include "lib/DicomLib.h"

namespace dicom
{

namespace visitor
{

/**
 *
 * Remove all ending spaces in a string
 *
 * @return const std::string : the treaten string
 *
 **/
const std::string TrimRight (const std::string &_sStr)
{
	std::string sRet = _sStr;

	int iIndex = sRet.length ();

	if (iIndex != 0)
	{
		iIndex --;

		while (iIndex >= 0 && sRet[iIndex] == ' ')
			iIndex --;

		iIndex ++;
		if (sRet[iIndex] == ' ')
			sRet.erase (iIndex, sRet.length () - iIndex);
	}

	return sRet;
}

////////////////////////////////////////////////////////////////////////////////////////////
CCreateImage::CCreateImage(const std::string& _sOutputFilename, mfo::CAcquisition * const _pAcquisition):
    m_bUnsignedFlag     (false),
	m_dSpacingBetweenSlicesSupposed(0.0),
    m_dSliceThickness	(0.0),
    m_ui32Height        (0),
    m_ui32Width         (0),
    m_ui8BitsPerPixel   (8),
	m_uiFileNumber		(0),
	m_uiNumberSlices	(0),
    m_pBuffer           (NULL),
	m_sModality			(""),
    m_sOutputFilename   (_sOutputFilename),
    m_vectorPixelSize	(std::vector<double>(2)),
	m_vPosition			(3),
    m_pAcquisition      (_pAcquisition)
{
   this-> m_iterFrame = _pAcquisition->GetFrameList()->begin();
   this->m_vectorPixelSize[0] = 0.0;
   this->m_vectorPixelSize[1] = 0.0;

   // Post-condition
   assert(this->m_pBuffer					== NULL	) ;
   assert(this->m_iterFrame					!= NULL	) ;
   assert(this->m_pAcquisition				!= NULL	) ;
   assert(this->m_sOutputFilename.size()	> 0		) ;
   assert(this->m_vectorPixelSize.size()	== 2	) ;

   const std::string stingModality = TrimRight(this->m_pAcquisition->GetStudy()->GetModality());

	// Modality ok ?
	if	(
				dicom::CSettingsDicom::s_bUseModality
			&&	stingModality != "CT"
			&&	stingModality != "MR"
			&&	stingModality != "PT"
		)
	{
		throw dicom::visitor::CDicom::CVisitorException(this->m_pFile->GetFullName(),
											dicom::exception::StringException::ModalityNotManaged
											+ this->m_sModality);
	}

	// Picture format ok ?
    //if (strcmp(this->m_stringTransfertSyntax.c_str(),"1.2.840.10008.1.2.2")==0 &&little_endian)
    //throw DCM_exception(PAS_DICOM);
    //if (strcmp(this->m_stringTransfertSyntax.c_str(),"1.2.840.10008.1.2.5") == 0)
    //throw DCM_exception(RLE_COMPR);
    //if (    (strcmp(this->m_stringTransfertSyntax.c_str(),"1.2.840.10008.1.2.4.50") == 0) ||
      //      (strcmp(this->m_stringTransfertSyntax.c_str(),"1.2.840.10008.1.2.4.70") == 0)
    //    )
    //i_nfo.type=JPEG;

	assert(		!dicom::CSettingsDicom::s_bUseModality
			||	stingModality == "CT"
			||	stingModality == "MR"
			||	stingModality == "PT"
			);
}

//////////////////////////////////////////////
CCreateImage::~CCreateImage()
{}

////////////////////////////////////////////////////////////////////////////////////////////
void CCreateImage::VisitDicomDirectory(dicom::io::CSimpleDirectory & _Directory)
{
    // Parse the folder
    this->ParseDirectory(_Directory);

    // Convert the old acquisition to the new picture (3D) and create the file
    Convert(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
void CCreateImage::VisitDicomFile(dicom::io::CDicomFile & _File)
{
    // Set the member variable and parse the file
    this->m_pFile = &_File;
	assert(this->m_pFile != NULL);

    this->ParseFile(*this->m_pFile);

    // It's really the same file ?
    assert(_File.GetFullName().compare((*this->m_iterFrame)->GetFullPath()) == 0);

	mfo::CFrame * pTempFrame= *this->m_iterFrame;
	assert(pTempFrame!=NULL);

    // save frame info
    pTempFrame->m_pData				= this->m_pBuffer;
    pTempFrame->SetPosition			( this->m_vPosition			) ;
    pTempFrame->SetLittleEndian		( _File.IsLittleEndian()	) ;

	// Verify frame validity
	try
	{
		this->VerifyImageValidity(*pTempFrame);
	}
	catch(const CVisitorException & e)
	{
		if (this->m_iterFrame == this->m_pAcquisition->GetFrameList()->begin())
		{
			this->m_pAcquisition->GetFrameList()->remove(0);
		}
		delete *this->m_iterFrame;
		throw e;
	}

	// It's ok, we can use this frame !
    // If it's the first picture
    // Sens is not important, for optimize algo, only pass is necessary
    if (this->m_iterFrame == this->m_pAcquisition->GetFrameList()->begin())
    {
		this->VerifyFirstImageValidity();
    }

    // Next picture
    this->m_iterFrame++;

	// Post-condition
    assert(pTempFrame->m_pData != NULL);
    assert(pTempFrame->IsLittleEndian() == true || pTempFrame->IsLittleEndian() == false) ;
}

////////////////////////////////////////////////////////////////////////////////////////////
void CCreateImage::VerifyImageValidity(mfo::CFrame & _TempFrame)
{
	// Picture ok ?
    if  (
				!this->m_bPicturePresent
            ||	(_TempFrame.m_pData == NULL)
        )
    {
        throw dicom::visitor::CDicom::CVisitorException(this->m_pFile->GetFullName(), "Picture is not present");
    }

	// Position ok ?
	if	(
				dicom::CSettingsDicom::s_bUseFramePosition
			&&	(
						_TempFrame.GetPosition(0) == POSITION_ERROR
					&&	_TempFrame.GetPosition(1) == POSITION_ERROR
					&&	_TempFrame.GetPosition(2) == POSITION_ERROR
				)
		)
    {
        throw dicom::visitor::CDicom::CVisitorException(this->m_pFile->GetFullName(), "Position tag is not present");
    }

	assert(		_TempFrame.GetPosition(0) != POSITION_ERROR
			||	_TempFrame.GetPosition(1) != POSITION_ERROR
			||	_TempFrame.GetPosition(2) != POSITION_ERROR);
	assert(_TempFrame.m_pData != NULL);
	assert(this->m_bPicturePresent);
}

////////////////////////////////////////////////////////////////////////////////////////////
void CCreateImage::VerifyFirstImageValidity( void )
{
    this->m_pAcquisition->SetBitsPerPixel   ( this->m_ui8BitsPerPixel		) ;
    this->m_pAcquisition->SetHeight         ( this->m_ui32Height			) ;
    this->m_pAcquisition->SetWidth          ( this->m_ui32Width				) ;
	this->m_pAcquisition->SetSliceThickness ( this->m_dSliceThickness		) ;
    this->m_pAcquisition->SetUnsignedFlag   ( this->m_bUnsignedFlag			) ;
	this->m_pAcquisition->SetTime			( this->m_stringAcquisitionTime	) ;
    if (dicom::CSettingsDicom::s_bUseFramePosition)
	{
		std::vector<double> vectorTmp( 3, 2.0 );
		assert( vectorTmp.size() == 3 );
		vectorTmp[0]	= this->m_vectorPixelSize[0]	;
		vectorTmp[1]	= this->m_vectorPixelSize[1]	;
		vectorTmp[2]	= this->m_dSliceThickness	;
		assert( vectorTmp.size() == 3 );
		this->m_pAcquisition->SetVoxelSize      ( vectorTmp ) ;
	}
	else
	{
		std::vector<double> vectorDefault( 3, 2.0 );
		assert( vectorDefault.size() == 3 );
		this->m_pAcquisition->SetVoxelSize      ( vectorDefault) ;
		assert(this->m_pAcquisition->GetVoxelSize()[0]	== 2.0	) ;
		assert(this->m_pAcquisition->GetVoxelSize()[1]	== 2.0	) ;
		assert(this->m_pAcquisition->GetVoxelSize()[2]	== 2.0	) ;
	}

	if	(		(
					this->m_pAcquisition->GetBitsPerPixel() != 8
					&&	this->m_pAcquisition->GetBitsPerPixel() != 16
				)
			||	this->m_pAcquisition->GetHeight()		<= 0
			||	this->m_pAcquisition->GetWidth()		<= 0
			||	this->m_pAcquisition->GetVoxelSize()[0]	<= 0
			||	this->m_pAcquisition->GetVoxelSize()[1]	<= 0
		)
		throw dicom::visitor::CDicom::CVisitorException(this->m_pFile->GetFullName(), "Image value not valid");

	assert(this->m_pAcquisition->GetBitsPerPixel() == 8 || this->m_pAcquisition->GetBitsPerPixel() == 16);
	assert(this->m_pAcquisition->GetHeight()		> 0		) ;
	assert(this->m_pAcquisition->GetWidth()			> 0		) ;
	assert(this->m_pAcquisition->GetVoxelSize()[0]	> 0.0	) ;
	assert(this->m_pAcquisition->GetVoxelSize()[1]	> 0.0	) ;
	assert(this->m_pAcquisition->GetUnsignedFlag() == true || this->m_pAcquisition->GetUnsignedFlag() == false);
	//assert(this->m_pAcquisition->GetTime());// Not used in MEDIC@
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomPixelDataTag(dicom::tag::CPixelData & _Tag)
{
	_Tag.SetSize(this->m_ui32Height*this->m_ui32Width*this->m_ui8BitsPerPixel/8);
	const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_pBuffer = reinterpret_cast<int8*>(_Tag.GetBuffer());
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomTransfertSyntax(dicom::tag::CTransfertSyntax & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
	this->m_stringTransfertSyntax = _Tag.GetTransfertSyntax();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomAcquisitionTimeTag(dicom::tag::CAcquisitionTime & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
	this->m_stringAcquisitionTime = _Tag.GetAcquisitionTime();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomBitsAllocatedTag(dicom::tag::CBitsAllocated & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_ui8BitsPerPixel = _Tag.GetBitsPerPixel();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomRowsTag(dicom::tag::CRows & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    if (
        m_iterFrame != m_pAcquisition->GetFrameList()->begin() &&
        this->m_ui32Height!=_Tag.GetHeight()
       )
    {
        throw dicom::visitor::CDicom::CVisitorException (
            (*this->m_iterFrame)->GetFullPath(), dicom::exception::StringException::SizeNotEqual
                                );
    }
    else
    {
        this->m_ui32Height = _Tag.GetHeight();
    }
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomColumnsTag(dicom::tag::CColumns & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    if (
        m_iterFrame != m_pAcquisition->GetFrameList()->begin() &&
        this->m_ui32Width!=_Tag.GetWidth()
       )
    {
        throw dicom::visitor::CDicom::CVisitorException (
            (*this->m_iterFrame)->GetFullPath(), dicom::exception::StringException::SizeNotEqual
                                );
    }
    else
    {
        this->m_ui32Width = _Tag.GetWidth();
    }
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomPixelSpacingTag(dicom::tag::CPixelSpacing & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_vectorPixelSize = _Tag.GetSpacing();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomImagePositionTag(dicom::tag::CImagePosition & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_vPosition = _Tag.GetPosition();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomModalityTag(dicom::tag::CModality & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sModality = TrimRight(_Tag.GetModalityPatient());

	if (
//			this->m_sModality == "US"
//		 || this->m_sModality == "DR"
//		 || this->m_sModality == "DS"

				dicom::CSettingsDicom::s_bUseModality
			&&	(
					this->m_sModality != "CT"
				&&	this->m_sModality != "MR"
				&&	this->m_sModality != "PT"
				)
	   )
	{
		throw dicom::tag::CDicomTag::CTagExceptionFatal ( this->m_pFile->GetFullName(),
													dicom::exception::StringException::ModalityNotManaged
													+ this->m_sModality
												   );
	}

#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomPixelRepresentation(dicom::tag::CPixelRepresentation & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_bUnsignedFlag = _Tag.isUnsignedFlag();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomSliceThicknessTag(dicom::tag::CSliceThickness & _Tag)
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_dSliceThickness = _Tag.GetThickness();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::GetHeight(void) const
{
	return this->m_ui32Height;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::GetWidth(void) const
{
	return this->m_ui32Width;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::GetBitsAllocated(void) const
{
	return this->m_ui8BitsPerPixel;
}

////////////////////////////////////////////////////////////////////////////////////////////
const bool CCreateImage::IsCreateImage( void ) const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomBirthdayTag(dicom::tag::CBirthday & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomSexeTag(dicom::tag::CSexe & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomHospitalTag(dicom::tag::CHospital & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomImageTypeTag(dicom::tag::CImageType & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomNameTag(dicom::tag::CName & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomPatientIDTag(dicom::tag::CPatientID & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomStudyUIDTag(dicom::tag::CStudyUID & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomSeriesUIDTag(dicom::tag::CSeriesUID & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomAcquisitionNumberTag(dicom::tag::CAcquisitionNumber & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomAcquisitionZoneTag(dicom::tag::CAcquisitionZone & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////
const uint32 CCreateImage::VisitDicomAcquisitionDateTag(dicom::tag::CAcquisitionDate & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

}// end namespace visitor

}// end namespace
