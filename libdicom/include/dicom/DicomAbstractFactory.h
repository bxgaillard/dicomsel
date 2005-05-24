// DicomAbstractFactory.h
#ifndef DICOM_ABSCTRACTFACTORY_H
#define DICOM_ABSCTRACTFACTORY_H

#include "Virtuals/Type.h"

namespace dicom
{

namespace tag
{
    class CDicomTag;
}

namespace visitor
{
    class CDicom;
}


class CDicomAbstractFactory
{
public:
    static tag::CDicomTag* CreateTag(const int32, const int32, const int32, dicom::io::CDicomFile &, dicom::visitor::CDicom &);
};

}// end namespace

#endif // DICOM_ABSCTRACTFACTORY_H
