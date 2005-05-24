// DicomDictionary.h
#ifndef DICOM_DICTIONARY_H
#define DICOM_DICTIONARY_H

#include "Virtuals/Type.h"
#include <string>

namespace dicom
{
/**
 * CDicomDictionary
 *********************
 * Namespace : dicom
 *********************
 * Description: 
 *
 *********************
 */
class CDicomDictionary
{
public :

    /**
     * All type used in DICOM file
     */
    typedef enum { 
        unknown,
        i8,
        i16,
        i32,
        ui8, 
        ui16, 
        us=ui16, 
        ui32,
        string=10,
        ae=string, 
        as=string, 
        cs=string, 
        da=string,
        ds=string, 
        dt=string, 
        is=string, 
        lo=string,
        lt=string, 
        pn=string, 
        sh=string, 
        st=string,
        tm=string, 
        ui=string
    } sDicomTypes;

    static const dicom::CDicomDictionary::sDicomTypes GetType(const int32&, const int32&) ;
    static const std::string GetNameElement(const int32&, const int32&) ;

};
}// end namespace
#endif // DICOM_DICTIONARY_H
