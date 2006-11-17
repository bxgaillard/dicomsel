#ifndef MFO_PATIENT_H
#define MFO_PATIENT_H
/** \class CPatient
 * Patient Medical Firmware Object.
 * This function is defines in all inline.
 * This class can be used (by derivation)
 * in all projects need Medical Firmware Object.
 */

// STL's vector
#include <vector>
#include <iostream>
#include <ostream>

// Assertions
#include <cassert>

// Basic Virutals Type (macro & integer)
#include "Virtuals/Type.h"

// Study
#include "Virtuals/mfo/Study.h"

// Medical Firmware Object
namespace mfo
{

/** \class CPatient
 *********************
 * Namespace : mfo
 *********************
 * This class defines a medical firmware
 * object. CPatient represents fundamental
 * object in the patient folder. This class
 * defines patient's name, sex, Birthdate ..
 */
class CPatient
{
public:

	/**
	 * Constructor with four params.
	 * Used by the dicom parser.
	 *********************
	 * @pre name length must be superior at 0
	 * @pre sex must be 'M' or 'F'
	 *********************
	 * @param const std::string & _strName		: FullName (Dicom format !!)
	 * @param const std::string & _strIDDicom	: ID Dicom
	 * @param const std::string & _strBirthdate	: Birthdate (Dicom format !!)
	 * @param const int8 _i8Sex		: 'M' or 'F'
	 */
	CPatient(
				const std::string & _strName,
				const std::string & _strIDDicom,
				const std::string & _strBirthdate,
				const int8 _i8Sex
			):
		m_strIDDicom    (_strIDDicom),
		m_strBirthdate	(_strBirthdate),
		m_vectorStudy   (0),
		m_i32DbID		(-1)
	{
		this->SetFullName(_strName);
//		assert(_i8Sex == 'M' || _i8Sex == 'F');
		this->m_bSex = (_i8Sex == 'M')?true:false;
//		this->m_vectorStudy.reserve(1);

		this->SetParentBase ("");

		assert(m_strName.length() > 0);
	}

	/**
	 * Constructor with six params.
	 * Used by the database application.
	 *********************
	 * @param const std::string & _strName		: Patient's name
	 * @param const std::string & _strFirstname	: Patient's first name
	 * @param const std::string & _strIDDicom	: ID Dicom
	 * @param const std::string & _strBirthdate	: Birthdate (Dicom format !!)
	 * @param const bool _bSex					: Male is true
	 * @param const std::string & _sParentBase	: eventual parent database. "" for local database
	 */
	CPatient(
				const std::string & _strName,
				const std::string & _strIDDicom,
				const std::string & _strFirstname,
				const std::string & _strBirthdate,
				const bool _bSex,
				const std::string _sParentBase
			):
		m_strName		(_strName),
		m_strFirstname	(_strFirstname),
		m_strIDDicom    (_strIDDicom),
		m_strBirthdate	(_strBirthdate),
		m_bSex			(_bSex),
		m_sParentBase	(_sParentBase),
		m_vectorStudy   (0)
	{
		this->m_vectorStudy.reserve(1);

		assert(m_strName.length() > 0);
	}


	/**
	 * Constructor with three params.
	 * Used to copy a total acquisition (acq + study + patient but
	 * without copy child objects)
	 *********************
	 * @param const CPatient * const _pPatient		:
	 * @param const CStudy * const _pStudy			:
	 * @param CAcquisition * const _pAcquisition	:
	 */
	CPatient(	const mfo::CPatient * const _pPatient,
						const mfo::CStudy * const _pStudy,
						const mfo::CAcquisition * const _pAcquisition)
	{
		assert(_pPatient!=NULL);
		this->m_strName			= _pPatient->GetName();
		this->m_strFirstname	= _pPatient->GetFirstname();
		this->m_strIDDicom		= _pPatient->GetIDDicom();
		this->m_strBirthdate	= _pPatient->GetBirthdate();
		this->m_bSex			= _pPatient->GetIsMale();
		this->m_sParentBase		= _pPatient->GetParentBase ();

		this->m_vectorStudy.reserve(1);

		assert(m_strName.length() > 0);

		if (_pPatient->GetDbID() != -1) this->SetDbID(_pPatient->GetDbID());

		if (_pStudy!=NULL)
		{
			mfo::CStudy* pStudy = new mfo::CStudy(
									_pStudy->GetUID(),
									this,
									_pStudy->GetHospital(),
									_pStudy->GetModality(),
									_pStudy->GetAcquisitionZone(),
									_pStudy->GetRISID()
									);
			if (_pStudy->GetDbID() != -1) pStudy->SetDbID(_pStudy->GetDbID());

			if (_pAcquisition!=NULL)
			{
				mfo::CAcquisition* pAcquisition = new mfo::CAcquisition(
														_pAcquisition->GetIndex(),
														_pAcquisition->GetDate(),
														pStudy,
														_pAcquisition->GetImageType(),
														_pAcquisition->GetSerieUID()
														);
				pAcquisition->SetHeight(_pAcquisition->GetHeight());
				pAcquisition->SetWidth(_pAcquisition->GetWidth());
				pAcquisition->SetBitsPerPixel(_pAcquisition->GetBitsPerPixel());
				pAcquisition->SetSliceThickness(_pAcquisition->GetSliceThickness());
				pAcquisition->SetAxe(_pAcquisition->GetAxe());
				pAcquisition->SetUnsignedFlag(_pAcquisition->GetUnsignedFlag());
				pAcquisition->SetVoxelSize(_pAcquisition->GetVoxelSize());
				pAcquisition->SetSpaceBetweenSlice(_pAcquisition->GetVoxelSize()[2]);
				pAcquisition->SetImageFormat(_pAcquisition->GetImageFormat());
				pAcquisition->SetIsMain(_pAcquisition->IsMain());
				pAcquisition->SetIsNormalDir(_pAcquisition->IsNormalDir());
				pAcquisition->SetPath(_pAcquisition->GetPath());
				pAcquisition->SetNetID(_pAcquisition->GetNetID());
				pAcquisition->SetDateSendLabo(_pAcquisition->GetDateSendLabo());
				pAcquisition->SetDateReceiveLabo(_pAcquisition->GetDateReceiveLabo());
				pAcquisition->SetDateSendBDD(_pAcquisition->GetDateSendBDD());
				pAcquisition->SetLaboID (_pAcquisition->GetLaboID ());

				if (_pAcquisition->GetDbID() != -1) pAcquisition->SetDbID(_pAcquisition->GetDbID());


				for (
						std::list<mfo::CFrame*>::iterator iterFrame = const_cast<mfo::CAcquisition*>(_pAcquisition)->GetFrameList()->begin();
						iterFrame != const_cast<mfo::CAcquisition*>(_pAcquisition)->GetFrameList()->end();
						iterFrame++
					)
				{
					CFrame * pInitFrame = *iterFrame;
					CFrame * pFrame = new CFrame(
													pInitFrame->GetFullPath(),
													pAcquisition,
													pInitFrame->IsLittleEndian()
												);

					pAcquisition->GetFrameList()->push_back(pFrame);
				}

				for (
						std::vector<mfo::CReconstruction*>::iterator iterRec = const_cast<mfo::CAcquisition*>(_pAcquisition)->GetVectorReconstruction()->begin();
						iterRec != const_cast<mfo::CAcquisition*>(_pAcquisition)->GetVectorReconstruction()->end();
						iterRec ++
					)
				{
					CReconstruction *pInitRec = *iterRec;
					CReconstruction *pRecons = new CReconstruction (
													pInitRec->GetOrganName (),
													pInitRec->GetReconstructionFormat (),
													pInitRec->GetPath (),
													pInitRec->GetOrganColor ()[0],
													pInitRec->GetOrganColor ()[1],
													pInitRec->GetOrganColor ()[2],
													pInitRec->GetOrganColor ()[3],
													pInitRec->IsVisible (),
													pAcquisition
												);

					if (pInitRec->GetDbID () != -1) pRecons->SetDbID (pInitRec->GetDbID ());

					pAcquisition->GetVectorReconstruction()->push_back(pRecons);
				}


				pStudy->GetVectorAcquisition()->push_back(pAcquisition);
			}

			this->m_vectorStudy.push_back(pStudy);
		}
	}

	/**
	 * This destructor lists all data of the
	 * m_vectorStudy member. For each data,
	 * it calls the destructor.
	 * This class not used another pointer.
	*/
	~CPatient()
	{
		for(
			std::vector<mfo::CStudy*>::reverse_iterator iterDelete = m_vectorStudy.rbegin();
			iterDelete != m_vectorStudy.rend();
			iterDelete++
		)
		{
			assert(*iterDelete != NULL);
			delete *iterDelete;
			*iterDelete = NULL;
			assert(*iterDelete == NULL);
		}
		this->m_vectorStudy.clear();
		assert(this->m_vectorStudy.size() == 0);
	}

	/**
	 * @return const std::string : Fullname in the DICOM format (TOTO^titi)
	 * -*- A new std::string is created, also it's not a reference which is
	 * return.
	 */
	const std::string GetFullName ( void ) const
	{
	        return (this->m_strFirstname.length() == 0) ?
			this->m_strName.c_str() :
			this->m_strName + "^" + this->m_strFirstname;
	}

	/**
	 * @param const std::string & : set the patient's DICOM fullname (TOTO^titi)
	 */
	void SetFullName(const std::string & _strFullName)
	{
			assert(_strFullName.size() > 0);
	        std::string::size_type sizeBase = std::string::npos;
	        std::string::size_type sizeIndex = _strFullName.find('^');
			if (sizeIndex == sizeBase)
			{
				this->m_strName = _strFullName;
				this->m_strFirstname = "";
				assert( this->m_strName.length() == _strFullName.length() );
			}
			else
			{
				this->m_strName = _strFullName.substr(0, sizeIndex);
				this->m_strFirstname = _strFullName.substr(sizeIndex+1, _strFullName.size());
				assert( this->m_strName.length() + this->m_strFirstname.length() + 1 == _strFullName.length() );
			}
	}

	/**
	 * @return const std::string : the patient's name
	 */
	const std::string & GetName ( void ) const
	{
		return this->m_strName;
	}

	/**
	 * @param const std::string : the patient's name
	 */
	void SetName (const std::string & _strName)
	{
		this->m_strName = _strName;
	}

	/**
	 * @return const std::string : the patient's first name
	 */
	const std::string & GetFirstname ( void ) const
	{
		return this->m_strFirstname;
	}

	/**
	 * @param const std::string : the patient's first name
	 */
	void SetFirstname (const std::string & _strFirstname)
	{
		this->m_strFirstname = _strFirstname;
	}

	/**
	 * @return const std::string & : the patient's Birthdate (eg : 19771010)
	 */
	const std::string & GetBirthdate ( void ) const
	{
		return this->m_strBirthdate;
	}

	/**
	 * @param const std::string : the patient's birth day
	 */
	void SetBirthdate (const std::string & _strBirthdate)
	{
		this->m_strBirthdate = _strBirthdate;
	}

	/**
	 * @return const bool : true if the patient is male, false if not
	 */
	const bool GetIsMale ( void ) const
	{
		return this->m_bSex;
	}

	/**
	 * @param const bool : true if the patient is male, false if not
	 */
	void SetIsMale (const bool _bSex)
	{
		this->m_bSex = _bSex;
	}

	/**
	 * @return const const char : sexe of this patient ('M' or 'P')
	 */
	const char GetSex ( void ) const
	{
		return (this->m_bSex)?'M':'F';
	}

	/**
	 * @param const char : set the patient's sex ('M' or 'F')
	 */
	void SetSex (const char _i8Sex)
	{
		assert(_i8Sex == 'M' || _i8Sex == 'F');
		this->m_bSex = (_i8Sex == 'M')?true:false;
	}

	/**
	 * @return const std::string & : DICOM identifier (eg : 01254312)
	 */
	const std::string & GetIDDicom ( void ) const
	{
		return this->m_strIDDicom;
	}

	/**
	 * @param const std::string & : set the patient's DICOM ID
	 */
	void SetIDDicom(const std::string & _strIDDicom)
	{
		this->m_strIDDicom = _strIDDicom;
	}

	/**
	 * @return const int32 : DB identifier
	 */
	const int32 GetDbID ( void ) const
	{
		return this->m_i32DbID;
	}

	/**
	 * @param const int32 : set the patient's DB Id
	 */
	void SetDbID (const int32 _i32DbID)
	{
		assert (_i32DbID >= 0);
		this->m_i32DbID = _i32DbID;
	}

	/**
	 * @return const std::string : the patient's parent base name
	 */
	const std::string & GetParentBase () const
	{
		return this->m_sParentBase;
	}

	/**
	 * @param const std::string : the patient's parent base name
	 */
	void SetParentBase (const std::string & _sParentBase)
	{
		this->m_sParentBase = _sParentBase;
	}

	/**
	 * @return std::vector<mfo::CStudy*> * : Vector of study
	 */
	std::vector<mfo::CStudy*> * GetVectorStudy ( void )
	{
        	return &(this->m_vectorStudy);
	}

	/**
	 * Text serialize
	 *********************
	 * @param std::ostream& _oss : Previous stream
	 * @param mfo::CPatient& _cPatient : Patient to serialze
	 *********************
	 * @return std::ostream& : Output stream
	 */
	friend std::ostream& operator<<(std::ostream& _oss, mfo::CPatient& _cPatient)
	{
		_oss << "|| >> Name : "		<< _cPatient.GetName()				<< std::endl
			<< "|| >> Firstname : "		<< _cPatient.GetFirstname()			<< std::endl
			<< "|| >> DICOM Name : "	<< _cPatient.GetFullName()			<< std::endl
			<< "|| >> DICOM ID : "		<< _cPatient.GetIDDicom()			<< std::endl
			<< "|| >> Birth date : "	<< _cPatient.GetBirthdate()			<< std::endl
			<< "|| >> Sex : "			<< _cPatient.GetSex()				<< std::endl
			<< "|| >> Patient DBId : "	<< _cPatient.GetDbID()				<< std::endl
			<< "|| >> Study number : "	<< static_cast<unsigned int>(_cPatient.GetVectorStudy()->size()) << std::endl;

		for(
			std::vector<mfo::CStudy*>::reverse_iterator iter = _cPatient.GetVectorStudy()->rbegin();
			iter!= _cPatient.GetVectorStudy()->rend();
			iter++
		)
		{
			_oss << (**iter);
		}
		_oss << "------------------------------------------" << std::endl;

		return _oss;
	}

protected:

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CPatient( void )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CPatient( const CPatient & )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CPatient &operator=( const CPatient & )
	{
		assert(false);
		return *this;
	}

	/**
	 * Patient's name
	 */
	std::string m_strName;

	/**
	 * Patient's first name
	 */
	std::string m_strFirstname;

	/**
	 * DICOM identifiant (eg : 01254312)
	 */
	std::string m_strIDDicom;

	/**
	 * Birthdate (eg : 19771010)
	 */
	std::string m_strBirthdate;

	/**
	 * true if the patient is male, false if not
	 */
	bool m_bSex;

	/**
	 * Patient's parent base name
	 */
	std::string m_sParentBase;

	/**
	 * Vector of study (which contains acquistion and slice ...)
	 */
	std::vector<mfo::CStudy*> m_vectorStudy;

	/*
	 **************
	 * USED IN DATABASE
	 **************
	 */

	/**
	 * Database indentifier
	 */
	int32				m_i32DbID;
};

}//end namespace 'mfo'

#endif // MFO_PATIENT_H


