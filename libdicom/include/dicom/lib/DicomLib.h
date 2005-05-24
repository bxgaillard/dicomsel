#ifndef DICOM_LIB_INCLUDES
#define DICOM_LIB_INCLUDES

//disable warnings on extern before template instantiation
#ifdef _MSC_VER
#pragma warning (disable : 4231)
#endif

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DICOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DicomLib_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef _MSC_VER
#ifdef DICOM_EXPORTS
#define DicomLib_API __declspec(dllexport)
#else
#define DicomLib_API __declspec(dllimport)
#endif
#endif


#include <ostream>
#include <string>
#include <vector>

#include <Virtuals/Type.h>

#include <Virtuals/mfo/Patient.h>

namespace dicom
{
namespace exception
{
	class CFatal;
	class CNotFatal;
}

namespace visitor
{
	class CCreateList;
}

// This class is exported from the DicomLib.dll
class
#ifdef _MSC_VER
DicomLib_API
#endif
CFacadeDicom
{
private :
	CFacadeDicom  (const CFacadeDicom&);
	CFacadeDicom &operator=  (const CFacadeDicom &) ;
	dicom::visitor::CCreateList * m_pVisitorList;
	void DestroyVisitor();
public :
	CFacadeDicom();
	~CFacadeDicom();
	void Clear(void);

	std::vector<mfo::CPatient*> * GetVectorPatient( void );

	mfo::CAcquisition* FindAcquisition(const std::string&);

	// Can create DicomException !!!!
	void LoadDirectory(const std::string&);
#ifdef XML_USED
	void LoadXML(const char *, std::ostream & _osError = std::cerr);
#endif
	/////////////////////////////////

	void ListAllASCII(std::ostream&);
	bool isOneSerie(const std::string&);

#ifdef XML_USED
	void ListAllXML(std::ostream&, std::ostream & _osError = std::cerr);
#endif
#ifdef TRIAN_USED
	void CreateAllInrimage(const std::string&);
	static void CreateInrimage( mfo::CAcquisition* , const std::string& , const std::string&, const bool _bCrypt );
	//void CreateInrimageOfOneAcquisition(char *, char *, char *, char *);
#endif
};

class
#ifdef _MSC_VER
DicomLib_API
#endif
CSettingsDicom
{
public:
	//CSettingsDicom():
		//m_bUseAcquisiCSettingsDicomCSettingsDicomtionDate(true),
		//m_bUseSeriesUID(true),
		//m_bUseStudyUID(true),
		//m_bUseModality(true),
		//m_bUseFramePosition(true)
	//{}

	static bool s_bUseAcquisitionDate;
	static bool s_bUseSeriesUID;
	static bool s_bUseStudyUID;
	static bool s_bUseModality;
	static bool s_bUseFramePosition;
/*
	static void CSettingsDicom::setUseAcquisitionDate(const bool _bval)
	{
		CSettingsDicom::s_bUseAcquisitionDate = _bval;
	}

	static void CSettingsDicom::setUseSeriesUID(const bool _bval)
	{
		CSettingsDicom::s_bUseSeriesUID = _bval;
	}

	static void CSettingsDicom::setUseStudyUID(const bool _bval)
	{
		CSettingsDicom::s_bUseStudyUID = _bval;
	}

	static void CSettingsDicom::setUseModality(const bool _bval)
	{
		CSettingsDicom::s_bUseAcquisitionDate = _bval;
	}

	static void CSettingsDicom::setUseFramePosition(const bool _bval)
	{
		CSettingsDicom::s_bUseAcquisitionDate = _bval;
	}*/
	//bool m_bUseAcquisitionDate;
	//bool m_bUseSeriesUID;
	//bool m_bUseStudyUID;
	//bool m_bUseModality;
	//bool m_bUseFramePosition;
	//
	// Sous VC7.1, g++, faire des accesseurs static... Et peut-�re exporter manuellement chacun des symboles.
	// A mettre le code des accesseurs dans le cpp.
};

}

/**
extern DicomLib_API int nDicomLib;

DicomLib_API int fnDicomLib(void);
*/

//DicomLib_API const std::string& GetError(const dicom::CExceptionDicom * const);
#endif // DICOM_LIB_INCLUDES
