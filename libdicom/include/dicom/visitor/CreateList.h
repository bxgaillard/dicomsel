// VisitorCreateList.h
#ifndef VISITORCREATELIST_DICOMFILE_H
#define VISITORCREATELIST_DICOMFILE_H

#include "Virtuals/Type.h"
#include "Dicom.h"

namespace mfo
{
	class CPatient;
}

namespace dicom
{

namespace visitor
{

/**
 * CreateList
 *********************
 * Namespace : dicom::visitor
 *********************
 * This class analyses a set of file and
 * organize this file in tree which represent
 * exam : (Patient-Study-Serie-Acquisition)
 */
class CCreateList : public dicom::visitor::CDicom
{

public:

	/**
	 * Constructor
	 */
	explicit				CCreateList								( void ) ;

	virtual const bool		IsCreateImage							( void ) const { return false ; }

	/**
	 * Initialize member and call the parse function file.
	 * The file added in the list in the good position.
	 *********************
	 * @pre file is open
	 *********************
	 * @param _File [dicom::CDicom &] : actual file analysed
	 */
	virtual void			VisitDicomFile							( dicom::io::CDicomFile & _File ) ;

	/**
	 * Call ParseDirectory function
	 *********************
	 * @param _Directory [dicom::CSimpleDirectory &] : actual directory analysed
	 */
	virtual void			VisitDicomDirectory						( dicom::io::CSimpleDirectory & _Directory) ;


	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CPixelSpacing &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelSpacingTag				( dicom::tag::CPixelSpacing & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CImagePosition &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomImagePositionTag				( dicom::tag::CImagePosition & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CTransfertSyntax &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomTransfertSyntax				( dicom::tag::CTransfertSyntax & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CRows &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomRowsTag						( dicom::tag::CRows & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CColumns &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomColumnsTag					( dicom::tag::CColumns & _Tag ) ;

	/**
	 * This function get the modality of this examen.
	 *********************
	 * @param _Tag [dicom::tag::CModality &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomModalityTag					( dicom::tag::CModality & _Tag ) ;

	/**
	 * This function get the patient birthday.
	 *********************
	 * @param _Tag [dicom::tag::CBirthday &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomBirthdayTag					( dicom::tag::CBirthday & _Tag ) ;

	/**
	 * This function get the sexe patient.
	 *********************
	 * @param _Tag [dicom::tag::CSexe &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSexeTag						( dicom::tag::CSexe & _Tag ) ;

	/**
	 * This function get the hospital of this examen.
	 *********************
	 * @param _Tag [dicom::tag::CHospital &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomHospitalTag					( dicom::tag::CHospital & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CBitsAllocated &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomBitsAllocatedTag				( dicom::tag::CBitsAllocated & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CPixelData &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelDataTag					( dicom::tag::CPixelData & _Tag ) ;

	/**
	 * This function get the type of this examen.
	 *********************
	 * @param _Tag [dicom::tag::CImageType &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomImageTypeTag					( dicom::tag::CImageType & _Tag ) ;

	/**
	 * This function get the patient name.
	 *********************
	 * @param _Tag [dicom::tag::CName &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomNameTag						( dicom::tag::CName & _Tag ) ;

	/**
	 * This function get the patient id.
	 *********************
	 * @param _Tag [dicom::tag::CPatientID &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPatientIDTag					( dicom::tag::CPatientID & _Tag ) ;

	/**
	 * This function get the study ID.
	 *********************
	 * @param _Tag [dicom::tag::CStudyUID &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomStudyUIDTag					( dicom::tag::CStudyUID & _Tag ) ;

	/**
 	 * This function get the serie ID.
 	 *********************
	 * @param _Tag [dicom::tag::CSeriesUID &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSeriesUIDTag					( dicom::tag::CSeriesUID & _Tag ) ;

	/**
	 * This function get the acquisition time.
	 *********************
	 * @param _Tag [dicom::tag::CAcquisitionNumber &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionNumberTag			( dicom::tag::CAcquisitionNumber & _Tag ) ;

	/**
	 * This function get the acquisition zone.
	 *********************
	 * @param _Tag [dicom::tag::CAcquisitionZone &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionZoneTag			( dicom::tag::CAcquisitionZone & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param  _Tag [dicom::tag::CPixelRepresentation &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelRepresentation			( dicom::tag::CPixelRepresentation & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CSliceThickness &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSliceThicknessTag				( dicom::tag::CSliceThickness & _Tag ) ;

	/**
	 * This function get the acquisition date.
	 *********************
	 * @param  _Tag [dicom::tag::CAcquisitionDate &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionDateTag			( dicom::tag::CAcquisitionDate & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CTransfertSyntax &] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionTimeTag			( dicom::tag::CAcquisitionTime & _Tag ) ;

protected:
	/**
	 * Add this file in the patient list.
	 * The file is add in the good patient, study, serie, acquisition...
	 *********************
	 */
	void					AddImageToList							( void ) ;

	/**
	 * Create a patient and call AddStudy
	 *********************
	 */
	void					AddPatient								( void ) ;

	/**
	 * Create a study, add the study in the list and call AddSerie
	 *********************
	 * @param _patient [mfo::CPatient*] : root(patient) of this study
	 */
	void					AddStudy								( mfo::CPatient* _patient ) ;

	/**
	 * Create a acquisition, add the acquisition in the list and call AddFrame
	 *********************
	 * @param _pserie [CSeries*] : root(serie) of this acquisition
	 */
	void					AddAcquisition							( mfo::CStudy* _pserie );

	/**
	 * Create a frame, add the frame in the list
	 *********************
	 * @param _pAcquisition [CAcquisitionNumber*] : root(acquisition) of this frame
	 */
	void					AddFrame								( mfo::CAcquisition* _pAcquisition ) ;

	std::string				m_sHospital;
	std::string				m_sModality;
	std::string				m_sAcquisitionNB;
	std::string				m_sBirthday;
	std::string				m_sUIDSeries;
	std::string				m_sUIDStudy;
	std::string				m_sName;
	std::string				m_sId;
	std::string				m_sImageType;
	std::string				m_sAcquisitionZone;
	std::string				m_sAcquisitionDate;
	int8					m_i8Sexe;
	int32					m_i32AcquisitionIndex;

private :

	/**
	 * Initialize param foreach patient
	 ****************************************
	 */
	void					InitParam								( void ) ;

	/**
	 * Update the progress loading.
	 ****************************************
	 */
	void					UpdateLoading							( void ) ;

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 ****************************************
	 */
   	explicit				CCreateList								( const CCreateList & ) ;

	/**
	 * Not used.
	 ****************************************
	 */
    CCreateList &			operator =								( const CCreateList & ) ;

	//@}

	int8					m_i32AllData ;
	int8					m_i32DataLoaded ;
	bool					m_bAllDataFound ;
};

}// end namespace visitor

}// end namespace

#endif // VISITORCREATELIST_DICOMFILE_H
