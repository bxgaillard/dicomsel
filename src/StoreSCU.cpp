/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005, 2006 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENSE >>
 *
 * ---------------------------------------------------------------------------
 */


#include <dicomsel/dcmtk.h>

#define opt_acse_timeout  storescu_opt_acse_timeout
#define opt_dimse_timeout storescu_opt_dimse_timeout
#define opt_blockMode     storescu_opt_blockMode
#define main              storescu_main
extern "C" static int main( int argc, char *argv[] );

#ifdef _MSC_VER
# pragma warning( disable: 4267 4244 )
#endif // _MSC_VER
#include "dcmtkapp/storescu.cc"
#ifdef _MSC_VER
# pragma warning( default: 4267 4244 )
#endif // _MSC_VER

#undef main

#include <dicomsel/StoreSCU.h>

namespace dicomsel
{

int StoreSCU::Main( int argc, char *argv[] )
{
    return storescu_main( argc, argv );
}

} // namespace dicomsel

/* End of File */
