/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENCE >>
 *
 * ---------------------------------------------------------------------------
 */


#ifndef DICOMSEL_VERSION_H
#define DICOMSEL_VERSION_H

// DicomSel application name
#define VERSION_NAME "DicomSel"

// DicomSel version information
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_RELEASE 9

// String making macros
#define MAKE_STRING2( s ) #s
#define MAKE_STRING( s )  MAKE_STRING2( s )

// DicomSel version string
#define VERSION_STRING MAKE_STRING( VERSION_MAJOR ) "." \
		       MAKE_STRING( VERSION_MINOR ) "." \
		       MAKE_STRING( VERSION_RELEASE )

#endif // DICOMSEL_VERSION_H

/* End of File */
