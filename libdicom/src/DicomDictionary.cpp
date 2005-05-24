/*
 * Filename: DicomDictionary.cpp
 *********************
 * Namespace : dicom
 *********************
 * Description: . 
 **/

#include "DicomDictionary.h"

namespace dicom
{

const std::string dicom::CDicomDictionary::GetNameElement(const int32& group, const int32&element)
{
    std::string info("unrecognized");

    switch (group)  {

        case 0x0002 :
            switch (element)  {
                case 0x00 :  info = "file meta elements group len"; break;
                case 0x01 :  info = "file meta info version";       break;
                case 0x02 :  info = "media storage SOP class uid";  break;
                case 0x03 :  info = "media storage SOP inst uid";   break;
                case 0x10 :  info = "transfer syntax uid";          break;
                case 0x12 :  info = "implementation class uid";     break;
                case 0x13 :  info = "implementation version name";  break;
                case 0x16 :  info = "source app entity title";      break;
                case 0x100:  info = "private info creator uid";     break;
                case 0x102:  info = "private info";                 break;
            }
            break;

        case 0x0008 :
            switch (element)  {
                case 0x00 :  info = "identifying group";             break;
                case 0x01 :  info = "length to end";                 break;
                case 0x05 :  info = "specific character set";        break;
                case 0x08 :  info = "image type";                    break;
                case 0x10 :  info = "recognition code";              break;
                case 0x16 :  info = "SOP Class UID";                 break;
                case 0x18 :  info = "SOP Instance UID";              break;
                case 0x20 :  info = "study date";                    break;
                case 0x21 :  info = "series date";                   break;
                case 0x22 :  info = "acquisition date";              break;
                case 0x23 :  info = "image date";                    break;
                case 0x30 :  info = "study time";                    break;
                case 0x31 :  info = "series time";                   break;
                case 0x32 :  info = "acquisition time";              break;
                case 0x33 :  info = "image time";                    break;
                case 0x40 :  info = "data set type";                 break;
                case 0x41 :  info = "data set subtype";              break;
                case 0x50 :  info = "accession number";              break;
                case 0x60 :  info = "modality";                      break;
                case 0x70 :  info = "manufacturer";                  break;
                case 0x80 :  info = "institution name";              break;
                case 0x90 :  info = "referring physician's name";    break;
                case 0x1010: info = "station name";                  break;
                case 0x103e: info = "series description";            break;
                case 0x1030: info = "study description";             break;
                case 0x1040: info = "institutional dept. name";      break;
                case 0x1050: info = "performing physician's name";   break;
                case 0x1060: info = "name phys(s) read stdy";        break;
                case 0x1070: info = "operator's name";               break;
                case 0x1080: info = "admitting diagnoses description"; break;
                case 0x1090: info = "manufacturer's model name";     break;
                case 0x1140: info = "referenced image sequence";     break;
            }
            break;

        case 0x0010 :
            switch (element)  {
                case 0x00 :  info = "patient group";                 break;
                case 0x10 :  info = "patient name";                  break;
                case 0x20 :  info = "patient ID";                    break;
                case 0x30 :  info = "patient birthdate";             break;
                case 0x40 :  info = "patient sex";                   break;
                case 0x1010: info = "patient age";                   break;
                case 0x1030: info = "patient weight";                break;
                case 0x21b0: info = "additional patient history";    break;
            }
            break;

        case 0x0018 :
            switch (element)  {
                case 0x00 :  info = "acquisition group";             break;
                case 0x10 :  info = "contrast/bolus agent";          break;
                case 0x15 :  info = "body part examined";            break;
                case 0x20 :  info = "scanning sequence";             break;
                case 0x21 :  info = "Sequence Variant";              break;
                case 0x22 :  info = "Scan Options";                  break;
                case 0x23 :  info = "MR Acquisition Type";           break;
                case 0x24 :  info = "Sequence Name";                 break;
                case 0x25 :  info = "Angio Flag";                    break;
                case 0x30 :  info = "radionuclide";                  break;
                case 0x50 :  info = "slice thickness";               break;
                case 0x60:   info = "kvp";                           break;
                case 0x80 :  info = "repetition time";               break;
                case 0x81 :  info = "echo time";                     break;
                case 0x82 :  info = "inversion time";                break;
                case 0x83 :  info = "Number of Averages";            break;
                case 0x84 :  info = "Imaging Frequency";             break;
                case 0x85 :  info = "Imaged Nucleus";                break;
                case 0x86 :  info = "Echo Number";                   break;
                case 0x87 :  info = "Magnetic Field Strength";       break;
                case 0x88 :  info = "Spacing Between Slices";        break;
                case 0x91 :  info = "Echo Train Length";             break;
                case 0x93 :  info = "Percent Sampling";              break;
                case 0x94 :  info = "Percent Phase Field of View";   break;
                case 0x95 :  info = "Pixel Bandwidth";               break;
                case 0x1020: info = "software version(s)";           break;
                case 0x1030: info = "protocol name";                 break;
                case 0x1040: info = "Contrast/Bolus Route";          break;
                case 0x1062: info = "Nominal Interval";              break;
                case 0x1088: info = "Heart Rate";                    break;
                case 0x1090: info = "Cardiac Number of Images";      break;
                case 0x1094: info = "Trigger Window";                break;
                case 0x1100: info = "Reconstruction Diameter";       break;
                case 0x1120: info = "gantry/detector tilt";          break;
                case 0x1150: info = "exposure time";                 break;
                case 0x1151: info = "x-ray tube current";            break;
                case 0x1210: info = "convolution kernel";            break;
                case 0x1250: info = "Receiving Coil";                break;
                case 0x1251: info = "Transmitting Coil";             break;
                case 0x1310: info = "Acquisition Matrix";            break;
                case 0x1314: info = "Flip Angle";                    break;
                case 0x1315: info = "Variable Flip Angle Flag";
                case 0x1316: info = "SAR";                           break;
                case 0x1400: info = "Acquisition Device Processing Description";  break;
                case 0x1401: info = "Acquisition Device Processing Code";  break;
                case 0x5100: info = "Patient Position";              break;
                case 0x5101: info = "View Position";                 break;
                case 0x6000: info = "Sensitivity";                   break;
            }
            break;

        case 0x0020 :
            switch (element)  {
                case 0x00 :  info = "relationship group";            break;
                case 0x0d :  info = "Study Instance UID";            break;
                case 0x0e :  info = "Series Instance UID";           break;
                case 0x10 :  info = "study id";                      break;
                case 0x11 :  info = "series number";                 break;
                case 0x12 :  info = "acquisition number";            break;
                case 0x13 :  info = "image number";                  break;
                case 0x20 :  info = "patient orientation";           break;
                case 0x30 :  info = "image position (ret)";          break;
                case 0x32 :  info = "Image Position (Patient)";      break;
                case 0x35 :  info = "image orientation (ret)";       break;
                case 0x37 :  info = "Image Orientation (Patient)";   break;
                case 0x50 :  info = "location (ret)";                break;
                case 0x52 :  info = "Frame of Reference UID";        break;
                case 0x60 :  info = "Laterality";                    break;
                case 0x1002: info = "images in acquisition";         break;
                case 0x1040: info = "position reference";            break;
                case 0x1041: info = "slice location";                break;
                case 0x3401: info = "modifying device id";           break;
                case 0x3402: info = "modified image id";             break;
                case 0x3403: info = "modified image date";           break;
                case 0x3404: info = "modifying device mfg.";         break;
                case 0x3405: info = "modified image time";           break;
                case 0x3406: info = "modified image desc.";          break;
                case 0x4000: info = "image comments";                break;
                case 0x5000: info = "original image id";             break;
            }
            break;

        case 0x0028 :
            switch (element)  {
                case 0x00 :  info = "image presentation group";      break;
                case 0x02 :  info = "samples per pixel";             break;
                case 0x04 :  info = "photometric interpretation";    break;
                case 0x05 :  info = "image dimensions (ret)";        break;
                case 0x06 :  info = "planar configuration";          break;
                case 0x08 :  info = "number of frames";              break;
                case 0x09 :  info = "frame increment pointer";       break;
                case 0x10 :  info = "rows";                          break;
                case 0x11 :  info = "columns";                       break;
                case 0x30 :  info = "pixel spacing";                 break;
                case 0x31 :  info = "zoom factor";                   break;
                case 0x32 :  info = "zoom center";                   break;
                case 0x34 :  info = "pixel aspect ratio";            break;
                case 0x40 :  info = "image format (ret)";            break;
                case 0x50 :  info = "manipulated image (ret)";       break;
                case 0x51 :  info = "corrected image";               break;
                case 0x60 :  info = "compression code (ret)";        break;
                case 0x0100: info = "bits allocated";                break;
                case 0x0101: info = "bits stored";                   break;
                case 0x0102: info = "high bit";                      break;
                case 0x0103: info = "pixel representation";          break;
                case 0x0200: info = "image location (ret)";          break;
                case 0x1050: info = "window center";                 break;
                case 0x1051: info = "window width";                  break;
                case 0x1052: info = "rescale intercept";             break;
                case 0x1053: info = "rescale slope";                 break;
                case 0x1100: info = "gray lookup table desc (ret)";  break;
                case 0x1200: info = "gray lookup table data (ret)";  break;
                case 0x1201: info="red table";                       break;
                case 0x1202: info="green table";                     break;
                case 0x1203: info="blue table";                      break;
            }
            break;

        case 0x4000 :
            info = "text";                                           break;

        case 0x7FE0 :
            switch (element)  {
                case 0x00 :  info = "pixel data";                    break;
                case 0x10 :  info = "pixel data";                    break;
            }
            break;
        default :
            if (group >= 0x6000 && group <= 0x601e && (group & 1) == 0) info = "overlay";
            if (element == 0x0000)  info = "group length";
            if (element == 0x4000)  info = "comments";
            break;
    }  //end switch group

    return info;
}

const dicom::CDicomDictionary::sDicomTypes dicom::CDicomDictionary::GetType(const int32& group, const int32&element)
{
    std::string info("unrecognized");
    CDicomDictionary::sDicomTypes dtType=unknown;
    switch (group)  
	{
        //case 0x0002 :
        //    switch (element)  
		//    {
        //        case 0x00 :  info = "file meta elements group len"; break;
        //        case 0x01 :  info = "file meta info version";       break;
        //        case 0x02 :  info = "media storage SOP class uid";  break;
        //        case 0x03 :  info = "media storage SOP inst uid";   break;
        //        case 0x10 :  info = "transfer syntax uid";          break;
        //        case 0x12 :  info = "implementation class uid";     break;
        //        case 0x13 :  info = "implementation version name";  break;
        //        case 0x16 :  info = "source app entity title";      break;
        //        case 0x100:  info = "private info creator uid";     break;
        //        case 0x102:  info = "private info";                 break;
        //    }
        //    break;

        case 0x0008 :
            switch (element)  
			{
        //        case 0x00 :  info = "identifying group";             break;
        //        case 0x01 :  info = "length to end";                 break;
                case 0x05 :  info = "specific character set";  dtType=CDicomDictionary::cs; break;
                case 0x08 :  info = "image type";              dtType=CDicomDictionary::cs; break;
        //        case 0x10 :  info = "recognition code";              break;
                case 0x16 :  info = "SOP Class UID";           dtType=CDicomDictionary::ui; break;
        //        case 0x18 :  info = "SOP Instance UID";              break;
        //        case 0x20 :  info = "study date";                    break;
        //        case 0x21 :  info = "series date";                   break;
        //        case 0x22 :  info = "acquisition date";              break;
        //        case 0x23 :  info = "image date";                    break;
        //        case 0x30 :  info = "study time";                    break;
        //        case 0x31 :  info = "series time";                   break;
        //        case 0x32 :  info = "acquisition time";              break;
        //        case 0x33 :  info = "image time";                    break;
        //        case 0x40 :  info = "data set type";                 break;
        //        case 0x41 :  info = "data set subtype";              break;
        //        case 0x50 :  info = "accession number";              break;
                case 0x60 :  info = "modality";                dtType=CDicomDictionary::cs; break;
        //        case 0x70 :  info = "manufacturer";                  break;
                case 0x80 :  info = "institution name";        dtType=CDicomDictionary::lo; break;
        //        case 0x90 :  info = "referring physician's name";    break;
        //        case 0x1010: info = "station name";                  break;
        //        case 0x103e: info = "series description";            break;
        //        case 0x1030: info = "study description";             break;
        //        case 0x1040: info = "institutional dept. name";      break;
                case 0x1050: info = "performing physician's name"; dtType=CDicomDictionary::pn; break;
        //        case 0x1060: info = "name phys(s) read stdy";        break;
                case 0x1070: info = "operator's name";         dtType=CDicomDictionary::pn; break;
                case 0x1080: info = "admitting diagnoses description"; dtType=CDicomDictionary::lo; break;
        //        case 0x1090: info = "manufacturer's model name";     break;
                case 0x1140: info = "referenced image sequence"; dtType=CDicomDictionary::string; break;
            }
            break;

        case 0x0010 :
            switch (element)  
			{
        //        case 0x00 :  info = "patient group";                 break;
                case 0x10 :  info = "patient name";            dtType=CDicomDictionary::pn; break;
                case 0x20 :  info = "patient ID";              dtType=CDicomDictionary::lo; break;
                case 0x30 :  info = "patient birthdate";       dtType=CDicomDictionary::da; break;
                case 0x40 :  info = "patient sex";             dtType=CDicomDictionary::cs; break;
                case 0x1010: info = "patient age";             dtType=CDicomDictionary::as; break;
                case 0x1030: info = "patient weight";          dtType=CDicomDictionary::ds; break;
                case 0x21b0: info = "additional patient history";
                                                               dtType=CDicomDictionary::lt; break;
            }
            break;

        case 0x0018 :
            switch (element)  
			{
        //        case 0x00 :  info = "acquisition group";             break;
                case 0x10 :  info = "contrast/bolus agent";    dtType=CDicomDictionary::lo; break;
                case 0x15 :  info = "body part examined";      dtType=CDicomDictionary::cs; break;
                case 0x20 :  info = "scanning sequence";       dtType=CDicomDictionary::cs; break;
                case 0x21 :  info = "Sequence Variant";        dtType=CDicomDictionary::cs; break;
                case 0x22 :  info = "Scan Options";            dtType=CDicomDictionary::cs; break;
                case 0x23 :  info = "MR Acquisition Type";     dtType=CDicomDictionary::cs; break;
                case 0x24 :  info = "Sequence Name";           dtType=CDicomDictionary::sh; break;
                case 0x25 :  info = "Angio Flag";              dtType=CDicomDictionary::cs; break;
                case 0x30 :  info = "radionuclide";            dtType=CDicomDictionary::lo; break;
                case 0x50 :  info = "slice thickness";         dtType=CDicomDictionary::ds; break;
                case 0x60:   info = "kvp";                     dtType=CDicomDictionary::ds; break;
                case 0x80 :  info = "repetition time";         dtType=CDicomDictionary::ds; break;
                case 0x81 :  info = "echo time";               dtType=CDicomDictionary::ds; break;
                case 0x82 :  info = "inversion time";          dtType=CDicomDictionary::ds; break;
                case 0x83 :  info = "Number of Averages";      dtType=CDicomDictionary::ds; break;
                case 0x84 :  info = "Imaging Frequency";       dtType=CDicomDictionary::ds; break;
                case 0x85 :  info = "Imaged Nucleus";          dtType=CDicomDictionary::sh; break;
                case 0x86 :  info = "Echo Number";             dtType=CDicomDictionary::is; break;
                case 0x87 :  info = "Magnetic Field Strength"; dtType=CDicomDictionary::ds; break;
                case 0x88 :  info = "Spacing Between Slices";  dtType=CDicomDictionary::ds; break;
                case 0x91 :  info = "Echo Train Length";       dtType=CDicomDictionary::is; break;
                case 0x93 :  info = "Percent Sampling";        dtType=CDicomDictionary::ds; break;
                case 0x94 :  info = "Percent Phase Field of View"; dtType=CDicomDictionary::ds; break;
                case 0x95 :  info = "Pixel Bandwidth";         dtType=CDicomDictionary::ds; break;
                case 0x1020: info = "software version(s)";     dtType=CDicomDictionary::lo; break;
                case 0x1030: info = "protocol name";           dtType=CDicomDictionary::lo; break;
                case 0x1040: info = "Contrast/Bolus Route";    dtType=CDicomDictionary::lo; break;
                case 0x1062: info = "Nominal Interval";        dtType=CDicomDictionary::is; break;
                case 0x1088: info = "Heart Rate";              dtType=CDicomDictionary::is; break;
                case 0x1090: info = "Cardiac Number of Images";    dtType=CDicomDictionary::is; break;
                case 0x1094: info = "Trigger Window";          dtType=CDicomDictionary::is; break;
                case 0x1100: info = "Reconstruction Diameter"; dtType=CDicomDictionary::ds; break;
                case 0x1120: info = "gantry/detector tilt";    dtType=CDicomDictionary::ds; break;
                case 0x1150: info = "exposure time";           dtType=CDicomDictionary::is; break;
                case 0x1151: info = "x-ray tube current";      dtType=CDicomDictionary::is; break;
                case 0x1210: info = "convolution kernel";      dtType=CDicomDictionary::sh; break;
                case 0x1250: info = "Receiving Coil";          dtType=CDicomDictionary::sh; break;
                case 0x1251: info = "Transmitting Coil";       dtType=CDicomDictionary::sh; break;
                case 0x1310: info = "Acquisition Matrix";      dtType=CDicomDictionary::us; break;
                case 0x1314: info = "Flip Angle";              dtType=CDicomDictionary::ds; break;
                case 0x1315: info = "Variable Flip Angle Flag";   dtType=CDicomDictionary::cs; break;
                case 0x1316: info = "SAR";                     dtType=CDicomDictionary::ds; break;
                case 0x1400: info = "Acquisition Device Processing Description";  dtType=CDicomDictionary::lo; break;
                case 0x1401: info = "Acquisition Device Processing Code"; dtType=CDicomDictionary::lo; break;
                case 0x5100: info = "Patient Position";        dtType=CDicomDictionary::cs; break;
                case 0x5101: info = "View Position";           dtType=CDicomDictionary::cs; break;
                case 0x6000: info = "Sensitivity";             dtType=CDicomDictionary::ds; break;
            }
            break;

        case 0x0020 :
            switch (element)  
			{
        //        case 0x00 :  info = "relationship group";            break;
                case 0x0d :  info = "Study Instance UID";      dtType=CDicomDictionary::ui; break;
                case 0x0e :  info = "Series Instance UID";     dtType=CDicomDictionary::ui; break;
                case 0x10 :  info = "study id";                dtType=CDicomDictionary::sh; break;
                case 0x11 :  info = "series number";           dtType=CDicomDictionary::is; break;
                case 0x12 :  info = "acquisition number";      dtType=CDicomDictionary::is; break;
                case 0x13 :  info = "image number";            dtType=CDicomDictionary::is; break;
                case 0x20 :  info = "patient orientation";     dtType=CDicomDictionary::cs; break;
        //        case 0x30 :  info = "image position (ret)";          break;
                case 0x32 :  info = "Image Position (Patient)"; dtType=CDicomDictionary::ds; break;
        //        case 0x35 :  info = "image orientation (ret)";       break;
                case 0x37 :  info = "Image Orientation (Patient)"; dtType=CDicomDictionary::ds; break;
        //        case 0x50 :  info = "location (ret)";                break;
                case 0x52 :  info = "Frame of Reference UID";  dtType=CDicomDictionary::ui; break;
        //        case 0x60 :  info = "Laterality";                    break;
        //        case 0x1002: info = "images in acquisition";         break;
        //        case 0x1040: info = "position reference";            break;
                case 0x1041: info = "slice location";          dtType=CDicomDictionary::ds; break;
        //        case 0x3401: info = "modifying device id";           break;
        //        case 0x3402: info = "modified image id";             break;
        //        case 0x3403: info = "modified image date";           break;
        //        case 0x3404: info = "modifying device mfg.";         break;
        //        case 0x3405: info = "modified image time";           break;
        //        case 0x3406: info = "modified image desc.";          break;
                case 0x4000: info = "image comments";          dtType=CDicomDictionary::lt; break;
        //        case 0x5000: info = "original image id";             break;
            }
            break;

        case 0x0028 :
            switch (element)  
			{
        //        case 0x00 :  info = "image presentation group";      break;
        //        case 0x02 :  info = "samples per pixel";             break;
                case 0x04 :  info = "photometric interpretation"; dtType=CDicomDictionary::cs; break;
        //        case 0x05 :  info = "image dimensions (ret)";        break;
        //        case 0x06 :  info = "planar configuration";          break;
        //        case 0x08 :  info = "number of frames";              break;
        //        case 0x09 :  info = "frame increment pointer";       break;
        //        case 0x10 :  info = "rows";                          break;
        //        case 0x11 :  info = "columns";                       break;
        //        case 0x30 :  info = "pixel spacing";                 break;
        //        case 0x31 :  info = "zoom factor";                   break;
        //        case 0x32 :  info = "zoom center";                   break;
        //        case 0x34 :  info = "pixel aspect ratio";            break;
        //        case 0x40 :  info = "image format (ret)";            break;
        //        case 0x50 :  info = "manipulated image (ret)";       break;
        //        case 0x51 :  info = "corrected image";               break;
        //        case 0x60 :  info = "compression code (ret)";        break;
        //        case 0x0100: info = "bits allocated";                break;
        //        case 0x0101: info = "bits stored";                   break;
        //        case 0x0102: info = "high bit";                      break;
        //        case 0x0103: info = "pixel representation";          break;
        //        case 0x0200: info = "image location (ret)";          break;
        //        case 0x1050: info = "window center";                 break;
        //        case 0x1051: info = "window width";                  break;
        //        case 0x1052: info = "rescale intercept";             break;
        //        case 0x1053: info = "rescale slope";                 break;
        //        case 0x1100: info = "gray lookup table desc (ret)";  break;
        //        case 0x1200: info = "gray lookup table data (ret)";  break;
        //        case 0x1201: info="red table";                       break;
        //        case 0x1202: info="green table";                     break;
        //        case 0x1203: info="blue table";                      break;
            }
            break;

        //case 0x4000 :break
        //case 0x7FE0 :break;
        default : break;
    }  //end switch group
    return dtType;
}

}// end namespace
