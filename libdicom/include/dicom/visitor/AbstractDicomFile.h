// VisitorAbstractDicomFile.h
#ifndef VISITORABSTRACT_DICOMFILE_H
#define VISITORABSTRACT_DICOMFILE_H

#include "Virtuals/Type.h"

namespace dicom
{

namespace io
{
	// Template class name used
	class CDicomFile;
	class CSimpleDirectory;
}

// Template class name used
namespace tag
{
class CDicomTag;
class CName;
class CPatientID;
//class CImageNumber;
class CStudyUID;
class CSeriesUID;
class CPixelSpacing;
class CImagePosition;
class CAcquisitionNumber;
class CRows;
class CColumns;
class CModality;
class CBirthday;
class CSexe;
class CHospital;
class CBitsAllocated;
class CPixelData;
class CImageType;
class CAcquisitionZone;
class CPixelRepresentation;
class CSliceThickness;
class CAcquisitionDate;
class CTransfertSyntax;
class CAcquisitionTime;
}

namespace visitor
{

/**
 * CAbstractDicomFile
 *********************
 * Namespace : dicom::visitor
 *********************
 * This class represents an abstract visitor of a dicom tree.
 * After visit folder and file, it visits each tag and copy
 * interest values.
 * One function is used to know if the concret visitor
 * is a create image visitor or not.
 * This class play the role of AbstractVisitor in the
 * Design Pattern VISITOR.
 * (p 387 in french version of DESIGN PATTERNS (Erich Gamma))
 */
class CAbstractDicomFile
{
public:
    virtual ~CAbstractDicomFile( void ) = 0;

    virtual void			VisitDicomFile							( dicom::io::CDicomFile	& )				= 0;
    virtual void			VisitDicomDirectory						( dicom::io::CSimpleDirectory & )		= 0;

    virtual const uint32	VisitUnknownDicomTag					( dicom::tag::CDicomTag & )				= 0;
    virtual const uint32	VisitDicomNameTag						( dicom::tag::CName & )					= 0;
    virtual const uint32	VisitDicomPatientIDTag					( dicom::tag::CPatientID & )			= 0;
    virtual const uint32	VisitDicomTransfertSyntax				( dicom::tag::CTransfertSyntax & )		= 0;
    virtual const uint32	VisitDicomStudyUIDTag					( dicom::tag::CStudyUID & )				= 0;
    virtual const uint32	VisitDicomSeriesUIDTag					( dicom::tag::CSeriesUID & )			= 0;
    virtual const uint32	VisitDicomPixelSpacingTag				( dicom::tag::CPixelSpacing & )			= 0;
    virtual const uint32	VisitDicomImagePositionTag				( dicom::tag::CImagePosition & )		= 0;
    virtual const uint32	VisitDicomAcquisitionNumberTag			( dicom::tag::CAcquisitionNumber & )	= 0;
    virtual const uint32	VisitDicomRowsTag						( dicom::tag::CRows & )					= 0;
    virtual const uint32	VisitDicomColumnsTag					( dicom::tag::CColumns & )				= 0;
    virtual const uint32	VisitDicomModalityTag					( dicom::tag::CModality & )				= 0;
    virtual const uint32	VisitDicomBirthdayTag					( dicom::tag::CBirthday & )				= 0;
    virtual const uint32	VisitDicomSexeTag						( dicom::tag::CSexe & )					= 0;
    virtual const uint32	VisitDicomHospitalTag					( dicom::tag::CHospital & )				= 0;
    virtual const uint32	VisitDicomBitsAllocatedTag				( dicom::tag::CBitsAllocated & )		= 0;
    virtual const uint32	VisitDicomPixelDataTag					( dicom::tag::CPixelData & )			= 0;
    virtual const uint32	VisitDicomImageTypeTag					( dicom::tag::CImageType & )			= 0;
    virtual const uint32	VisitDicomAcquisitionZoneTag			( dicom::tag::CAcquisitionZone & )		= 0;
    virtual const uint32	VisitDicomPixelRepresentation			( dicom::tag::CPixelRepresentation & )	= 0;
    virtual const uint32	VisitDicomSliceThicknessTag				( dicom::tag::CSliceThickness & )		= 0;
	virtual const uint32	VisitDicomAcquisitionDateTag			( dicom::tag::CAcquisitionDate & )		= 0;
	virtual const uint32	VisitDicomAcquisitionTimeTag			( dicom::tag::CAcquisitionTime & )		= 0;

    virtual const bool		IsCreateImage							( void ) const							= 0;
};

}// end namespace visitor

}// end namespace

#endif // VISITORABSTRACT_DICOMFILE_H
