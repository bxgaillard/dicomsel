#ifdef TRIAN_USED

// VIRTUALS basic type
#include "Virtuals/Type.h"

// STL's stream
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

// fabs is include in :
#include <cmath>

// Assertions
#include <cassert>

// Medimage Type (contains Inrimage)
#include <MedImage/ii.h>

#include "Virtuals/mfo/mfo.h"
//
#include "lib/DicomLib.h"

#include "visitor/CreateInrimage.h"

//DicomLib_API bool dicom::CSettingsDicom::s_bUseAcquisitionDate = true;
//DicomLib_API bool dicom::CSettingsDicom::s_bUseSeriesUID = true;
//DicomLib_API bool dicom::CSettingsDicom::s_bUseStudyUID = true;
//extern DicomLib_API bool dicom::CSettingsDicom::s_bUseModality;
//extern DicomLib_API bool dicom::CSettingsDicom::s_bUseFramePosition;

namespace mfo
{

double operator - (const mfo::CFrame& _FrameOrigin, const mfo::CFrame& _FrameToCompare)
{
    double dOrigin = _FrameOrigin.GetPosition(_FrameOrigin.GetAcquisition()->GetAxe());
    double dFinal = _FrameToCompare.GetPosition(_FrameToCompare.GetAcquisition()->GetAxe());
    return dOrigin - dFinal ;
}

bool operator < (const mfo::CPointerFrame& _FrameOrigin, const mfo::CPointerFrame& _FrameToCompare)
{
    return (    _FrameOrigin.m_pFrame->GetPosition(_FrameOrigin.m_pFrame->GetAcquisition()->GetAxe()) <
                _FrameToCompare.m_pFrame->GetPosition(_FrameToCompare.m_pFrame->GetAcquisition()->GetAxe())
           ) ? true : false;
}

bool operator == (const mfo::CPointerFrame& _FrameOrigin, const mfo::CPointerFrame& _FrameToCompare)
{
	return
		( _FrameOrigin.m_pFrame->GetPosition(0) == _FrameToCompare.m_pFrame->GetPosition(0) ) &&
		( _FrameOrigin.m_pFrame->GetPosition(1) == _FrameToCompare.m_pFrame->GetPosition(1) ) &&
		( _FrameOrigin.m_pFrame->GetPosition(2) == _FrameToCompare.m_pFrame->GetPosition(2) ) ;
}

}

namespace dicom
{

namespace visitor
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CCreateInrimage::CCreateInrimage(const std::string& _sOutputFilename, mfo::CAcquisition* _pAcquisition):
    CCreateImage(_sOutputFilename, _pAcquisition)
{}

//////////////////////////////////////////
CCreateInrimage::~CCreateInrimage()
{
#ifdef _DEBUG_STREAM
	// Close the log file
	m_ofsSBSLogFile << "Final space between slice : " << this->m_pAcquisition->GetVoxelSize()[2] << " millimeters";
	m_ofsSBSLogFile.flush();
	m_ofsSBSLogFile.close();
#endif
/**
	std::ofstream fileInfoPatient((m_sOutputFilename + m_pAcquisition->GetPath() + ".txt").c_str());
	fileInfoPatient				<< "Nom-Pr�om :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetFullName() << std::endl
                    			<< "Nom :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetName() << std::endl
                    			<< "Pr�om :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetFirstname() << std::endl
								<< "Id :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetIDDicom() << std::endl
								<< "Date de naissance :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetBirthdate() << std::endl
								<< "Sexe :" << this->m_pAcquisition->GetStudy()->GetPatient()->GetSex() << std::endl
								<< "Study UID :" << this->m_pAcquisition->GetStudy()->GetUID() << std::endl
								<< "Hospital :" << this->m_pAcquisition->GetStudy()->GetHospital() << std::endl
								<< "Modalit�:" << this->m_pAcquisition->GetStudy()->GetModality() << std::endl
								<< "Zone d'acquisition :" << this->m_pAcquisition->GetStudy()->GetAcquisitionZone() << std::endl
								<< "Serie UID :" << this->m_pAcquisition->GetSerieUID() << std::endl
								<< "Type d'image :" << this->m_pAcquisition->GetImageType() << std::endl
								<< "Num�o d'acquisition :" << this->m_pAcquisition->GetIndex() << std::endl
								<< "Date d'acquisition :" << this->m_pAcquisition->GetDate() << std::endl
								<< "Axe de mouvement :" << static_cast<uint32>(this->m_pAcquisition->GetAxe()) << std::endl
								<< "Hauteur des images :" << this->m_pAcquisition->GetHeight() << std::endl
								<< "Largeur des images  :" << this->m_pAcquisition->GetWidth() << std::endl
								<< "Flag :" << this->m_pAcquisition->GetUnsignedFlag() << std::endl
								<< "Bit par pixel :" << this->m_pAcquisition->GetBitsPerPixel() << std::endl
								<< "Taille voxel :" 	<< this->m_pAcquisition->GetVoxelSize()[0] << " - "
														<< this->m_pAcquisition->GetVoxelSize()[1] << " - "
														<< this->m_pAcquisition->GetVoxelSize()[2] << std::endl
								<< "Epaisseur :" << this->m_pAcquisition->GetSliceThickness() << std::endl
								<< "Nombre d'image :" << this->m_pAcquisition->GetFrameList()->size() << std::endl;

	fileInfoPatient.flush();
	fileInfoPatient.close();
*/
}

//////////////////////////////////////////////////
const uint8 CCreateInrimage::GetTypeOfData( void )
{
	assert(		this->m_pAcquisition != NULL );
	assert(		this->m_pAcquisition->GetBitsPerPixel() == 8
			||	this->m_pAcquisition->GetBitsPerPixel() == 16);

	uint8 ui8Type;
    if (this->m_pAcquisition->GetBitsPerPixel() == 8)
    {
        if (this->m_pAcquisition->GetUnsignedFlag())
        {
            ui8Type = II_TYPE_UCHAR;
        }
        else
        {
            ui8Type = II_TYPE_SCHAR;
        }
    }
    else
    {
		assert(this->m_pAcquisition->GetBitsPerPixel() == 16);

        if (this->m_pAcquisition->GetUnsignedFlag())
        {
            ui8Type = II_TYPE_USHORT;
        }
        else
        {
            ui8Type = II_TYPE_SSHORT;
        }
    }
	return ui8Type;
}

//////////////////////////////////////////////////
void CCreateInrimage::SetAxeOfMove( void )
{
	assert(		this->m_pAcquisition != NULL );

    // search move orientation
    std::list<mfo::CFrame*>::iterator iterFirstFrame = this->m_pAcquisition->GetFrameList()->begin();
    std::list<mfo::CFrame*>::iterator iterSecondFrame = iterFirstFrame;
    iterSecondFrame++;

    uint8 ui8Axe = 0;
    // for each axe (3), check if the offset between the first picture and
    // the second picture is not null. In this case, save the offset.
	// In the case where there are multiple move, algorithm get the
	// greater size.
	while (ui8Axe == 0 && iterSecondFrame != this->m_pAcquisition->GetFrameList()->end())
	{
		for(uint8 iterAxe = 0; iterAxe<3; iterAxe++)
		{
			const double dPosFirst		= (*iterFirstFrame )->GetPosition(iterAxe);
			const double dPosSecond		= (*iterSecondFrame )->GetPosition(iterAxe);
			const int32 i32PosFirst		= static_cast<int32>(100.0*dPosFirst);
			const int32 i32PosSecond	= static_cast<int32>(100.0*dPosSecond);
#ifdef _DEBUG_STREAM
			this->m_ofsSBSLogFile << "(" << i32PosFirst << "," << i32PosSecond << ")";
#endif
			if ( i32PosFirst != i32PosSecond )
			{
				if (ui8Axe != 0)
				{
					const double dPosFirstAxe		= (*iterFirstFrame )->GetPosition(iterAxe);
					const double dPosSecondAxe		= (*iterSecondFrame)->GetPosition(iterAxe);
					const int32 i32PosFirstAxe		= static_cast<int32>(100.0*dPosFirstAxe);
					const int32 i32PosSecondAxe		= static_cast<int32>(100.0*dPosSecondAxe);
					const double dPosFirstOldAxe	= (*iterFirstFrame )->GetPosition(this->m_pAcquisition->GetAxe());
					const double dPosSecondOldAxe	= (*iterSecondFrame)->GetPosition(this->m_pAcquisition->GetAxe());
					const int32 i32PosFirstOldAxe	= static_cast<int32>(100.0*dPosFirstOldAxe);
					const int32 i32PosSecondOldAxe	= static_cast<int32>(100.0*dPosSecondOldAxe);
#ifdef _DEBUG_STREAM
					this->m_ofsSBSLogFile << "(" << i32PosFirstAxe << "," << i32PosSecondAxe << ")";
					this->m_ofsSBSLogFile << "(" << i32PosFirstOldAxe << "," << i32PosSecondOldAxe << ")";
#endif

					if (abs( i32PosFirstAxe - i32PosSecondAxe ) > abs( i32PosFirstOldAxe - i32PosSecondOldAxe ))
					{
						this->m_pAcquisition->SetAxe(iterAxe);
#ifdef _DEBUG_STREAM
						this->m_ofsSBSLogFile << " BINGO : "<< static_cast<int>(iterAxe) << std::endl;
#endif
					}
					else
					{
#ifdef _DEBUG_STREAM
						this->m_ofsSBSLogFile << std::endl;
#endif
					}
				}
				else
				{
					this->m_pAcquisition->SetAxe(iterAxe);
#ifdef _DEBUG_STREAM
					this->m_ofsSBSLogFile << " BINGO : "<< static_cast<int>(iterAxe) << std::endl;
#endif
				}
				ui8Axe++;
			}
			else
			{
#ifdef _DEBUG_STREAM
				this->m_ofsSBSLogFile << std::endl;
#endif
			}
		}
		iterSecondFrame++;
    }

    // One move : In another case, the previous compute is not valid !
	// Make a voxel which is cubic.
	// Inrimage format doesn't understand voxel which is not cubic.
	// If multiaxe move detected, probably acquisition is an IRM.
    if (1 != ui8Axe)
    {
#ifdef _DEBUG_STREAM
		this->m_pAcquisition->SetPath( "MULTui8Axe__" + this->m_pAcquisition->GetPath() );
#endif
        this->m_pAcquisition->SetSpaceBetweenSlice(1.0);
    }

	assert(1 <= ui8Axe);
	//assert(0 <= this->m_pAcquisition->GetAxe() );
	assert(3 > this->m_pAcquisition->GetAxe() );
}

#ifdef _DEBUG_STREAM
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::DisplaySpaceBetweenSliceInOuputStream(	const mfo::CFrame * const _pFrameFirst,
																const mfo::CFrame * const _pFrameSecond,
																const double _dSpaceBetweenSlice )
{
	this->m_ofsSBSLogFile << std::endl << " [ "
	 << _pFrameFirst->GetPosition(0)
	 << " - "
	 << _pFrameFirst->GetPosition(1)
	 << " - "
	 << _pFrameFirst->GetPosition(2)
	 << " ]"
	 << _pFrameFirst->GetPosition( _pFrameFirst->GetAcquisition()->GetAxe() )
	 << " - "
	 << " [ "
	 << _pFrameSecond->GetPosition(0)
	 << " - "
	 << _pFrameSecond->GetPosition(1)
	 << " - "
	 << _pFrameSecond->GetPosition(2)
	 << " ]"
	 << _pFrameSecond->GetPosition( _pFrameSecond->GetAcquisition()->GetAxe() )
	 << " : " << _dSpaceBetweenSlice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::DisplayFinalSpaceBetweenSliceInOuputStream(
								std::map<double, uint32, std::greater<uint32> >& _mapDifferentSpace )
{
	this->m_ofsSBSLogFile << std::endl << std::endl;
	std::map<double, uint32, std::greater<uint32> >::iterator iterMapWrite = _mapDifferentSpace.begin();
	while(iterMapWrite!=_mapDifferentSpace.end())
	{
		this->m_ofsSBSLogFile	<< "Space : " << (iterMapWrite->first) << " ---- Nb SBS : "
								<< iterMapWrite->second << std::endl;
		++iterMapWrite;
	}
#ifdef _DEBUG_STREAM
	if (_mapDifferentSpace.size()!=1)
	{
		this->m_pAcquisition->SetPath( "NOGOOD_SBS__" + this->m_pAcquisition->GetPath() );
	}
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::InitializeListPointerFrame ( std::list<mfo::CPointerFrame> & _listPointerFrame )
{
	assert(	this->m_pAcquisition != NULL );
	assert(	_listPointerFrame.size() ==  this->m_pAcquisition->GetFrameList()->size());

	std::list<mfo::CPointerFrame>::iterator iterPointer =
									_listPointerFrame.begin();
	std::list<mfo::CFrame*>::iterator iterFrame2 =
									this->m_pAcquisition->GetFrameList()->begin();

    while(iterFrame2 != this->m_pAcquisition->GetFrameList()->end())
    {
        (iterPointer++)->m_pFrame = *(iterFrame2++); ////////////// FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!
        //(++iterPointer)->m_pFrame = *(++iterFrame2);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const double CCreateInrimage::ComputeSpaceBetweenSlice( std::list<mfo::CPointerFrame> & _listPointerFrame,
											std::map<double, uint32, std::greater<uint32> >& _mapDifferentSpace )
{
	double dSpaceBetweenSlice = 0.0;
	std::list<mfo::CPointerFrame>::iterator iterFrame			= _listPointerFrame.begin();
	std::list<mfo::CPointerFrame>::iterator iterFramePrevious	= _listPointerFrame.begin();
	iterFrame++;

	// Compute the space between slice. // FIXME !!!!
	for (	;
			iterFrame != _listPointerFrame.end();
			iterFrame++, iterFramePrevious++
		)
	{
		bool bfindEqualSpace = false;
		// This compute make the roundness of the space between slice because dicom file doesn't
		// store the data which the same value.
		dSpaceBetweenSlice = ((int)(0.5+10*fabs((*iterFramePrevious->m_pFrame)-(*iterFrame->m_pFrame))))/10.0;

#ifdef _DEBUG_STREAM
		DisplaySpaceBetweenSliceInOuputStream(iterFramePrevious->m_pFrame, iterFrame->m_pFrame, dSpaceBetweenSlice);
#endif

		// Search this spaceBS...
		std::map<double, uint32, std::greater<uint32> >::iterator iterMap = _mapDifferentSpace.begin();
		while(iterMap!=_mapDifferentSpace.end() && !bfindEqualSpace)
		{
			if ((int)(iterMap->first) == (int)(10.0*dSpaceBetweenSlice))
			{
				bfindEqualSpace = true;
			}
			else
			{
				++iterMap;
			}
		}

		// Add the SpaceBS or increase the number of the SpaceBS (if this value is find)
		if (bfindEqualSpace)
		{
			++iterMap->second;
#ifdef _DEBUG_STREAM
			m_ofsSBSLogFile << "(find) - " << iterMap->first << " - " << iterMap->second;
#endif
		}
		else
		{
			std::pair<std::map<double, uint32, std::greater<uint32> >::iterator, bool> aPair;
			aPair = _mapDifferentSpace.insert(std::map<double, uint32,
									std::greater<uint32> >::value_type(double(10.0*dSpaceBetweenSlice),(uint32)1));
#ifdef _DEBUG_STREAM
			std::map<double, uint32, std::greater<uint32> >::iterator iterMapFind =
												_mapDifferentSpace.find(10*dSpaceBetweenSlice);
			m_ofsSBSLogFile << "(not find) - " << iterMapFind->first << " - "
							<< iterMapFind->second << " Pair(" << aPair.first->first
							<< "," << aPair.first->second << ")";
#endif
		}
	}

#ifdef _DEBUG_STREAM
	this->DisplayFinalSpaceBetweenSliceInOuputStream(_mapDifferentSpace);
#endif

	return dSpaceBetweenSlice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::MakeImageListWithMultipleFrame( std::list<mfo::CPointerFrame> & _listPointerFrame )
{
	// Initialize
	this->InitializeListPointerFrame(_listPointerFrame);

	if (dicom::CSettingsDicom::s_bUseFramePosition)
	{
		// STL Sort... (Used the operator < of PointerFrame class) & delete doublon
		this->SetAxeOfMove(); // Use to the next algorithm.
		_listPointerFrame.sort();
		_listPointerFrame.unique();

		// Initialize data for space between slice.
		std::map<double, uint32, std::greater<uint32> > mapDifferentSpace;
		double dSpaceBetweenSlice = this->ComputeSpaceBetweenSlice(_listPointerFrame, mapDifferentSpace);

		// Fix the Space between slice on the most usual value.
		uint32 ui32NbDifferentSpace = 0;
		for(
			std::map<double, uint32, std::greater<uint32> >::iterator iterMapSort = mapDifferentSpace.begin();
			iterMapSort!=mapDifferentSpace.end();
			iterMapSort++
			)
		{
			if (iterMapSort->second > ui32NbDifferentSpace)
			{
				dSpaceBetweenSlice = iterMapSort->first;
				ui32NbDifferentSpace = iterMapSort->second;
			}
		}

		// Computer pixel spacing
		this->m_pAcquisition->SetSpaceBetweenSlice(dSpaceBetweenSlice/10);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::Convert( const bool _bCrypt )
{
    assert(this->m_pAcquisition->GetWidth()!=0);
    assert(this->m_pAcquisition->GetHeight()!=0);
    assert(this->m_pAcquisition->IsGoodBitsPerPixel());
    assert(this->m_pAcquisition->GetFrameList()->size() > 0);

#ifdef _DEBUG_STREAM
	this->m_ofsSBSLogFile.open((m_sOutputFilename+"SBSLog.txt").c_str());// Log file
#endif

    // Create an new picture list (initialize with the old number of picture)
    std::list<mfo::CPointerFrame> listPointerFrame(this->m_pAcquisition->GetFrameList()->size());

    this->m_pAcquisition->SetSpaceBetweenSlice(1.0);

    // search the pixel representation
    int8 type = this->GetTypeOfData();

    if (this->m_pAcquisition->GetFrameList()->size()>1)
    {
		this->MakeImageListWithMultipleFrame(listPointerFrame);
    }
    else
    {
        // Copy the pointer in the encapsulate class
        std::list<mfo::CPointerFrame>::iterator iterPointer = listPointerFrame.begin();
        std::list<mfo::CFrame*>::iterator iterFrame2 = this->m_pAcquisition->GetFrameList()->begin();
        iterPointer->m_pFrame = *iterFrame2;
#ifdef _DEBUG_STREAM
		this->m_pAcquisition->SetPath( "NOGOOD_ONEFILE__" + this->m_pAcquisition->GetPath() );
#endif
    }

    // Check paramater
    if (
			(
					dicom::CSettingsDicom::s_bUseFramePosition
				&&	(
						this->m_pAcquisition->GetVoxelSize()[0]			== 0.0
					||  this->m_pAcquisition->GetVoxelSize()[1]			== 0.0
					)
			)
		// This test is necessary but at this time is not functionnal.
		// Because, compute of the spaceBS is not optimal. --- FIXME--- FIXME--- FIXME--- FIXME--- FIXME--- FIXME
        ||  this->m_pAcquisition->GetVoxelSize()[2]			== 0.0
        ||  this->m_pAcquisition->GetWidth()				== 0
        ||  this->m_pAcquisition->GetHeight()				== 0
        ||  listPointerFrame.size()							== 0
        ||  !this->m_pAcquisition->IsGoodBitsPerPixel()
       )
    {
        char param[1024];
        ::sprintf(param, "Pixel size : (%f,%f)\nSpace between slice: %f\nFrame size : (%d,%d)\nNb Frame : %d\nBytesPerPixel :%d\n",
                                this->m_pAcquisition->GetVoxelSize()[0],
                                this->m_pAcquisition->GetVoxelSize()[1],
                                this->m_pAcquisition->GetVoxelSize()[2],
                                this->m_pAcquisition->GetWidth(),
                                this->m_pAcquisition->GetHeight(),
                                listPointerFrame.size(),
                                this->m_pAcquisition->GetBitsPerPixel()
                                );
		throw CInrimageException (m_sOutputFilename, std::string("Param�re non conforme pour cr�r l'inrimage\n")+param);
    }

    // Create an inrimage
    iiImage::Tdata data; // FIXME
                /////////////////////////////////////
                // That run, but if data is not    //
                // created before the inrimage     //
                // union is not visible (C++ bugs?)//
                /////////////////////////////////////
    iiImage inr;
    if (inr.iiCreateImage(
        data,
        this->m_pAcquisition->GetWidth(),
        this->m_pAcquisition->GetHeight(),
        listPointerFrame.size(),
        1,
        static_cast<uint8>(this->m_pAcquisition->GetBitsPerPixel()),
        type ,
		_bCrypt ,
        static_cast<float>(this->m_pAcquisition->GetVoxelSize()[0]),
        static_cast<float>(this->m_pAcquisition->GetVoxelSize()[1]),
        static_cast<float>(this->m_pAcquisition->GetVoxelSize()[2])
    ) != II_OK )
	{
		throw CInrimageException (m_sOutputFilename, "Inrimage allocation error\n");
	}

#ifdef _DEBUG
	void * p_iStartPicture3D = &data.ptr[0][0][0];
	void * p_iEndPicture3D = &data.ptr[0][0][0] + listPointerFrame.size()*this->m_pAcquisition->GetWidth()*this->m_pAcquisition->GetHeight()*this->m_pAcquisition->GetBitsPerPixel();
#endif

    // Copy data
    uint32 uiSizeFrame = this->m_pAcquisition->GetWidth()*this->m_pAcquisition->GetHeight()*this->m_pAcquisition->GetBytesPerPixel();
    std::list<mfo::CPointerFrame>::reverse_iterator iterFrame = listPointerFrame.rbegin();
    if (!iterFrame->m_pFrame->IsLittleEndian () && this->m_pAcquisition->GetBitsPerPixel()==16)
    {
        for (
               uint32 ui32Index=0;
               iterFrame != listPointerFrame.rend();
               ui32Index++, iterFrame++
            )
        {
			assert( ui32Index < listPointerFrame.size() ) ;

            char* p_iDataChar	= &data.ptr[ui32Index][0][0];				// get the beginning of the slice
			int8* p_iDataOrigin	= &iterFrame->m_pFrame->m_pData[0];			// get the beginning of the dicom file
            char* p_iEndOfFile	= &data.ptr[ui32Index][0][0]+uiSizeFrame-1;	// get the theorical end of the slice
																			// -1 because, when we write inrimage file
																			// we write 2 octets in the same time.

            while(p_iDataChar < p_iEndOfFile)
            {
#ifdef _DEBUG
				assert( p_iDataChar		>=	p_iStartPicture3D ) ;
				assert( p_iDataChar		<	p_iEndPicture3D ) ;
 				assert( p_iDataChar		>=	&data.ptr[ui32Index][0][0] ) ;
 				assert( p_iDataChar		<	&data.ptr[ui32Index][0][0] + uiSizeFrame ) ;
				assert( p_iDataOrigin	>=	&iterFrame->m_pFrame->m_pData[0] ) ;
				assert( p_iDataOrigin	<	&iterFrame->m_pFrame->m_pData[0] + uiSizeFrame ) ;
#endif
                *p_iDataChar = *(p_iDataOrigin+1);	// dicom : 0A 0A 0A -----> inrimage : A0 A0 A0
				p_iDataChar++;
                *p_iDataChar = *p_iDataOrigin;
				p_iDataChar++;
				p_iDataOrigin+=2; // We have read 2 char, we move offset of 2 char
            }

            delete []iterFrame->m_pFrame->m_pData;
            iterFrame->m_pFrame->m_pData = NULL;

        }
    }
    else
    {
        for (
               uint32 ui32Index=0;
               iterFrame != listPointerFrame.rend();
               ui32Index++, iterFrame++
            )
        {
			assert( ui32Index < listPointerFrame.size() ) ;

            char* p_iDataChar	= &data.ptr[ui32Index][0][0];				// get the beginning of the slice
			int8* p_iDataOrigin	= &iterFrame->m_pFrame->m_pData[0];			// get the beginning of the dicom file
            char* p_iEndOfFile	= &data.ptr[ui32Index][0][0]+uiSizeFrame;	// get the theorical end of the slice

            while(p_iDataChar < p_iEndOfFile)
            {
#ifdef _DEBUG
				assert( p_iDataChar		>=	p_iStartPicture3D ) ;
				assert( p_iDataChar		<	p_iEndPicture3D ) ;
 				assert( p_iDataChar		>=	&data.ptr[ui32Index][0][0] ) ;
 				assert( p_iDataChar		<	&data.ptr[ui32Index][0][0] + uiSizeFrame ) ;
				assert( p_iDataOrigin	>=	&iterFrame->m_pFrame->m_pData[0] ) ;
				assert( p_iDataOrigin	<	&iterFrame->m_pFrame->m_pData[0] + uiSizeFrame ) ;
#endif
				*p_iDataChar = *p_iDataOrigin; // copy byte by byte =>  dicom : 0A 0A 0A -----> inrimage : 0A 0A 0A
				p_iDataChar++;
				p_iDataOrigin++;
            }

            delete []iterFrame->m_pFrame->m_pData;
            iterFrame->m_pFrame->m_pData = NULL;
        }
    }

	this->DeleteDuplicateFrame(listPointerFrame) ;

////////// A VIRER //////////////// FIXME ///////////// DELETEME
//std::stringstream stringstreamVoxel;
//stringstreamVoxel
//					<< "n"	<< m_pAcquisition->GetFrameList()->size()
//					<< "[x" << m_pAcquisition->GetFrameList()->front()->GetPosition(0) << "]"
//					<< "[y" << m_pAcquisition->GetFrameList()->front()->GetPosition(1) << "]"
//					<< "[z" << m_pAcquisition->GetFrameList()->front()->GetPosition(2) << "]"
//					<< "[x" << m_pAcquisition->GetFrameList()->back()->GetPosition(0)  << "]"
//					<< "[y" << m_pAcquisition->GetFrameList()->back()->GetPosition(1)  << "]"
//					<< "[z" << m_pAcquisition->GetFrameList()->back()->GetPosition(2)  << "]"
//					<< "-Z" << m_pAcquisition->GetVoxelSize()[2]  << "";
//
//m_pAcquisition->SetPath(m_pAcquisition->GetTime());
////////////////////////////////////////////////

    // Write to the hard disk in the 'pFilename' file
	if (inr.iiWriteImage(
							(
								(
									m_sOutputFilename
								+	m_pAcquisition->GetPath()
								+	".inr")
								+	((_bCrypt)?".gz":"")
								).c_str()
							)
							!= II_OK
						)
	{
		throw CInrimageException (m_sOutputFilename, "Picture 3D write error");
	}

	m_pAcquisition->SetPath(((m_pAcquisition->GetPath() + ".inr") + ((_bCrypt)?".gz":"")).c_str());

    listPointerFrame.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCreateInrimage::DeleteDuplicateFrame( std::list<mfo::CPointerFrame> & _listPointerFrame )
{
	mfo::CPointerFrame aPointerFrame;

	for (
			std::list<mfo::CFrame*>::iterator iterFrameFirstList = this->m_pAcquisition->GetFrameList()->begin();
			iterFrameFirstList != this->m_pAcquisition->GetFrameList()->end();
		)
	{
		bool bFind = false;
		std::list<mfo::CFrame*>::iterator iterFrameSecFirstList = iterFrameFirstList;
		iterFrameSecFirstList++;

		for (
				std::list<mfo::CPointerFrame>::iterator iterFrameSecondList = _listPointerFrame.begin();
				iterFrameSecondList != _listPointerFrame.end() && !bFind;
				iterFrameSecondList++
			)
		{
			aPointerFrame.m_pFrame = *iterFrameFirstList ;

			if ( (*iterFrameSecondList) == aPointerFrame )
			{
				bFind = true;
			}
		}

		if (bFind)
		{
			iterFrameFirstList++;
		}
		else
		{
			mfo::CFrame * pFirstListFrame = *iterFrameFirstList;
			this->m_pAcquisition->GetFrameList()->remove(*iterFrameFirstList);
			assert(pFirstListFrame != NULL);
			delete (pFirstListFrame);
			pFirstListFrame = NULL;
			assert(pFirstListFrame == NULL);
			iterFrameFirstList = iterFrameSecFirstList ;
		}
	}
}


}// end namespace visitor

}// end namespace










/*
    // Copy data
    uint32 uiSizeFrame = this->m_pAcquisition->GetWidth()*this->m_pAcquisition->GetHeight()*this->m_pAcquisition->GetBytesPerPixel();
    std::list<mfo::CPointerFrame>::reverse_iterator iterFrame = listPointerFrame.rbegin();
    if (!iterFrame->m_pFrame->IsLittleEndian () && this->m_pAcquisition->GetBitsPerPixel()==16)
    {
        for (
               int i=0;
               iterFrame != listPointerFrame.rend();
               i++, iterFrame++
            )
        {
            int8* dataChar = &data.ptr[i][0][0];										// set on the beginning of this file
            int8* uiEndOfFile = &data.ptr[i][0][0]+uiSizeFrame;
            while(dataChar < uiEndOfFile)
            {
                *dataChar = *(iterFrame->m_pFrame->m_pData+1);	// It's the next data
				dataChar++;
                *(dataChar++) = *(iterFrame->m_pFrame->m_pData);
				dataChar++;
				iterFrame->m_pFrame->m_pData+=2; // We have read 2 char, we move offset of 2 char
            }
            iterFrame->m_pFrame->m_pData-=uiSizeFrame; // Go to the beginning !
            delete []iterFrame->m_pFrame->m_pData;
            iterFrame->m_pFrame->m_pData = NULL; ///////////// FIXME

        }
    }
    else
    {
        for (
               int i=0;
               iterFrame != listPointerFrame.rend();
               i++, iterFrame++
            )
        {
            int8* dataChar = &data.ptr[i][0][0]; // set on the beginning of this file
            int8* uiEndOfFile = &data.ptr[i][0][0]+uiSizeFrame;
            while(dataChar < uiEndOfFile)
            {
				assert()
                *dataChar = *(iterFrame->m_pFrame->m_pData); // copy byte by byte
				dataChar++;
				iterFrame->m_pFrame->m_pData++;
            }
            iterFrame->m_pFrame->m_pData-=uiSizeFrame; // Go to the beginning !
            delete []iterFrame->m_pFrame->m_pData;
            iterFrame->m_pFrame->m_pData = NULL;///////////// FIXME
        }
    }

*/








/**
	class CIsPresent
	{
	public:
		std::list<mfo::CPointerFrame> * m_pList;
		CIsPresent(std::list<mfo::CPointerFrame> * _pList) : m_pList(_pList){}
		bool operator() (mfo::CFrame)
		{
			bool bFind = false;
			const mfo::CFrame * const pFirstListFrame = (*iterFrameSecondList).m_pFrame;
			const mfo::CFrame * const pSecondListFrame = *iterFrameFirstList;
			if ( pFirstListFrame == pSecondListFrame )
			{
				bFind = true;
			}
			return bFind;
		}

	};

	this->m_pAcquisition->GetFrameList()->erase(
												std::remove_if	(
																	this->m_pAcquisition->GetFrameList()->begin(),
																	this->m_pAcquisition->GetFrameList()->end(),
																	CIsPresent(&_listPointerFrame)
																),
												this->m_pAcquisition->GetFrameList()->end()
											  );*/

#endif // TRIAN_USED
