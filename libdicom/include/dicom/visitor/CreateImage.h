// CreateImage.h
#ifndef CreateImage_DICOMFILE_H
#define CreateImage_DICOMFILE_H

#include "Virtuals/Type.h"
#include "visitor/Dicom.h"

namespace dicom
{
namespace visitor
{

/** 
 * CreateImage
 *********************
 * Namespace : dicom::visitor
 *********************
 * This file is a framework to create block image
 * with a dicom file acquisition. (eg : Inrimage)
 * For each file (with egal patient name, uid study,
 * uid serie and acquisition date otherwise an
 * exeption is created ), this class save
 * the data (picture + size + bpp + pixel size +
 * position + modality).
 * This class is a concret visitor in the Design
 * Pattern Visitor. It's implement the Visitor
 * abstract visitor. And this run into a dicom
 * tree file.
 */
class CCreateImage : public dicom::visitor::CDicom
{

public:

	/**
	 * Constructor with two parameters.
	 * Initialize a pointer on the actual acquisition, fix the picture filename,
	 * set frame iterator on the acquisition first frame and fix the buffer
	 * on NULL (it will initialize after read the dicom tag : 'PIXEL DATA').
	 * All another member are initilized with default value.
	 *********************
	 * @post Frame iterator, size of name picture file and acquisition not null,
	 * data buffer null.
	 * @post Modality is a CT or MR
	 *********************
	 * @param _sOutputFilename [const std::string &]  : Filename of images block created
	 * @param _pAcquisition [CAcquisition *] : Original data
	 *********************
	 * @exception CVisitorException
	 */
	explicit				CCreateImage							(	const std::string & _sOutputFilename, 
																		mfo::CAcquisition* const _pAcquisition) ;
	/**
	 * Destructor
	 *********************
	 */
							~CCreateImage							();

	virtual void			Convert									( const bool ) = 0;

	/**
	 * @return const uint32 : true, because this class creates picture !
	 *********************
	 */
	virtual const bool		IsCreateImage							( void ) const ;

	/**
	 * This function visits a dicom file and finds interesting variables
	 * to create the picture. For each picture, it saves the data buffer,
	 * the space position of the picture (to sort pictures before to created
	 * the 3D picture) and the endianess of the file.
	 * For the first file, it saves main acquisition informations.
	 *********************
	 * @pre the file exists and gives in parameter must 
	 * the same in the frame interator
	 *********************
	 * @post bits per pixel == 8 or 16
	 * @post height & width > 0
	 * @post voxel size (x,y) > 0
	 * @post flag is true or false
	 * @post position tag present
	 * @post picture data present
	 *********************
	 * @param _File [dicom::io::CDicomFile &] : actual dicom file parsed
	 *********************
	 * @exception CVisitorException
	 */
	virtual void			VisitDicomFile							( dicom::io::CDicomFile & _File) ;

	/**
	 * This function call the directory's parse. Next, it converts the
	 * the picture block to a 3d picture.
	 * Check the acquisition (and study) value.
	 * <LI>Test modality (CT or MR)
	 * <LI>Test Transfert syntax (Not used at this date).
	 *********************
	 * @param _Directory [dicom::CSimpleDirectory &] : the directory analysed
	 */
	virtual void			VisitDicomDirectory						( dicom::io::CSimpleDirectory & _Directory) ;

	/**
	 * This function get the transfert syntax of the dicom file.
	 *********************
	 * @param _Tag [dicom::tag::CTransfertSyntax & : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomTransfertSyntax				( dicom::tag::CTransfertSyntax & _Tag ) ;

	/**
	 * This function get the pixel spacing (Z space in the 3D picture).
	 *********************
	 * @param _Tag [dicom::tag::CPixelSpacing & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelSpacingTag				( dicom::tag::CPixelSpacing & _Tag ) ;

	/**
	 * This function get the position of the picture.
	 *********************
	 * @param _Tag [dicom::tag::CImagePosition & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomImagePositionTag				( dicom::tag::CImagePosition & _Tag ) ;

	/**
	 * This function get the modality of this acquisition.
	 *********************
	 * @param _Tag [dicom::tag::CModality & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomModalityTag					( dicom::tag::CModality & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CSexe & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSexeTag						( dicom::tag::CSexe & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CBirthday & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomBirthdayTag					( dicom::tag::CBirthday & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CHospital & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomHospitalTag					( dicom::tag::CHospital & _Tag ) ;

	/**
	 * This function get the picture height.
	 *********************
	 * @exception CVisitorException with dicom::exception::SizeNotEqual parameter
	 *********************
	 * @param _Tag [dicom::tag::CRows & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomRowsTag						( dicom::tag::CRows & _Tag ) ;

	/**
	 * This function get the picture width.
	 *********************
	 * @exception CVisitorException with dicom::exception::SizeNotEqual parameter
	 *********************
	 * @param _Tag [dicom::tag::CColumns & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomColumnsTag					( dicom::tag::CColumns & _Tag ) ;

	/**
	 * This function get the number of bytes per pixel.
	 *********************
	 * @param _Tag [dicom::tag::CBitsAllocated & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomBitsAllocatedTag				( dicom::tag::CBitsAllocated & _Tag ) ;

	/**
	 * This function get the data buffer
	 *********************
	 * @param _Tag [dicom::tag::CPixelData & ] : actual tag parsed.
	 ****************************************
	 * @return the size of this tag
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelDataTag					( dicom::tag::CPixelData & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CImageType & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomImageTypeTag					( dicom::tag::CImageType & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CName & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomNameTag						( dicom::tag::CName & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CPatientID & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPatientIDTag					( dicom::tag::CPatientID & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CStudyUID & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomStudyUIDTag					( dicom::tag::CStudyUID & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CSeriesUID & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSeriesUIDTag					( dicom::tag::CSeriesUID & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CAcquisitionNumber & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionNumberTag			( dicom::tag::CAcquisitionNumber & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CAcquisitionZone & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionZoneTag			( dicom::tag::CAcquisitionZone & _Tag ) ;

	/**
	 * This function get the pixel representation.
	 *********************
	 * @param _Tag [dicom::tag::CPixelRepresentation & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomPixelRepresentation			( dicom::tag::CPixelRepresentation & _Tag ) ;

	/**
	 * This function get the slice thickness.
	 *********************
	 * @param _Tag [dicom::tag::CSliceThickness & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomSliceThicknessTag				( dicom::tag::CSliceThickness & _Tag ) ;

	/**
	 * Not used !
	 *********************
	 * @param _Tag [dicom::tag::CAcquisitionZone & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionDateTag			( dicom::tag::CAcquisitionDate & _Tag ) ;

	/**
	 * This function get the acquisition time.
	 *********************
	 * @param _Tag [dicom::tag::CTransfertSyntax & ] : actual tag parsed
	 ****************************************
	 * @return the size of this tag
	 */
	virtual const uint32	VisitDicomAcquisitionTimeTag			( dicom::tag::CAcquisitionTime & _Tag ) ;


	/** @name Accessor (Get) */
	//@{

	/**
	 * @return const uint32 : height the current picture
	 *********************
	 */
	const uint32			GetHeight								( void ) const ;

	/**
	 * @return const uint32 : width the current picture
	 *********************
	 */
	const uint32			GetWidth								( void ) const ;

	/**
	 * @return const uint32 : Bytes per pixel used in the current picture
	 *********************
	 */
	const uint32			GetBitsAllocated						( void ) const ;

	//@}

	/** @name Accessor (Set) */
	//@{

	/**
	 * @param const std::string
	 *********************
	 */
	void					SetOutput								( const std::string _stringOutput ) { this->m_sOutputFilename = _stringOutput; }

	//@}

protected:

	/**
	 * Assign value for the acquisition from the
	 * first dicom file. Check value validity.
	 ****************************************
	 * @post bits per pixel == 8 or 16
	 * @post height & width > 0
	 * @post voxel size (x,y) > 0
	 * @post flag is true or false
	 *********************
	 * @exception CVisitorException
	 */
	void					VerifyFirstImageValidity				( void ) ;

	/**
	 * Check the frame value.
	 * <LI>Test if the file has a picture.
	 * <LI>Test the position.
	 ****************************************
	 * @param _TempFrame [mfo::CFrame &] : frame to check
	 ****************************************
	 * @post position tag present
	 * @post picture data present
	 *********************
	 * @exception CVisitorException
	 */
	void					VerifyImageValidity						( mfo::CFrame & _TempFrame ) ;

	void					SetPixelSpacing							( const uint32 & ) ;

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 ****************************************
	 */
	explicit				CCreateImage							( const CCreateImage & ) ;

	/**
	 * Not used.
	 ****************************************
	 */
	CCreateImage &			operator =								( const CCreateImage & ) ;

	//@}

	bool					m_bUnsignedFlag;
	double					m_dSpacingBetweenSlicesSupposed;
	double					m_dSliceThickness;
	uint32					m_ui32Height;
	uint32					m_ui32Width;
	uint8					m_ui8BitsPerPixel;
	uint32					m_uiFileNumber;
	uint32					m_uiNumberSlices;
	int8 *					m_pBuffer;
	std::string				m_sModality;
	std::string				m_stringTransfertSyntax;
	std::string				m_stringAcquisitionTime;
	std::string				m_sOutputFilename;
	std::vector<double>		m_vectorPixelSize;
	std::vector<double>		m_vPosition;
	std::list<mfo::CFrame*>::iterator m_iterFrame;
	mfo::CAcquisition *		m_pAcquisition;

};

}// end namespace visitor

}// end namespace

#endif // CreateImage_DICOMFILE_H
