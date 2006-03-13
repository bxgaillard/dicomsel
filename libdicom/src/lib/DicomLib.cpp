// DicomLib.cpp : Defines the entry point for the DLL application.
//

#ifdef _MSC_VER
/*
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOGDICAPMASKS     ;
#define NOVIRTUALKEYCODES ;
#define NOWINMESSAGES     ;
#define NOWINSTYLES       ;
#define NOSYSMETRICS      ;
#define NOMENUS           ;
#define NOICONS           ;
#define NOKEYSTATES       ;
#define NOSYSCOMMANDS     ;
#define NORASTEROPS       ;
#define NOSHOWWINDOW      ;
#define OEMRESOURCE       ;
#define NOATOM            ;
#define NOCLIPBOARD       ;
#define NOCOLOR           ;
#define NOCTLMGR          ;
#define NODRAWTEXT        ;
#define NOGDI             ;
#define NOKERNEL          ;
#define NOUSER            ;
#define NONLS             ;
#define NOMB              ;
#define NOMEMMGR          ;
#define NOMETAFILE        ;
#define NOMINMAX          ;
#define NOMSG             ;
#define NOOPENFILE        ;
#define NOSCROLL          ;
#define NOSERVICE         ;
#define NOSOUND           ;
#define NOTEXTMETRIC      ;
#define NOWH              ;
#define NOWINOFFSETS      ;
#define NOCOMM            ;
#define NOKANJI           ;
#define NOHELP            ;
#define NOPROFILER        ;
#define NODEFERWINDOWPOS  ;
#define NOMCX			  ;
*/
#include <windows.h>

#endif


#include <sstream>

// Assertions
#include <cassert>

#include "Virtuals/Type.h"
#include "Virtuals/mfo/mfo.h"

#ifdef XML_USED
#include "XMLParserMFO/XMLParserMFO.h"
#endif

#include "io/SimpleDirectory.h"
#include "visitor/CreateList.h"
#ifdef TRIAN_USED
#include "visitor/CreateInrimage.h"
#endif

#include "ExceptionDicom.h"

#include "lib/DicomLib.h"

const std::string dicom::exception::StringException::FormatNotDefined("Format DICOM inconnu ou fichier DICOM incorrect");
const std::string dicom::exception::StringException::FileNotFound("File specification that could not be matched or Invalid filename specification (perhaps not read permissions)");
const std::string dicom::exception::StringException::ModalityNotManaged("This modality is not managed at this time");
const std::string dicom::exception::StringException::NoPictureFound("This file have not picture");
const std::string dicom::exception::StringException::SizeNotEqual("Size not egual in all files");

// Nous sommes sous Visual Studio
#ifdef _MSC_VER
BOOL APIENTRY DllMain( HANDLE /**hModule*/,
                       DWORD  ul_reason_for_call,
                       LPVOID /**lpReserved*/
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif
// Dans le cas o on exporte les symboles

bool dicom::CSettingsDicom::s_bUseAcquisitionDate = true;
bool dicom::CSettingsDicom::s_bUseSeriesUID = true;
bool dicom::CSettingsDicom::s_bUseStudyUID = true;
bool dicom::CSettingsDicom::s_bUseModality = true;
bool dicom::CSettingsDicom::s_bUseFramePosition = true;

namespace dicom
{

/**
 * This is the constructor of a class that has been exported.
 ****************************************
 * @param :
 ****************************************
 * @see DicomLib.h for the class definition
 ****************************************
 * @return
 ****************************************
 * @exception
 */
CFacadeDicom::CFacadeDicom(void):
    m_pVisitorList(new dicom::visitor::CCreateList)
{
    assert(sizeof(uint8)  == 1);
    assert(sizeof(uint16) == 2);
    assert(sizeof(uint32) == 4);
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::LoadDirectory(const std::string& _sPathFile)
{
#ifdef _DEBUG_STREAM
	std::ofstream ofsDicomDirLog("DicomDirLog.txt", std::ios_base::out | std::ios_base::app );
	ofsDicomDirLog << "\nLoad dicom folder : " << _sPathFile  << std::endl;
#endif
    dicom::io::CSimpleDirectory directory(_sPathFile);
#ifdef _DEBUG_STREAM
	ofsDicomDirLog << std::endl << "Start visit" << std::endl ;
	ofsDicomDirLog.flush();
	ofsDicomDirLog.close();
#endif
    directory.Visit(this->m_pVisitorList);
}

mfo::CAcquisition* CFacadeDicom::FindAcquisition(const std::string& _sSerieUID)
{
    for (
            std::vector<mfo::CPatient*>::iterator iterPatient = this->m_pVisitorList->GetVectorPatient()->begin();
            iterPatient<this->m_pVisitorList->GetVectorPatient()->end();
			iterPatient++
        )
    {
        for (
                std::vector<mfo::CStudy*>::iterator iterStudy = (*iterPatient)->GetVectorStudy()->begin();
                iterStudy<(*iterPatient)->GetVectorStudy()->end();
				iterStudy++
            )
        {
	        for (
	                std::vector<mfo::CAcquisition*>::iterator iterAcquisition = (*iterStudy)->GetVectorAcquisition()->begin();
	                iterAcquisition<(*iterStudy)->GetVectorAcquisition()->end();
					iterAcquisition++
	            )
	        {
				if ( (*iterAcquisition)->GetSerieUID() == _sSerieUID )
				{
					return (*iterAcquisition);
				}
			}
		}
	}
	return 0;
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::DestroyVisitor()
{
    if (this->m_pVisitorList!=NULL)
	{
		assert(this->m_pVisitorList != NULL);
		delete this->m_pVisitorList;
		this->m_pVisitorList = NULL;
	}
	assert(this->m_pVisitorList == NULL);
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
CFacadeDicom::~CFacadeDicom()
{
    this->DestroyVisitor();
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::ListAllASCII(std::ostream& _osList)
{
	_osList << *(this->m_pVisitorList) << std::endl;
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
bool CFacadeDicom::isOneSerie(const std::string& _sDicomSerieUID)
{
	bool bRes = true;
	uint32 ui32NbPatient = 0;
	uint32 ui32NbStudy = 0;
	uint32 ui32NbAcquisition = 0;
    for (
            std::vector<mfo::CPatient*>::iterator iterPatient = this->m_pVisitorList->GetVectorPatient()->begin();
            iterPatient<this->m_pVisitorList->GetVectorPatient()->end();
			iterPatient++
        )
    {
		if ( ++ui32NbPatient > 1 ) bRes = false ;
        for (
                std::vector<mfo::CStudy*>::iterator iterStudy = (*iterPatient)->GetVectorStudy()->begin();
                iterStudy<(*iterPatient)->GetVectorStudy()->end();
				iterStudy++
            )
        {
			if ( ++ui32NbStudy > 1 ) bRes = false ;
	        for (
	                std::vector<mfo::CAcquisition*>::iterator iterAcquisition = (*iterStudy)->GetVectorAcquisition()->begin();
	                iterAcquisition<(*iterStudy)->GetVectorAcquisition()->end();
					iterAcquisition++
	            )
	        {
				if ( ++ui32NbAcquisition > 1 ) bRes = false ;
				if ( (*iterAcquisition)->GetSerieUID() != _sDicomSerieUID )
				{
					bRes = false;
				}
			}
		}
	}
	return bRes;
}

#ifdef XML_USED

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::ListAllXML(std::ostream& _osList, std::ostream & _osError )
{
	mxp::CXMLParserMFO::writePatientVector(_osList, *(this->m_pVisitorList->GetVectorPatient()), _osError);
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::LoadXML( const char * _ofsList, std::ostream & _osError )
{
    mxp::CXMLParserMFO::readPatientVector(_ofsList, this->m_pVisitorList->GetVectorPatient(), _osError);
}

#endif

#ifdef TRIAN_USED
/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::CreateAllInrimage(const std::string& _sFolder)
{
	std::ostringstream ost("", std::ios_base::ate);

    for (
            std::vector<mfo::CPatient*>::reverse_iterator iterPatient = this->m_pVisitorList->GetVectorPatient()->rbegin();
            iterPatient<this->m_pVisitorList->GetVectorPatient()->rend();
        )
    {
        for (
                std::vector<mfo::CStudy*>::reverse_iterator iterStudy = (*iterPatient)->GetVectorStudy()->rbegin();
                iterStudy<(*iterPatient)->GetVectorStudy()->rend();
            )
        {
#ifdef _DEBUG
			uint32 ui32index = 0;
#endif
	        for (
	                std::vector<mfo::CAcquisition*>::reverse_iterator iterAcquisition = (*iterStudy)->GetVectorAcquisition()->rbegin();
	                iterAcquisition<(*iterStudy)->GetVectorAcquisition()->rend();
#ifdef _DEBUG
					ui32index++
#endif
	            )
	        {
	            try
	            {
	                char cTabSize[20];
	                ::sprintf(cTabSize,"0%d-d%d__n%d\0",
#ifdef _DEBUG
										ui32index,
#else
										0,
#endif
										(*iterAcquisition)->GetIndex(), (*iterAcquisition)->GetFrameList()->size());

	                dicom::visitor::CCreateInrimage vConvert(_sFolder+"/+patient", *iterAcquisition);

	                // Pas besoin de re-parcourir toute l'arborescence, on reprend les fichiers d��list�
	                for (   std::list<mfo::CFrame*>::iterator iterFrame = (*iterAcquisition)->GetFrameList()->begin();
	                        iterFrame != (*iterAcquisition)->GetFrameList()->end();
	                        iterFrame++
	                    )
	                {
	                    //dicom::io::CDicomFile file((*iterFrame)->GetFullPath()*, "");
			    dicom::io::CDicomFile file((*iterFrame)->GetFullPath());
	                    file.LoadInputFile();
	                    file.Visit(&vConvert);
	                }
	                std::string filename(_sFolder+"/"+
										(*iterAcquisition)->GetTime()
	                                    //+(((*iterAcquisition)->GetUnsignedFlag())?"Unsigned_":"Signed_")
	                                    +(( (*iterPatient)->GetFullName()[1] == '/')?std::string(""):(*iterPatient)->GetFullName().c_str())
	                                    //+"__date"+(*iterAcquisition)->GetDate()+
	                                    +"__"+ std::string(cTabSize)
	                                    //+(*iterAcquisition)->GetSerieUID()+"__")
										+"__"
										);

#ifdef _CONSOLE
	                std::cout << filename << std::endl;
#endif
					vConvert.SetOutput(filename);

	                // On demande au visiteur de convertir (ici en Inrimage)
	                vConvert.Convert( true );
	            }
				catch(const dicom::visitor::CDicom::CVisitorException& e)
				{
	                ost << (*iterPatient)->GetName() << " : " << e.what() << " - " << (*iterAcquisition)->GetFrameList()->size() << std::endl;
				}
	            catch(const std::exception& e)
	            {
	                ost << (*iterPatient)->GetName() << " : " << e.what() << std::endl;
	            }
	            catch(...)
	            {
	                ost << (*iterPatient)->GetName() << " : " << "unknown exception" << std::endl;
	            }

	            //delete *iterAcquisition;
	            //(*iterStudy)->GetVectorAcquisition()->erase(&*iterAcquisition);
	            delete *iterAcquisition;
	            (*iterStudy)->GetVectorAcquisition()->pop_back();
		    iterAcquisition = (*iterStudy)->GetVectorAcquisition()->rbegin();
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
//         FIXME          //
////////////////////////////
////////////////////////////
////////////////////////////
//	            iterAcquisition = (*iterStudy)->GetVectorAcquisition()->rend();
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////

            }
            delete *iterStudy;
            (*iterPatient)->GetVectorStudy()->pop_back();
            iterStudy = (*iterPatient)->GetVectorStudy()->rbegin();
        }
        delete *iterPatient;
        this->m_pVisitorList->GetVectorPatient()->pop_back();
        iterPatient = this->m_pVisitorList->GetVectorPatient()->rbegin();
        //iterPatient = this->m_pVisitorList->GetVectorPatient()->end();
    }

	if (ost.str().size() > 1)
	{
		throw dicom::visitor::CDicom::CVisitorException(_sFolder, ost.str());
	}
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::CreateInrimage( mfo::CAcquisition* _pAcquisition, const std::string& _sOutputFolder, const std::string& _stringFullIdHospital, const bool _bCrypt)
{
	std::ostringstream ost("", std::ios_base::ate);

	try
	{
		_pAcquisition->SetPath(_stringFullIdHospital);
	    dicom::visitor::CCreateInrimage vConvert(_sOutputFolder+"/", _pAcquisition);

	    // Pas besoin de re-parcourir toute l'arborescence, on reprend les fichiers d��list�
	    for (   std::list<mfo::CFrame*>::iterator iterFrame = _pAcquisition->GetFrameList()->begin();
	            iterFrame != _pAcquisition->GetFrameList()->end();
	            iterFrame++
	        )
	    {
	        dicom::io::CDicomFile file((*iterFrame)->GetFullPath());
	        file.LoadInputFile();
	        file.Visit(&vConvert);
	    }

	    // On demande au visiteur de convertir (ici en Inrimage)
	    vConvert.Convert(_bCrypt);
	}
	catch(std::exception e)
	{
	    ost << e.what() << std::endl;
	}

	if (ost.str().size() > 1)
	{
		throw dicom::visitor::CDicom::CVisitorException(_sOutputFolder, ost.str());
	}
}
#endif

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
std::vector<mfo::CPatient*> * CFacadeDicom::GetVectorPatient( void )
{
	return this->m_pVisitorList->GetVectorPatient();
}

/**
 *
 ****************************************
 * @param :
 ****************************************
 * @return
 ****************************************
 * @exception
 */
void CFacadeDicom::Clear()
{
    this->DestroyVisitor();
	this->m_pVisitorList = new dicom::visitor::CCreateList;
}

//DicomLib_API const std::string CFacadeDicom::GetError(const dicom::CExceptionDicom * const _pException )
//{
//	return _pException->what();
//}
/*
void CSettingsDicom::setUseAcquisitionDate(const bool _bval)
{
	CSettingsDicom::s_bUseAcquisitionDate = _bval;
}

void CSettingsDicom::setUseSeriesUID(const bool _bval)
{
	CSettingsDicom::s_bUseSeriesUID = _bval;
}

void CSettingsDicom::setUseStudyUID(const bool _bval)
{
	CSettingsDicom::s_bUseStudyUID = _bval;
}

void CSettingsDicom::setUseModality(const bool _bval)
{
	CSettingsDicom::s_bUseAcquisitionDate = _bval;
}

void CSettingsDicom::setUseFramePosition(const bool _bval)
{
	CSettingsDicom::s_bUseAcquisitionDate = _bval;
}
*/
} //namespace dicom
