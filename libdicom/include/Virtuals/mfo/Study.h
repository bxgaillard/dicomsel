#ifndef MFO_STUDY_H
#define MFO_STUDY_H
/** \class CStudy
 * Study Medical Firmware Object.
 * This function is defines in all inline.
 * This class can be used (by derivation)
 * in all projects need Medical Firmware Object.
 */

// STL's vector & string
#include <vector>
#include <string>

// Assertions
#include <cassert>

// Basic Virutals Type (macro & integer)
#include "Virtuals/Type.h"

#include "Virtuals/mfo/Acquisition.h"

namespace mfo
{

class CPatient;

/** \class CStudy
 *********************
 * Namespace : mfo
 *********************
 * This class defines a medical firmware
 * object. CStudy represents fundamental
 * object in the patient's study. This class
 * defines study's hospital, modality, acquisition zone..
 */
class CStudy
{
public:

	/**
	 * Constructor used by the DICOM parser
	 */
	CStudy::CStudy(
		const std::string& _sUID,
		mfo::CPatient* _pPatient,
		const std::string& _sHospital,
		const std::string& _sModality,
		const std::string& _sAcquisitionZone,
		const std::string& _sRISId
	):
		m_sHospital(_sHospital),
		m_sModality(_sModality),
		m_sAcquisitionZone (_sAcquisitionZone),
		m_vectorAcquisition(0),
		m_sRISId (_sRISId),
		m_sUID(_sUID),
		m_i32DbID (-1)
	{
		this->m_pPatient = _pPatient;

		this->m_vectorAcquisition.reserve(20);

		assert(this->m_pPatient != NULL);
		//assert(this->m_sAcquisitionZone.length() > 0); // FIXME !!!!! BDD used this string but it's not always provided
	}

	/**
	 * Constructor used by the database
	 */
	CStudy::CStudy(
		mfo::CPatient* _pPatient,
		const std::string& _sHospital,
		const std::string& _sModality,
		const std::string& _sAcquisitionZone,
		const std::string& _sRISId
	):
		m_sHospital(_sHospital),
		m_sModality(_sModality),
		m_sAcquisitionZone (_sAcquisitionZone),
		m_vectorAcquisition(0),
		m_sRISId (_sRISId),
		m_i32DbID (-1)
	{
		this->m_pPatient = _pPatient;

		this->m_vectorAcquisition.reserve(20);

		assert(this->m_pPatient != NULL);
		//assert(this->m_sAcquisitionZone.length() > 0); // FIXME !!!!! BDD used this string but it's not always provided
	}

	/**
	 * Destructor
	 */
	CStudy::~CStudy()
	{
		for(
			std::vector<mfo::CAcquisition*>::reverse_iterator iterDelete = m_vectorAcquisition.rbegin();
			iterDelete != m_vectorAcquisition.rend();
			iterDelete++
		)
		{
			assert(*iterDelete != NULL);
			delete *iterDelete;
			*iterDelete = NULL;
			assert(*iterDelete == NULL);
		}
		this->m_vectorAcquisition.clear();
		assert(this->m_vectorAcquisition.size() == 0);
	}


	/**
	 * @return std::string& : the hospital where the study was done
	 **/
	const std::string & GetHospital(void) const
	{
		return this->m_sHospital;
	}

	/**
	 * @param const std::string& : the hospital where the study was done
	 **/
	void SetHospital (const std::string& _sHospital)
	{
		this->m_sHospital = _sHospital;
	}

	/**
	 * @return const std::string& : the studie's modality
	 **/
	const std::string & GetModality(void) const
	{
		return this->m_sModality;
	}

	/**
	 * @param const std::string& : the studie's modality
	 **/
	void SetModality (const std::string& _sModality)
	{
		this->m_sModality = _sModality;
	}

	/**
	 * @return const std::string& : the acquisition zone
	 **/
	const std::string & GetAcquisitionZone(void) const
	{
		return this->m_sAcquisitionZone;
	}

	/**
	 * @param const std::string& : the acquisition zone
	 **/
	void SetAcquisitionZone (const std::string& _sZone)
	{
		this->m_sAcquisitionZone = _sZone;
	}

	/**
	 * @return const std::string& : the DICOM UID
	 **/
	const std::string & GetUID(void) const
	{
		return this->m_sUID;
	}

	/**
	 * @param const std::string& : the DICOM UID
	 **/
	void SetUID (const std::string& _sUID)
	{
		this->m_sUID = _sUID;
	}

	/**
	 * @return mfo::CPatient* : the patient who passed this study
	 **/
	mfo::CPatient * GetPatient(void) const
	{
		return this->m_pPatient;
	}

	/**
	 * @return const int32 : the Id of this stury in the database
	 **/
	const int32 GetDbID(void) const
	{
		return this->m_i32DbID;
	}

	/**
	 * @param const int32 : the Id of this study in the database
	 **/
	void SetDbID (const int32 _i32DbID)
	{
		assert (_i32DbID >= 0);
		this->m_i32DbID = _i32DbID;
	}

	/**
	 * @return const std::string& : the RIS Id
	 **/
	const std::string& GetRISID () const
	{
		return this->m_sRISId;
	}

	/**
	 * @param const std::string& : the RIS Id
	 **/
	void SetRISID (const std::string& _sRISId)
	{
		this->m_sRISId = _sRISId;
	}

	/**
	 * @return vector<mfo::CAcquisition*>* : acquisitions list
	 */
	std::vector<mfo::CAcquisition*> * GetVectorAcquisition(void)
	{
		return &(this->m_vectorAcquisition);
	}

	/**
	 * Text serialize
	 *********************
	 * @param std::ostream& _oss : Previous stream
	 * @param mfo::CStudy& _cStudy : Study to serialze
	 *********************
	 * @return std::ostream& : Output stream
	 */
	friend std::ostream& operator<<(std::ostream & _oss, mfo::CStudy& _cStudy)
	{
		_oss	<< "|| --------- >> Hospital : "			<< _cStudy.GetHospital()			<< std::endl
				<< "|| --------- >> Modality : "			<< _cStudy.GetModality()			<< std::endl
				<< "|| --------- >> Acquisition zone : "	<< _cStudy.GetAcquisitionZone()		<< std::endl
				<< "|| --------- >> RIS Id : "				<< _cStudy.GetRISID()				<< std::endl
				<< "|| --------- >> Study UID : "			<< _cStudy.GetUID()					<< std::endl
				<< "|| --------- >> Patient DBId : "		<< _cStudy.GetDbID()				<< std::endl
				<< "|| --------- >> Acquisition number : "	<< static_cast<unsigned int>(_cStudy.GetVectorAcquisition()->size()) << std::endl;

		for(
			std::vector<mfo::CAcquisition*>::reverse_iterator iter = _cStudy.GetVectorAcquisition()->rbegin();
			iter != _cStudy.GetVectorAcquisition()->rend();
			iter++
		)
		{
			 _oss << (**iter);
		}

		return _oss;
	}

protected:

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CStudy				( void )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CStudy				( const CStudy & )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CStudy &operator=	( const CStudy & )
	{
		assert(false);
		return *this;
	}

	/*
	 **************
	 * FRAMEWORK MEMBER OBJECT
	 **************
	 */

	/**
	 * Hospital name (eg : RADIOLOGIE URGENCE CHUV LAUSANNE)
	 */
	std::string m_sHospital;

	/**
	 * Acquisition modality (MT or CT)
	 */
	std::string m_sModality;

	/**
	 * Acquisition Zone (eg : CT ABDOMEN COMPLET)
	 */
	std::string m_sAcquisitionZone;

	/**
	 * Vector of acquisition (which represents an exam)
	 */
	std::vector<mfo::CAcquisition*> m_vectorAcquisition;

	/**
	 * Relative patient...
	 */
	mfo::CPatient* m_pPatient;

	/**
	 * The RIS Id
	 **/
	std::string m_sRISId;

	/*
	 **************
	 * USED IN THE DICOM PARSER
	 **************
	 */

	/**
	 * Unique Identifier DICOM
	 */
	std::string m_sUID;


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

#endif // MFO_STUDY_H
