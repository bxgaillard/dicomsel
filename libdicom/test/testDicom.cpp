#include <ostream>
#include <fstream>
#include <string>
#include <exception>
#include <stdlib.h>

#include "../include/lib/DicomLib.h"
#include "../include/ExceptionDicom.h"
#include "Virtuals/mfo/Patient.h"

int main(int argc, char * argv[])
{
    dicom::CFacadeDicom dicom;
    std::string strPathDicom;
    std::string strPathInr;

    switch (argc)
    {
	    case 8 :
/*
		dicom::CSettingsDicom::setUseAcquisitionDate	(static_cast<bool>(atoi(argv[7])));
		dicom::CSettingsDicom::setUseSeriesUID		(static_cast<bool>(atoi(argv[6])));
		dicom::CSettingsDicom::setUseStudyUID		(static_cast<bool>(atoi(argv[5])));
		dicom::CSettingsDicom::setUseModality		(static_cast<bool>(atoi(argv[4])));
		dicom::CSettingsDicom::setUseFramePosition	(static_cast<bool>(atoi(argv[3])));
*/
	    case 3 :
		strPathInr = argv[2];
		strPathDicom = argv[1];
		break;
	    default :


		std::cerr << "(C) Copyright 2004 IRCAD" << std::endl
			<< "Prohibition of Medical Use" << std::endl
			<< "usage : " << argv[0] << " "
			<< "pathDicom " 
			<< "pathInr " 
			<< "[positionImageUtilisee modaliteUtilisee" 
			<< "StudyUID SeriesUID dateAcquisitionUtilisee]"
			<< std::endl
			<< "default : "
			<< "[positionImageUtilisee==1 modaliteUtilisee==1" 
			<< "StudyUID==1 SeriesUID==1 dateAcquisitionUtilisee==1]"
			<< std::endl;
		return -1;
    }
    
    
    try
    {
	std::ofstream ofsList;	    
        dicom.LoadDirectory(strPathDicom);
	ofsList.open("D:\\listes.txt");
        dicom.ListAllASCII(ofsList);
        ofsList.flush();
        ofsList.close();	
    }
    catch( const dicom::exception::CBase & e )
    {
        std::cerr << "DICOM : Impossible de charger le repertoire" 
		 << std::endl << e.what() << std::endl ;
	return -1;
    }
    catch( ... )
    {
        std::cerr << "Inconnue : Impossible de charger le repertoire" << std::endl;
	return -1;
    }
/*
    try
    {
	dicom.CreateAllInrimage(strPathInr);
    }
    catch(const std::exception & e)
    {
        std::cerr << "Inrimage : Impossible de creer l'inrimage"
		 << std::endl << e.what() << std::endl ;
	return -1;
    }
    catch( ... )
    {
        std::cerr << "Inconnue : Impossible de creer l'inrimage" << std::endl;
	return -1;
    }
*/  
    return 0;
}
