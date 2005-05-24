#ifndef MFO_RECONSTRUCTION_H
#define MFO_RECONSTRUCTION_H

// STL basic type
#include <string>
#include <vector>

// VIRTUALS basic type
#include "Virtuals/Type.h"

namespace mfo
{   
class CAcquisition;
    
/** \class CReconstruction
 *********************
 * Namespace : mfo
 *********************
 * This class represents a organ reconstruction.
 */
class CReconstruction
{

public :
	/**
	 * Constructor
	 * Initialize all reconstruction data members.
	 *********************  
	 * @param const std::string&	: reconstructed organ name
	 * @param const std::string&	: reconstruction format (trian, ...)
	 * @param const std::string&	: reonstruction file path
	 * @param const double			: the red component of the organ color
	 * @param const double			: the green component of the organ color
	 * @param const double			: the blue component of the organ color
	 * @param const double			: the transparency component of the organ color
	 * @param const bool			: true if the organ is visible
	 * @param mfo::CAcquisition*	: the the acquisition which generated this reconstruction
	 *********************  
	 * @pre parent (acquisition) exists.
	 * @post parent member is initialized, color vector is created
	 */
	CReconstruction::CReconstruction(
					 const std::string&	_sOrganName,
					 const std::string&	_sReconstructionFormat,
					 const std::string&	_sPath,
					 const double _dRed,
					 const double _dGreen,
					 const double _dBlue,
					 const double _dAlpha,
					 const bool	_bIsVisible,
					 mfo::CAcquisition*	_pAcquisition
	            ) :
		m_bIsVisible		(_bIsVisible),
		m_sReconstructionFormat(_sReconstructionFormat),
		m_sOrganName		(_sOrganName),
		m_pAcquisition		(_pAcquisition),
		m_sPath				(_sPath),
		m_i32DbID			(-1)
	{
		assert(_pAcquisition != NULL);

		this->m_vectorOrganColor		= std::vector<double>(4);
		this->m_vectorOrganColor[0]		= _dRed;
		this->m_vectorOrganColor[1]		= _dGreen;
		this->m_vectorOrganColor[2]		= _dBlue;
		this->m_vectorOrganColor[3]		= _dAlpha;

		assert(this->m_vectorOrganColor.size() == 4);
		assert(this->m_pAcquisition != NULL);
	}
	
	/**
	 * Destructor.
	 */
	CReconstruction::~CReconstruction()
	{
	}
	
	/**
	 * @return const std::string& : the organ name
	 */
	const std::string& CReconstruction::GetOrganName (void) const
	{
		return this->m_sOrganName;
	}
	
	/**
	 * @param const std::string& : the organ name
	 */
	void CReconstruction::SetOrganName (const std::string& _sOrganName)
	{
		this->m_sOrganName = _sOrganName;
	}

	/**
	 * @return const std::string& : the reconstruction format
	 */
	const std::string& CReconstruction::GetReconstructionFormat ( void ) const 
	{
		return this->m_sReconstructionFormat;
	}
	
	/**
	 * @param const std::string& : the reconstruction format
	 */
	void CReconstruction::SetReconstructionFormat(const std::string& _sFormat)
	{
		this->m_sReconstructionFormat = _sFormat;
	}

	/**
	 * @return const std::string& : the reconstruction path
	 */
	const std::string& CReconstruction::GetPath ( void ) const 
	{
		return this->m_sPath;
	}
	
	/**
	 * @param const std::string& : the reconstruction path
	 */
	void CReconstruction::SetPath(const std::string& _sPath)
	{
		this->m_sPath = _sPath;
	}

	/**
	 * @return const std::vector<double>& : the organ color (r, g, b, a)
	 */
	const std::vector<double> & CReconstruction::GetOrganColor( void ) const 
	{
		return this->m_vectorOrganColor;
	}
	
	/**
	 * @param const std::vector<double>& : the organ color (r, g, b, a)
	 */
	void CReconstruction::SetOrganColor(const std::vector<double> & _vColor)
	{
		this->m_vectorOrganColor = _vColor;
	}
	
	/**
	 * @return const bool : true if the organ is to be shown
	 */
	const bool CReconstruction::IsVisible (void) const
	{
		return this->m_bIsVisible;
	}	
	
	/**
	 * @param const bool : true if the organ is to be shown
	 */
	void CReconstruction::SetIsVisible(const bool _bIsVisible)
	{
		this->m_bIsVisible = _bIsVisible;
	}	
	
	/**
	 * @return const mfo::CAcquisition* : the acquisition from which comes this reconstruction
	 */
	const mfo::CAcquisition * const CReconstruction::GetAcquisition( void ) const
	{
		return this->m_pAcquisition;
	}
	
	/**
	 * @return const int32 : the Id of this stury in the database
	 */
	const int32 GetDbID(void) const
	{
		return this->m_i32DbID;
	}

	/**
	 * @param const int32 : the Id of this study in the database
	 */
	void SetDbID (const int32 _i32DbID)
	{
		this->m_i32DbID = _i32DbID;
	}
	
	/**
	 * Text serialize
	 *********************
	 * @param std::ostream& _oss : Previous stream
	 * @param mfo::CReconstruction& _cReconstruction : Reconstruction to serialze
	 *********************	
	 * @return std::ostream& : Output stream
	 */	
	friend std::ostream& operator<<(std::ostream & _oss, mfo::CReconstruction& _cReconstruction)
	{
		_oss << "|| --------------------------- >> Organ name : " << _cReconstruction.GetOrganName () << std::endl
			<< "|| --------------------------- >> Image format : " << _cReconstruction.GetReconstructionFormat() << std::endl
			<< "|| --------------------------- >> Is visible : " << (_cReconstruction.IsVisible () ? "true" : "false") << std::endl
			<< "|| --------------------------- >> Organ color : "
			<< _cReconstruction.GetOrganColor ()[0] << " | "
			<< _cReconstruction.GetOrganColor ()[1] << " | "
			<< _cReconstruction.GetOrganColor ()[2] << " | "
			<< _cReconstruction.GetOrganColor ()[3] << std::endl
			<< "|| --------------------------- >> Reconstruction DBId : " << _cReconstruction.GetDbID() << std::endl
			<< "|| --------------------------- >> Reconstruction file path : " << _cReconstruction.GetPath() << std::endl
			<< "|| --------------------------- >> ------------------------------------------------ " << std::endl;

		return _oss;
	}

protected :

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object. 
	 */
	CReconstruction( void )				
	{ 
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object. 
	 */
	CReconstruction( const CReconstruction & )				
	{ 
		assert(false);
	}
	
	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object. 
	 */
	CReconstruction  &operator=( const CReconstruction & )				
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
	 * true if this reconstruction is visible
	 */
	bool				m_bIsVisible;
		
	/**
	 * Color of the reconstructed organ. eg 1.0 0.52 0.3 0.0
	 */
	std::vector<double>		m_vectorOrganColor;
	
	/**
	 * Reconstruction format. eg : TRIAN, TRIAN_GPG
	 */
	std::string			m_sReconstructionFormat;
	
	/**
	 * Organ name
	 */
	std::string			m_sOrganName;
	
	/**
	 * Acquisition which contains this reconstruction
	 */
	mfo::CAcquisition*			m_pAcquisition;

	/*
	 **************	
	 * USED IN DATABASE
	 **************
	 */
	 
	/**
	 * Reconstruction path
	 */
	std::string			m_sPath;

	/**
	 * Database indentifier
	 */
	int32				m_i32DbID;
};

}//end namespace 'mfo'

#endif // MFO_RECONSTRUCTION_H
