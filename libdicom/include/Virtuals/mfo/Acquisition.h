#ifndef MFO_ACQUISITION_H
#define MFO_ACQUISITION_H
/** \class CStudy
 * Acquisition Medical Firmware Object.
 * This function is defines in all inline.
 * This class can be used (by derivation)
 * in all projects need Medical Firmware Object.
 */

// STL basic type
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <ctime>

// Assertions
#include <cassert>

// VIRTUALS basic type
#include "Virtuals/Type.h"

// Frame
#include "Virtuals/mfo/Frame.h"
#include "Virtuals/mfo/Reconstruction.h"

namespace mfo
{
class CStudy;

/**
 *
 */
class CYMDDate : public std::time_get_byname<char>
{
public:
	/**
	 *
	 */
	CYMDDate (const char *_szName) : std::time_get_byname<char> (_szName)
	{
	}

protected:
	/**
	 *
	 */
	virtual std::time_base::dateorder do_date_order () const
	{
		return std::time_base::ymd;
	}
};


/** \class CAcquisition
 *********************
 * Namespace : mfo
 *********************
 * This class defines a medical firmware
 * object. CAcquisition represents fundamental
 * object in the patient's acquistion. This class
 * defines all informations of acquistion
 * and informations picture and reconstruction
 */
class CAcquisition
{

public :
	/**
	 * Constructor with four paramater.
	 * Initialize all acquisition data member.
	 *********************
	 * @param const uint32          : index of this acquisition
	 * @param const std::string&    : date of this acquisition
	 * @param dicom::CStudy*		: study which contains this acquisition
	 * @param const std::string&    : image type in this acquisition
	 *********************
	 * @pre parent (study) exists.
	 * @post parent member is initialized, pixel vector is created
	 */
	CAcquisition::CAcquisition(
			const uint32		_ui32AcquisitionNumber,
			const std::string&	_sDate,
			mfo::CStudy*		_pStudy,
			const std::string&	_sImageType,
			const std::string&	_sUID
	            ):
		m_ui32Height			( 0 ),
		m_ui32Width				( 0 ),
		m_ui8BitsPerPixel		( 8 ),
		m_dSliceThickness		( 0.0 ),
		m_ui8Axe				( 4 ),
		m_bUnsignedFlag			( false ),
		m_ui32AcquisitionIndex	( _ui32AcquisitionNumber ),
		m_vectorVoxelSize		( 3, 0.0 ),
		m_strImageType			( _sImageType ),
		m_strImageFormat		( "unknown" ),
		m_strDate				( _sDate ),
		m_listFrame				( 0 ),
		m_bIsMain				( true ),
		m_bIsNormalDir			( true ),
		m_vectorReconstruction	( 0 ),
		m_strUID				( _sUID ),
		m_strPath				( "unknown" ),
		m_i32DbID				( -1 ),
		m_ui32LaboID			( 0 ),
		m_ui32NetID				( 0 ),
		m_stringDateSendLabo	( "" ),
		m_stringDateReceiveLabo	( "" ),
		m_stringDateSendBDD		( "" )
	{
		this->m_pStudy					= _pStudy;

		// Reconstructions are probably used after the call of this constructor
		m_vectorReconstruction.reserve( 20 );

		assert(this->m_ui8BitsPerPixel==16 || this->m_ui8BitsPerPixel==8);
		assert(this->m_vectorVoxelSize.size() == 3);
		assert(this->GetVoxelSize()[0]	== 0.0	) ;
		assert(this->GetVoxelSize()[1]	== 0.0	) ;
		assert(this->GetVoxelSize()[2]	== 0.0	) ;
		assert(this->m_pStudy != NULL);
//		assert (this->IsRightDateFormat (_sDate)); // TODO : Add semantic on date element.
	}


	/**
	 * Constructor with all paramaters
	 * Initialize all acquisition data member.
	 *********************
	 * @param const std::string&    : date of this acquisition
	 * @param const uint32          : index of this acquisition
	 * @param const std::string&	: image type (VENTRALE, DORSALE, ...) of this acquisition
	 * @param const std::string&	: image format (INR, INR.GZ, ...) of this acquisition
	 * @param const std::string&	: image storage filename
	 * @param const bool			: true if this acquisition is the main one
	 * @param const bool			: true if the reading sens is not inversed
	 * @param const double			: the X voxel size
	 * @param const double			: the Y voxel size
	 * @param const double			: the Z voxel size
	 * @param const double			: the slice thickness
	 * @param const CStudy*			: the parent study
	 *********************
	 * @post pixel vector is created
	 */
	CAcquisition::CAcquisition(
			const std::string&	_sDate,
			const uint32		_ui32AcquisitionIndex,
			const std::string&  _sImageType,
			const std::string&	_sImageFormat,
			const std::string&	_sPath,
			const bool			_bIsMain,
			const bool			_bIsNormalDir,
			const double		_dXVoxSize,
			const double		_dYVoxSize,
			const double		_dZVoxSize,
			const double		_dSliceThickness,
			const uint32		_ui32UID,
			mfo::CStudy *		_pStudy
	            ):
		m_ui32Height			( 0 ),
		m_ui32Width				( 0 ),
		m_ui8BitsPerPixel		( 8 ),
		m_dSliceThickness		( _dSliceThickness ),
		m_ui8Axe				( 4 ),
		m_bUnsignedFlag			( false ),
		m_ui32AcquisitionIndex	( _ui32AcquisitionIndex ),
		m_vectorVoxelSize		( 3, 0.0 ),
		m_strImageType			( _sImageType ),
		m_strImageFormat		( _sImageFormat ),
		m_strDate				( _sDate ),
		m_listFrame				( 0 ),
		m_bIsMain				( _bIsMain ),
		m_bIsNormalDir			( _bIsNormalDir ),
		m_vectorReconstruction	( 0 ),
		m_strUID				( "unknown" ),
		m_strPath				( _sPath ),
		m_i32DbID				( -1 ),
		m_ui32LaboID			( _ui32UID ),
		m_ui32NetID				( 0 ),
		m_stringDateSendLabo	( "" ),
		m_stringDateReceiveLabo	( "" ),
		m_stringDateSendBDD		( "" )
	{
		this->m_listFrame				= std::list		<mfo::CFrame*>			(0);
		this->m_vectorReconstruction	= std::vector	<mfo::CReconstruction*>	(0);
		this->m_vectorVoxelSize			= std::vector	<double>				(3);
		this->m_vectorVoxelSize[0]		= _dXVoxSize;
		this->m_vectorVoxelSize[1]		= _dYVoxSize;
		this->m_vectorVoxelSize[2]		= _dZVoxSize;
		m_pStudy = _pStudy;

		// Reconstructions are probably used after the call of this constructor
		this->m_vectorReconstruction.reserve(20);

		assert(this->m_ui8BitsPerPixel==16 || this->m_ui8BitsPerPixel==8);
		assert(this->m_vectorVoxelSize.size() == 3);
		assert(this->GetVoxelSize()[0]	== _dXVoxSize	) ;
		assert(this->GetVoxelSize()[1]	== _dYVoxSize	) ;
		assert(this->GetVoxelSize()[2]	== _dZVoxSize	) ;
		assert(this->m_pStudy != NULL);
//		assert (this->IsRightDateFormat (_sDate));	// TODO : Add semantic on date element.
	}

	/**
	 * Constructor with nothing paramaters
	 * Initialize all acquisition data member.
	 *********************
	 * @param
	 */
	CAcquisition( void ):
		m_ui32Height			( 0 ),
		m_ui32Width				( 0 ),
		m_ui8BitsPerPixel		( 8 ),
		m_dSliceThickness		( 0.0 ),
		m_ui8Axe				( 4 ),
		m_bUnsignedFlag			( false ),
		m_ui32AcquisitionIndex	( 0 ),
		m_vectorVoxelSize		( 3, 0.0 ),
		m_strImageType			( "unknown" ),
		m_strImageFormat		( "unknown" ),
		m_strDate				( "unknown" ),
		m_listFrame				( 0 ),
		m_pStudy				( NULL ),
		m_bIsMain				( true ),
		m_bIsNormalDir			( true ),
		m_vectorReconstruction	( 0 ),
		m_strUID				( "unknown" ),
		m_strPath				( "unknown" ),
		m_i32DbID				( -1 ),
		m_ui32LaboID			( 0 ),
		m_ui32NetID				( 0 ),
		m_stringDateSendLabo	( "" ),
		m_stringDateReceiveLabo	( "" ),
		m_stringDateSendBDD		( "" )
	{
		// Reconstructions are probably used after the call of this constructor
		m_vectorReconstruction.reserve( 20 );

		assert(this->m_ui8BitsPerPixel==16 || this->m_ui8BitsPerPixel==8);
		assert(this->m_vectorVoxelSize.size() == 3);
		assert(this->GetVoxelSize()[0]	== 0.0	) ;
		assert(this->GetVoxelSize()[1]	== 0.0	) ;
		assert(this->GetVoxelSize()[2]	== 0.0	) ;
		assert(this->m_pStudy == NULL);
	}

	/**
	 * Destructor.
	 * Clear the set of image.
	 *********************
	 */
	~CAcquisition()
	{
		this->ClearFrame();
		this->ClearReconstruction();
	}

	/**
	 * clear the frame list.
	 *********************
	 * @post frame list clear
	 */
	void ClearFrame( void )
	{
		for(
			std::list<mfo::CFrame*>::reverse_iterator iterDelete = this->m_listFrame.rbegin();
			iterDelete != this->m_listFrame.rend();
			iterDelete++
		   )
		{
			assert (*iterDelete != NULL);
			delete *iterDelete;
			*iterDelete = NULL;
			assert (*iterDelete == NULL);
		}
		this->m_listFrame.clear();
		assert (m_listFrame.size() == 0);
	}

	/**
	 * clear the reconstruction list.
	 *********************
	 * @post reconstruction vector clear
	 */
	void ClearReconstruction( void )
	{
		for(
			std::vector<mfo::CReconstruction*>::reverse_iterator iterDeleteRec = m_vectorReconstruction.rbegin();
			iterDeleteRec != m_vectorReconstruction.rend();
			iterDeleteRec ++
		)
		{
			assert(*iterDeleteRec != NULL);
			delete *iterDeleteRec;
			*iterDeleteRec = NULL;
			assert(*iterDeleteRec == NULL);
		}
		this->m_vectorReconstruction.clear();
		assert(this->m_vectorReconstruction.size() == 0);
	}

	/**
	 * @return const bool : the flag indicating if the image elements are signed (true) or not
	 */
	const bool GetUnsignedFlag(void) const
	{
		return this->m_bUnsignedFlag;
	}

	/**
	 * @param const bool : the flag indicating if the image elements are signed (true) or not
	 */
	void SetUnsignedFlag(const bool _bFlag)
	{
		this->m_bUnsignedFlag = _bFlag;
	}

	/**
	 * @return const uint32 : the image width
	 */
	const uint32 GetWidth( void ) const
	{
		return this->m_ui32Width;
	}

	/**
	 * @param const uint32 : the image width
	 */
	void SetWidth(const uint32 _uiWidth)
	{
		this->m_ui32Width = _uiWidth;
	}

	/**
	 * @return const uint32 : the image height
	 */
	const uint32 GetHeight( void ) const
	{
		return this->m_ui32Height;
	}

	/**
	 * @param const uint32 : the image height
	 */
	void SetHeight(const uint32 _uiHeight)
	{
		this->m_ui32Height = _uiHeight;
	}

	/**
	 * @return const uint32 : the number of bits per pixel
	 */
	const uint8 GetBitsPerPixel( void ) const
	{
		return this->m_ui8BitsPerPixel;
	}

	/**
	 * @return const uint32 : the number of bytes per pixel
	 */
	const uint8 GetBytesPerPixel( void ) const
	{
		return this->m_ui8BitsPerPixel/8;
	}

	/**
	 * @param const uint32 : set the number of bits (and bytes) per pixel
	 */
	void SetBitsPerPixel(const uint8 _ui8BitsPerPixel)
	{
		this->m_ui8BitsPerPixel = _ui8BitsPerPixel;
		assert(this->m_ui8BitsPerPixel==16 || this->m_ui8BitsPerPixel==8);
	}

	/**
	 * @return const double : the slice thickness
	 */
	const double GetSliceThickness( void ) const
	{
		return this->m_dSliceThickness;
	}

	/**
	 * @param const double : the slice thickness
	 */
	void SetSliceThickness(const double _dSliceThickness)
	{
		this->m_dSliceThickness = _dSliceThickness;
	}

	/**
	 * @return const std::vector<double>& : the pixel size (x, y, z)
	 */
	const std::vector<double> & GetVoxelSize( void ) const
	{
		return this->m_vectorVoxelSize;
	}

	/**
	 * @param const std::vector<double>& : the voxel size (x, y, z)
	 */
	void SetVoxelSize(const std::vector<double> & _vVoxelSize)
	{
		assert( this->m_vectorVoxelSize.size() == 3 );
		this->m_vectorVoxelSize = _vVoxelSize;
	}

	/**
	 * @param const double& : the space between two slice (vz voxel)
	 */
	void SetSpaceBetweenSlice(const double & _dSpaceBetweenSlice)
	{
		assert( this->m_vectorVoxelSize.size() == 3 );
		this->m_vectorVoxelSize[2] = _dSpaceBetweenSlice;
	}

	/**
	 * @return uint8 : the axe
	 */
	const uint8 GetAxe( void ) const
	{
		return this->m_ui8Axe;
	}

	/**
	 * @param uint8 : the axe
	 */
	void SetAxe(const uint8 _uiAxe)
	{
		this->m_ui8Axe = _uiAxe;
	}

	/**
	 * @return const uint32 : the acquisition index
	 */
	const int32 GetIndex( void ) const
	{
		return this->m_ui32AcquisitionIndex;
	}

	/**
	 * @param const uint32 : the acquisition index
	 */
	void SetIndex (const uint32 _ui32Index)
	{
		this->m_ui32AcquisitionIndex = _ui32Index;
	}

	/**
	 * @return const std::string& : the image type
	 */
	const std::string & GetImageType( void ) const
	{
		return this->m_strImageType;
	}

	/**
	 * @param const std::string& : the image type
	 */
	void SetImageType (const std::string& _sImageType)
	{
		this->m_strImageType = _sImageType;
	}

	/**
	 * @return const std::string& : the image format
	 */
	const std::string & GetImageFormat ( void ) const
	{
		return m_strImageFormat;
	}

	/**
	 * @param const std::string& : the image format
	 */
	void SetImageFormat ( const std::string & _sImageFormat )
	{
		this->m_strImageFormat = _sImageFormat;
	}

	/**
	 * @return const std::string& : the inrimage path
	 */
	const std::string& GetPath () const
	{
		return this->m_strPath;
	}

	/**
	 * @param const std::string& : the inrimage path
	 */
	void SetPath (const std::string& _sPath)
	{
		this->m_strPath = _sPath;
	}

	/**
	 * @return const std::string& : the acquisition UID
	 */
	const std::string & GetSerieUID( void ) const
	{
		return this->m_strUID;
	}

	/**
	 * @param const std::string& : the acquisition UID
	 */
	void SetSerieUID(const std::string& _sUID)
	{
		this->m_strUID = _sUID;
	}

	/**
	 * @return std::string& : the acquisition date
	 */
	const std::string & GetDate( void ) const
	{
		return this->m_strDate;
	}

	/**
	 * @param std::string& : the acquisition date
	 */
	void SetDate(const std::string& _sDate)
	{
		this->m_strDate = _sDate;
	}

	/**
	 * @return std::string& : the acquisition date
	 */
	const std::string & GetTime( void ) const
	{
		return this->m_stringTime;
	}

	/**
	 * @param std::string& : the acquisition date
	 */
	void SetTime(const std::string& _sTime)
	{
		this->m_stringTime = _sTime;
	}

	/**
	 * @return const mfo::CStudy* : the study from which comes this acquisition
	 */
	mfo::CStudy * const GetStudy( void ) const
	{
		return this->m_pStudy;
	}

	/**
	 * @return std::list<mfo::CFrame*>* : the frame list
	 */
	std::list<mfo::CFrame*> * GetFrameList ( void )
	{
		return &(this->m_listFrame);
	}

	/**
	 * @return const bool : true if the acquisition has the good number of bits per pixel
	 */
	const bool IsGoodBitsPerPixel( void ) const
	{
		return (this->m_ui8BitsPerPixel == 8 || this->m_ui8BitsPerPixel == 16) ;
	}

	/**
	 * @return const bool : true if this acquisition is the main one
	 */
	const bool IsMain () const
	{
		return this->m_bIsMain;
	}

	/**
	 * @param const bool : true if this acquisition is the main one
	 */
	void SetIsMain (const bool _bIsMain)
	{
		this->m_bIsMain = _bIsMain;
	}

	/**
	 * @return const bool : true if the reading direction is NOT invertted
	 */
	const bool IsNormalDir () const
	{
		return this->m_bIsNormalDir;
	}

	/**
	 * @param const bool : true if the reading direction is NOT inverted
	 */
	void SetIsNormalDir (const bool _bIsNormalDir)
	{
		this->m_bIsNormalDir = _bIsNormalDir;
	}

	/**
	 * @return const int32 :
	 */
	const int32 GetDbID(void) const
	{
		return this->m_i32DbID;
	}

	/**
	 * @param const int32 :
	 */
	void SetDbID (const int32 _i32DbID)
	{
		assert (_i32DbID >= 0);
		this->m_i32DbID = _i32DbID;
	}

	/**
	 * @return const int32 :
	 */
	const uint32 GetLaboID(void) const
	{
		return this->m_ui32LaboID;
	}

	/**
	 * @param const int32 :
	 */
	void SetLaboID (const uint32 _ui32LaboID)
	{
		this->m_ui32LaboID = _ui32LaboID;
	}

	/**
	 * @return const int32 :
	 */
	const uint32 GetNetID(void) const
	{
		return this->m_ui32NetID;
	}

	/**
	 * @param const int32 :
	 */
	void SetNetID (const uint32 _ui32NetID)
	{
		this->m_ui32NetID = _ui32NetID;
	}

	/**
	 * @return const uint32 : size of frame list
	 */
	const uint32 GetFrameNumber(void) const
	{
		return static_cast<uint32>(this->m_listFrame.size());
	}

	/**
	 * @return vector<mfo::CReconstruction*>* : reconstructions list
	 */
	std::vector<mfo::CReconstruction*> * GetVectorReconstruction(void)
	{
		return &(this->m_vectorReconstruction);
	}

	/**
	 * @return const uint32 : the size of the reconstruction list
	 */
	const uint32 GetNumberReconstruction ( void ) const
	{
		return static_cast<uint32>(this->m_vectorReconstruction.size());
	}

	/**
	 * @return const std::string : date of send inrimage to the laboratory
	 */
	const std::string GetDateSendLabo ( void ) const
	{
		return this->m_stringDateSendLabo;
	}

	/**
	 * param const std::string : date of send inrimage to the laboratory
	 */
	void SetDateSendLabo ( const std::string _stringDateSendLabo )
	{
		this->m_stringDateSendLabo = _stringDateSendLabo;
	}

	/**
	 * @return const std::string : date of receive reconstruction from the laboratory
	 */
	const std::string GetDateReceiveLabo ( void ) const
	{
		return this->m_stringDateReceiveLabo;
	}

	/**
	 * param const std::string : date of receive reconstruction from the laboratory
	 */
	void SetDateReceiveLabo ( const std::string _stringDateReceiveLabo )
	{
		this->m_stringDateReceiveLabo = _stringDateReceiveLabo;
	}

	/**
	 * @return const std::string : date of send all date to the BDD
	 */
	const std::string GetDateSendBDD ( void ) const
	{
		return this->m_stringDateSendBDD;
	}

	/**
	 * param const std::string : date of send all date to the BDD
	 */
	void SetDateSendBDD ( const std::string _stringDateSendBDD )
	{
		this->m_stringDateSendBDD = _stringDateSendBDD;
	}

	/**
	 * @return const std::string : date of reconstruction disponibility in the MEDICALAB
	 */
	const std::string GetDateDispo ( void ) const
	{
		return this->m_stringDateDispoReconstruction;
	}

	/**
	 * param const std::string : date of reconstruction disponibility in the MEDICALAB
	 */
	void SetDateDispo ( const std::string _stringDateDispoReconstruction )
	{
		m_stringDateDispoReconstruction = _stringDateDispoReconstruction;
	}

	/**
	 * Text serialize
	 *********************
	 * @param std::ostream& _oss : Previous stream
	 * @param mfo::CAcquisition& _cAcquisition : Acquisition to serialze
	 *********************
	 * @return std::ostream& : Output stream
	 */
	friend std::ostream& operator<<(std::ostream & _oss, mfo::CAcquisition& _cAcquisition)
	{
		_oss << "|| ------------------ >> Image height : " << _cAcquisition.GetHeight() << std::endl
			<< "|| ------------------ >> Image width : " << _cAcquisition.GetWidth() << std::endl
			<< "|| ------------------ >> Image bytes per pixel : " << _cAcquisition.GetBytesPerPixel() << std::endl
			<< "|| ------------------ >> Image bits per pixel : " << _cAcquisition.GetBitsPerPixel() << std::endl
			<< "|| ------------------ >> Image slice thickness : " << _cAcquisition.GetSliceThickness() << std::endl
			<< "|| ------------------ >> Image axe move : " << (uint32)_cAcquisition.GetAxe() << std::endl
			<< "|| ------------------ >> Image unsigned flag : " << (_cAcquisition.GetUnsignedFlag()?true:false) << std::endl
			<< "|| ------------------ >> Image voxel size : " <<
										_cAcquisition.GetVoxelSize()[0] << " | " <<
										_cAcquisition.GetVoxelSize()[1] << " | " <<
										_cAcquisition.GetVoxelSize()[2] << " | "  << std::endl
			<< "|| ------------------ >> Acquisition date : " << _cAcquisition.GetDate() << std::endl
			<< "|| ------------------ >> Acquisition type : " << _cAcquisition.GetImageType() << std::endl
			<< "|| ------------------ >> Acquisition format : " << _cAcquisition.GetImageFormat() << std::endl
			<< "|| ------------------ >> Acquisition index : " << _cAcquisition.GetIndex() << std::endl
			<< "|| ------------------ >> Number Image : " <<  _cAcquisition.GetFrameNumber() << std::endl
			<< "|| ------------------ >> Number is good bits per pixel: " << _cAcquisition.IsGoodBitsPerPixel() << std::endl
			<< "|| ------------------ >> Acquisition is the main and is in normal dir : " << (_cAcquisition.IsMain()?true:false)  << std::endl
			<< "|| ------------------ >> Acquisition is the normal dir : " << (_cAcquisition.IsNormalDir()?true:false) << std::endl
			<< "|| ------------------ >> Serie UID : " << _cAcquisition.GetSerieUID() << std::endl
			<< "|| ------------------ >> Date send to laboratory : " << _cAcquisition.GetDateSendLabo() << std::endl
			<< "|| ------------------ >> Labo Id : " << _cAcquisition.GetLaboID() << std::endl
			<< "|| ------------------ >> Acquisition DBId : " << _cAcquisition.GetDbID() << std::endl
			<< "|| ------------------ >> Image path : " << _cAcquisition.GetPath() << std::endl;


		for(
			std::vector<mfo::CReconstruction*>::reverse_iterator iter = _cAcquisition.GetVectorReconstruction()->rbegin();
			iter!= _cAcquisition.GetVectorReconstruction()->rend();
			iter++
		)
		{
			_oss << (**iter);
		}
		for(
			std::list<mfo::CFrame*>::reverse_iterator iterFrame = _cAcquisition.GetFrameList()->rbegin();
			iterFrame!= _cAcquisition.GetFrameList()->rend();
			iterFrame++
		)
		{
			_oss << (**iterFrame);
		}
		_oss << "|| ------------------ >> ------------------------------------------------ " << std::endl;

		return _oss;
	}

protected :

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CAcquisition( const CAcquisition & )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CAcquisition  &operator=( const CAcquisition & )
	{
		assert(false);
		return *this;
	}

	/*
	 **************
	 * IMAGE INFO
	 **************
	 */

	/**
	 * Height commun for all picture (frame). For exemple 256
	 */
	uint32				m_ui32Height;

	/**
	 * Width commun for all picture (frame). For exemple 256
	 */
	uint32				m_ui32Width;

	/**
	 * Bits per pixel for all picture (frame). A this time,
	 * value is 8 or 16 (bits)
	 */
	uint8				m_ui8BitsPerPixel;

	/**
	 * Thickness of each slices. (eg : 1.0)
	 */
	double				m_dSliceThickness;

	/**
	 * Acquisition axe. (value : 0, 1 or 2)
	 */
	uint8				m_ui8Axe;

	/**
	 * If this acquisition use an unsigned value.
	 */
	bool				m_bUnsignedFlag;

	/*
	 **************
	 * FRAMEWORK MEMBER OBJECT
	 **************
	 */

	/**
	 * Index of this acquisition. (value : 0,1,2,3,4,5,6....)
	 */
	uint32					m_ui32AcquisitionIndex;

	/**
	 * Size (3d) for each voxel. eg 0.83 0.83 0.83
	 * Usually it's a cube.
	 */
	std::vector<double>		m_vectorVoxelSize;

	/**
	 * Type picture. eg : ORIGINAL\PRIMARY\OTHER
	 */
	std::string				m_strImageType;

	/**
	 * picture format. eg : INR, INR.GZ, INR.GZ.GPG
	 */
	std::string				m_strImageFormat;

	/**
	 * Acquisition date. eg 20010913
	 */
	std::string				m_strDate;

	/**
	 * Acquisition time.
	 */
	std::string				m_stringTime;

	/**
	 * Frame(slice, picture ...) list.
	 */
	std::list<mfo::CFrame*>	m_listFrame;

	/**
	 * Study which contains this acquisition
	 */
	mfo::CStudy*			m_pStudy;

	/**
	 * Flag indicating if this acquisition is the main one
	 */
	bool					m_bIsMain;

	/**
	 * Flag indicating if the reading sens of the inrimage is normal (true) or inverted (false)
	 */
	bool					m_bIsNormalDir;

	/**
	 * Vector of reconstructions
	 */
	std::vector<mfo::CReconstruction*> m_vectorReconstruction;

	/*
	 **************
	 * USED IN THE DICOM PARSER
	 **************
	 */

	/**
	 * Serie UID
	 */
	std::string			m_strUID;

	/*
	 **************
	 * USED IN DATABASE
	 **************
	 */

	/**
	 * Path image 3D file (Actually : the inrimage path)
	 */
	std::string			m_strPath;

	/**
	 * Database indentifier
	 */
	int32				m_i32DbID;

	/**
	 * MEDICALAB ID
	 */
	uint32				m_ui32LaboID;

	/**
	 * temporary id (used in radiology only)
	 */
	uint32				m_ui32NetID;

	/**
	 * Date where this acquisition has been send to labo
	 */
	std::string			m_stringDateSendLabo;

	/**
	 * Date where this acquisition has been received from labo
	 */
	std::string			m_stringDateReceiveLabo;

	/**
	 * Date where this acquisition has been send to database
	 */
	std::string			m_stringDateSendBDD;

	/**
	 * Date of reconstruction disponibility in the MEDICALAB
	 */
	std::string			m_stringDateDispoReconstruction;

	/**
	 *
	 * Check if the given date is in format YYYY/MM/DD
	 *
	 * @param const std::string& _sDate : date to check
	 *
	 * @return const bool : true if good format; false if not
	 *
	 */
	const bool IsRightDateFormat (const std::string& _sDate)
	{
		std::stringstream ssDate (_sDate);

		std::ios_base::iostate state = std::ios_base::goodbit;
/*
		struct tm tmDate;
#ifdef _MSC_VER
		std::locale loc = std::_ADDFAC (std::locale (""), new CYMDDate (""));
#else
		std::locale loc (std::locale ("en_US"), new CYMDDate (""));
#endif

#ifdef _MSC_VER
		const std::time_get<char>& tg = std::_USE (loc, std::time_get<char>);
#else
		const std::time_get<char>& tg = std::use_facet (loc, (std::time_get<char>*)0, false);
#endif

		std::time_get<char>::iter_type begin (ssDate);
		std::time_get<char>::iter_type end;

		tg.get_date (begin, end, ssDate, state, &tmDate);
*/
		return (!state);
	}
};

}//end namespace 'mfo'

#endif // MFO_ACQUISITION_H
