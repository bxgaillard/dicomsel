// VIRTUALS basic type
#include "Virtuals/Type.h"

// STL's stream
#include <iostream>

//      TAG     //
//////////////////
//all tag
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
//0x0018
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
//////////////////

// IO
#include "io/DicomFile.h"
#include "io/SimpleDirectory.h"
// Base file
#include "Virtuals/mfo/mfo.h"
// Factory
#include "DicomAbstractFactory.h"
// Exception
#include "ExceptionDicom.h"
////
#include "visitor/CreateList.h"
#include "lib/DicomLib.h"

namespace dicom
{

namespace visitor
{

//////////////////////////////////
CCreateList::CCreateList( void ) :
	CDicom			(),
	m_i32AllData	(0),
	m_i32DataLoaded	(0),
	m_bAllDataFound	(false)
{}

/////////////////////////////////////////////////////////////////////////////////
void CCreateList::VisitDicomDirectory( dicom::io::CSimpleDirectory & _Directory )
{
#ifdef _DEBUG_STREAM
	std::ofstream ofsDicomDirLog("DicomDirLog.txt", std::ios_base::out | std::ios_base::app );
	ofsDicomDirLog << "Start visit \"" << _Directory.GetFullName() << "\" directory" << std::endl;
#endif
    this->ParseDirectory(_Directory);
#ifdef _DEBUG_STREAM
	ofsDicomDirLog << "End visit" << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////
void CCreateList::VisitDicomFile( dicom::io::CDicomFile & _File )
{
	assert(_File.IsOpen());

    this->m_pFile = &_File;
    this->InitParam();
    this->ParseFile(*m_pFile);
    this->AddImageToList();

	//assert( le fichier a ��ajout��la liste ); // fixme
}

///////////////////////////////////////
void CCreateList::UpdateLoading( void )
{
	++(this->m_i32DataLoaded);
	if (this->m_i32DataLoaded == this->m_i32AllData)
	{
		this->m_bAllDataFound = true;
	}
}

///////////////////////////////////
void CCreateList::InitParam( void )
{
	this->m_i8Sexe				= 0 ; this->m_i32AllData = 1;
    this->m_i32AcquisitionIndex = 0 ; this->m_i32AllData++;
	this->m_sModality			= ""; this->m_i32AllData++;
	this->m_sBirthday			= ""; this->m_i32AllData++;
	this->m_sHospital			= ""; this->m_i32AllData++;
	this->m_sImageType			= ""; this->m_i32AllData++;
    this->m_sName               = ""; this->m_i32AllData++;
    this->m_sId                 = ""; this->m_i32AllData++;
    this->m_sUIDSeries          = ""; this->m_i32AllData++;
    this->m_sUIDStudy           = ""; this->m_i32AllData++;
	this->m_sAcquisitionDate	= ""; this->m_i32AllData++;
	this->m_sAcquisitionZone	= ""; this->m_i32AllData++;

    this->m_sFullPath           = "";
	this->m_bAllDataFound		= false;
	this->m_i32DataLoaded		= 0;
    this->SetPicturePresent		(false);
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomModalityTag( dicom::tag::CModality & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sModality = _Tag.GetModalityPatient();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomBirthdayTag( dicom::tag::CBirthday & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sBirthday = _Tag.GetBirthdayPatient();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomSexeTag( dicom::tag::CSexe & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_i8Sexe = _Tag.GetSexePatient();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomHospitalTag( dicom::tag::CHospital & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sHospital = _Tag.GetHospitalPatient();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomImageTypeTag( dicom::tag::CImageType & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sImageType = _Tag.GetType();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomNameTag( dicom::tag::CName & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sName = _Tag.GetNamePatient();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomPatientIDTag( dicom::tag::CPatientID & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sId = _Tag.GetID();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomStudyUIDTag( dicom::tag::CStudyUID & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sUIDStudy = _Tag.GetUID();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomSeriesUIDTag( dicom::tag::CSeriesUID & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sUIDSeries = _Tag.GetUID();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomAcquisitionNumberTag( dicom::tag::CAcquisitionNumber & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_i32AcquisitionIndex = _Tag.GetNumber();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomAcquisitionZoneTag( dicom::tag::CAcquisitionZone & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sAcquisitionZone = _Tag.GetZone();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomAcquisitionDateTag( dicom::tag::CAcquisitionDate & _Tag )
{
    const uint32 ui32Pos = _Tag.SetValue(*this->m_pFile);
    this->m_sAcquisitionDate = _Tag.GetAcquisitionDate();
	this->UpdateLoading();
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
void CCreateList::AddImageToList( void )
{
    bool bPatientFound      = false;
    bool bStudyFound        = false;
    bool bAcquisitionFound  = false;

    std::vector<mfo::CPatient*>::iterator        iListPatient;
    std::vector<mfo::CStudy*>::iterator          iListStudy;
    std::vector<mfo::CAcquisition*>::iterator    iListAcquisition;

    // Search a patient
    for(iListPatient = this->m_vectorPatient.begin();
        iListPatient < this->m_vectorPatient.end() &&
        !bPatientFound;
        iListPatient++
        )
    {
        if ((*iListPatient)->GetFullName() == this->m_sName)
        {
            // Search a study
            for(iListStudy = (*iListPatient)->GetVectorStudy()->begin();
                iListStudy < (*iListPatient)->GetVectorStudy()->end() &&
                !bStudyFound;
                iListStudy++
                )
            {
                if	(
							!dicom::CSettingsDicom::s_bUseStudyUID
						||	(*iListStudy)->GetUID() == this->m_sUIDStudy
					)
                {
                            // Search an acquisition
                            for(iListAcquisition = (*iListStudy)->GetVectorAcquisition()->begin();
                                iListAcquisition < (*iListStudy)->GetVectorAcquisition()->end() &&
                                !bAcquisitionFound;
                                iListAcquisition++
                                )
                            {
                                if ( 
                                        (
												!dicom::CSettingsDicom::s_bUseAcquisitionDate
											||	(*iListAcquisition)->GetIndex() == this->m_i32AcquisitionIndex	
										)
                                     && (*iListAcquisition)->GetImageType().compare(this->m_sImageType) == 0 
                                     && (
												!dicom::CSettingsDicom::s_bUseSeriesUID
											||	(*iListAcquisition)->GetSerieUID().compare(this->m_sUIDSeries) == 0
										)
                                   )
                                {
                                    bAcquisitionFound = true;
                                }

                            }
                    bStudyFound = true;
                }
            }
            bPatientFound = true;
        }
    }

    if(bPatientFound)
    {
        if(bStudyFound)
        {
                if(bAcquisitionFound)
                {
                    AddFrame(*(iListAcquisition-1)); // new frame
                }
                else
                {
                    AddAcquisition(*(iListStudy-1)); // New acquisition
                }
        }
        else
        {
            AddStudy(*(iListPatient-1)); // New study
        }
    }
    else
    {
        AddPatient(); // New patient
    }

	//assert( le fichier a ��ajout��la liste ); // fixme
}

///////////////////////////////////////////////////////////////////////////////
void CCreateList::AddPatient( void )
{
#if defined(_CONSOLE)
    std::cout<< this->m_sName << std::endl;
#endif
    mfo::CPatient* pPatient = new mfo::CPatient(this->m_sName, this->m_sId, this->m_sBirthday, this->m_i8Sexe);
    this->m_vectorPatient.push_back(pPatient);
    this->AddStudy(pPatient);
}

///////////////////////////////////////////////////////////////////////////////
void CCreateList::AddStudy( mfo::CPatient* _pPatient )
{
    mfo::CStudy* pStudy = new mfo::CStudy(this->m_sUIDStudy, _pPatient, this->m_sHospital, this->m_sModality, this->m_sAcquisitionZone, std::string(""));
    _pPatient->GetVectorStudy()->push_back(pStudy);
    this->AddAcquisition(pStudy);
}

///////////////////////////////////////////////////////////////////////////////
void CCreateList::AddAcquisition( mfo::CStudy* _pStudy )
{
    mfo::CAcquisition* pAcquisition = new mfo::CAcquisition(this->m_i32AcquisitionIndex, this->m_sAcquisitionDate, _pStudy, this->m_sImageType, this->m_sUIDSeries);
    _pStudy->GetVectorAcquisition()->push_back(pAcquisition);
    this->AddFrame(pAcquisition);
}

///////////////////////////////////////////////////////////////////////////////
void CCreateList::AddFrame( mfo::CAcquisition* _pAcquisition )
{

    // A special condition can be add to check if a file with a same name
    // doesn't exist in this acquisition.
    // But it's not really interesting and it will more time to add a frame

    mfo::CFrame* pFrame = new mfo::CFrame( m_pFile->GetFullName(), _pAcquisition ) ;
    _pAcquisition->GetFrameList()->push_back( pFrame ) ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomPixelRepresentation(dicom::tag::CPixelRepresentation & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomPixelSpacingTag(dicom::tag::CPixelSpacing & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomBitsAllocatedTag(dicom::tag::CBitsAllocated & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomPixelDataTag(dicom::tag::CPixelData & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomRowsTag(dicom::tag::CRows & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomColumnsTag(dicom::tag::CColumns & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomImagePositionTag(dicom::tag::CImagePosition & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomSliceThicknessTag(dicom::tag::CSliceThickness & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomTransfertSyntax(dicom::tag::CTransfertSyntax & _Tag)
{
	const uint32 ui32Pos = this->m_pFile->GetOffset()+_Tag.GetLength();
	this->m_pFile->SetOffset(ui32Pos);
#ifdef _DEBUG
    VisitDicomTag(_Tag);
#endif
	return ui32Pos;
}

///////////////////////////////////////////////////////////////////////////////
const uint32 CCreateList::VisitDicomAcquisitionTimeTag(dicom::tag::CAcquisitionTime & _Tag)
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
