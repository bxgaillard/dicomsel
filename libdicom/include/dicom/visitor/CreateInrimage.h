// VisitorInrimage.h
#ifndef VISITORINRIMAGE_DICOMFILE_H
#define VISITORINRIMAGE_DICOMFILE_H
#ifdef TRIAN_USED

#include "Virtuals/Type.h"

#include <map>

#include "CreateImage.h"

namespace dicom
{
namespace visitor
{

/** 
 * CreateInrimage
 *********************
 * Namespace : dicom::visitor
 *********************
 * This class is specialised for convert an
 * dicom acquisition to an inrimage file.
 * It's implements convert function of
 * CreateImage class.
 */

class CCreateInrimage : public dicom::visitor::CCreateImage
{
public:

	/**
	 * Constructor with two parameters.
	 *********************
	 * @param _sOutputFilename [const std::string&] : Filename of images block created.
	 * @param _pAcquisition [CAcquisition*] : Data origin.
	 */
    explicit				CCreateInrimage							(	const std::string & _sOutputFilename, 
																		mfo::CAcquisition* _pAcquisition ) ;

	/**
	 * Destructor.
	 * In debug mode, close the debug output stream.
	 ****************************************
	 */
							~CCreateInrimage						() ;

	/**
	 * This function converts all picture in an acquisition to an inrimage.
	 *********************
	 * @param _bCrypt [const bool] : use gzip to create the inrimage file
	 *********************
	 * @pre Width, height, bytes per pixel and number of frame must be not null
	 *********************
	 * @exception std::exception
	 */
    virtual void			Convert									( const bool _bCrypt ) ;

private:

	/**
	 * Get the type of data (known in medimage) with unsignedFlag and
	 * bitsPerPixel member of mfo::CAcquisition
	 ****************************************
	 * @pre acquisition allocated
	 * @pre 8 or 16 bits per pixel
	 ****************************************
	 * @return const int8 one value in II_TYPE_UCHAR, II_TYPE_SCHAR, II_TYPE_USHORT or II_TYPE_SSHORT
	 */
	const uint8				GetTypeOfData							( void ) ;

	/**
	 * Search and set the moving axe which
	 * has been used during the patient's exam.
	 ****************************************
	 * @pre acquisition allocated
	 ****************************************
	 * @post a moving axe exists
	 * @post the acquisition moving axe set
	 */
	void					SetAxeOfMove							( void ) ;

	/**
	 * Create a good slice list. With unique
	 * slice which sort and with a good space between themself.
	 ****************************************
	 * @param _listPointerFrame [std::list<mfo::CPointerFrame> &] : list of frame pointer
	 ****************************************
	 */
 	void					MakeImageListWithMultipleFrame			( std::list<mfo::CPointerFrame> & ) ;
	
	/**
	 * We cannot used pointer with STL sort function.
	 * We must used encapsulate the pointer in an another class....
	 * This function initialize list of this second class.
	 ****************************************
	 * @param _listPointerFrame [std::list<mfo::CPointerFrame> &] : list of frame pointer
	 ****************************************
	 * @pre acquisition allocated
	 * @pre the parmater list has the same size with the acquisition frame list
	 */
	void					InitializeListPointerFrame				( std::list<mfo::CPointerFrame> & _listPointerFrame ) ;
	
	const double			ComputeSpaceBetweenSlice				( std::list<mfo::CPointerFrame> & _listPointerFrame, std::map<double, uint32, std::greater<uint32> >& ) ;
	
	/**
	 * Delete duplicate frame in the list.
	 * (use when the user select root of dicom cdrom
	 * with a www folder and a dicom folder. Twice
	 * folder use the same data file).
	 ****************************************
	 * @param _listPointerFrame [std::list<mfo::CPointerFrame> &] : list of frame pointer
	 ****************************************
	 */
	void					DeleteDuplicateFrame					( std::list<mfo::CPointerFrame> & _listPointerFrame ) ;

	/** @name Copy 
	 * Copy operator and constructor.
	 * No copy enabled !
	 */
	//@{

	/**
	 * Not used.
	 ****************************************
	 */
   	explicit				CCreateInrimage							( const CCreateInrimage & ) ;

	/**
	 * Not used.
	 ****************************************
	 */
    CCreateInrimage &		operator =								( const CCreateInrimage & ) ;

	//@}

#ifdef _DEBUG_STREAM
	/**
	 * stream debug
	 */
	std::ofstream			m_ofsSBSLogFile;

	/**
	 * DEBUG Function
	 */
	void					DisplaySpaceBetweenSliceInOuputStream	( const mfo::CFrame * const, const mfo::CFrame * const, const double ) ;

	/**
	 * DEBUG Function
	 */
	void					DisplayFinalSpaceBetweenSliceInOuputStream ( std::map<double, uint32, std::greater<uint32> >& ) ;
#endif

public :

    /** \class CFileException
     *********************
     * Namespace : dicom::visitor::CCreateInrimage
     *********************
     * Description : Basic exception in used of inrimage.
     *********************
     * @see CCreateInrimage
     * @see MedImage
     *********************
     */
    class CInrimageException : public dicom::exception::CBase
    {
		public:
			/**
			 * Constructor with two parameter
			 *********************
			 * @param _stringFile [const string&] : Full name of the file
			 * @param _stringError [const string&] : Error string
			 */
			CInrimageException(const std::string _stringFile, const std::string _stringError):
				dicom::exception::CBase(_stringFile + " : " + _stringError)
			{}
    };

};

}// end namespace visitor
}// end namespace

#endif // TRIAN_USED

#endif // VISITORINRIMAGE_DICOMFILE_H
