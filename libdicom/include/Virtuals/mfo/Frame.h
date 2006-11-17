#ifndef FRAME_H
#define FRAME_H
/** \class CFrame
 * Frame Medical Firmware Object.
 * This function is defines in all inline.
 * This class can be used (by derivation)
 * in all projects need Medical Firmware Object.
 */

// STL's vector
#include <vector>
#include <limits>

// Assertions
#include <cassert>

// Basic Virutals Type (macro & integer)
#include "Virtuals/Type.h"

// min & max defined in winbase.h
#undef min
#undef max
#define POSITION_ERROR (std::numeric_limits<float>::max())

namespace mfo
{
class CAcquisition;

/** \class CFrame
 *********************
 * Namespace : mfo
 *********************
 * This class defines a medical firmware
 * object. CFrame represents fundamental
 * object in one frame of patient's acquisition.
 * This class defines path file of this picture,
 * position in space of this frame ...
 */
class CFrame
{
public:
	/**
	 * Constructor
	 */
	CFrame
	(
		const std::string& _vFullPath,
		mfo::CAcquisition* _pAcquisition
	):
		m_vectorPosition	(3, POSITION_ERROR),
		m_bLittleEndian		(false),
		m_vFullPath			(_vFullPath)
	{
		this->m_pAcquisition = _pAcquisition;
		this->m_pData = NULL;
	}

	/**
	 * Constructor
	 */
	CFrame
	(
		const std::string& _vFullPath,
		mfo::CAcquisition* _pAcquisition,
		const bool _bLittleEndian
	):
		m_vectorPosition	(3, POSITION_ERROR),
		m_bLittleEndian		(_bLittleEndian),
		m_vFullPath			(_vFullPath)
	{
		this->m_pAcquisition = _pAcquisition;
		this->m_pData = NULL;
	}

	/**
	 * Destructor
	 */
	~CFrame()
	{
		if (this->m_pData != NULL)
		{
			assert (this->m_pData != NULL);
			delete[] this->m_pData;
			this->m_pData = NULL;
		}
		assert (this->m_pData == NULL);
		this->m_vFullPath.erase();
		this->m_vectorPosition.clear();
	}

	/**
	 * @return std::string& : Get the path of this frame
	 */
	const std::string & GetFullPath(void) const
	{
		return this->m_vFullPath;
	}

	/**
	 * @param : the path of this frame.
	 */
	void SetFullPath( const std::string & _vFullPath )
	{
		this->m_vFullPath = _vFullPath ;
	}

	/**
	 * @return const bool : Get the endianness of this frame.
	 */
	const bool IsLittleEndian( void ) const
	{
		return this->m_bLittleEndian;
	}

	/**
	 * @param : endianness of this frame.
	 */
	void SetLittleEndian( const bool _bLittleEndian )
	{
		this->m_bLittleEndian = _bLittleEndian;
	}

	/**
	 * @return mfo::CAcquisition* : the acquisition from which comes this frame.
	 */
	mfo::CAcquisition* GetAcquisition( void ) const
	{
		return this->m_pAcquisition;
	}

	/**
	 * @return const double : value of one axe in the vector of the space position of this frame in the patient's acqusition.
	 */
	const double GetPosition(const uint8 ui8Index) const
	{
		return m_vectorPosition[ui8Index];
	}

	/**
	 * @return const std::vector<double>& : vector of the space position of this frame in the patient's acqusition.
	 */
	const std::vector<double>& GetPosition( void ) const
	{
		return m_vectorPosition;
	}

	/**
	 * @param : the space position of this frame in the patient's acqusition.
	 */
	void SetPosition (const std::vector<double>& _vectorPosition)
	{
		this->m_vectorPosition[0] = _vectorPosition[0];
		this->m_vectorPosition[1] = _vectorPosition[1];
		this->m_vectorPosition[2] = _vectorPosition[2];
	}

	/**
	 * Header function to sort frame.
	 * But is the responsibility of the caller to implement algortihm (an exemple is present in DicomLib).
	 *********************
	 * @param const mfo::CFrame& _FrameOrigin : first frame.
	 * @param const mfo::CFrame& _FrameToCompare : second frame.
	 *********************
	 * @return double : unknow, because this value is given by the algorithm
	 */
	friend double operator - (const mfo::CFrame& _FrameOrigin, const mfo::CFrame& _FrameToCompare);

	/**
	 * Data of this frame.
	 * Pointer is not protected because
	 * this member is used in different case
	 * & accessor is not interesting for that.
	 */
	int8* m_pData;

	/**
	 * Text serialize
	 *********************
	 * @param std::ostream& _oss : Previous stream
	 * @param mfo::CFrame& _cFrame : Frame to serialze
	 *********************
	 * @return std::ostream& : Output stream
	 */
	friend std::ostream& operator<<(std::ostream & _oss, mfo::CFrame& _cFrame)
	{
		_oss << "|| --------------------------- >> Frame path : " << _cFrame.GetFullPath () << std::endl
			<< "|| --------------------------- >> Frame endianness : " << (_cFrame.IsLittleEndian() ? true : false) << std::endl
			<< "|| --------------------------- >> Frame position : "
			<< _cFrame.GetPosition (0) << " | "
			<< _cFrame.GetPosition (1) << " | "
			<< _cFrame.GetPosition (2) << std::endl;

		return _oss;
	}

protected:

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CFrame				( void )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CFrame				( const CFrame & )
	{
		assert(false);
	}

	/**
	 * Not used at this time.
	 * If you need that you can create function in
	 * a child object.
	 */
	CFrame &operator=	( const CFrame & )

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
	 * Space position of this frame in the patient's acqusition
	 */
	std::vector<double> m_vectorPosition;

	/**
	 * true if this frame is in littleEndian
	 */
	bool m_bLittleEndian;

	/**
	 * Acquisition which contains this frame
	 */
	mfo::CAcquisition* m_pAcquisition;

	/*
	 **************
	 * USED IN DATABASE
	 **************
	 */

	/**
	 * Frame path
	 */
	std::string m_vFullPath;

	/**
	 * Database indentifier
	 */
	//Not used
	//int32				m_i32DbID;
};

/**
 *
 */
 class CPointerFrame
{
public:

	/**
	 *
	 */
	CFrame* m_pFrame;

	/**
	 *
	 */
	friend bool operator < (const CPointerFrame& _FrameOrigin, const CPointerFrame& _FrameToCompare);

	/**
	 *
	 */
	~CPointerFrame()
	{
		//if (m_pFrame!=NULL) delete m_pFrame;
		// Do not delete because objects (frames) are only use for process (sort)
		// They are used by another object when this class is destroy
		// This class doesn't allocate and deallocate.
		m_pFrame = NULL;
	}
};

}//end namespace 'mfo'

#endif // FRAME_H
