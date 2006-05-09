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


#ifndef DICOMSEL_DCMTK_H
#define DICOMSEL_DCMTK_H

#ifdef _WIN32
# undef HAVE_CONFIG_H
# undef HAVE_CONST_CAST
# undef HAVE_DYNAMIC_CAST
# undef HAVE_REINTERPRET_CAST
# undef HAVE_STATIC_CAST
# define ssize_t scmtk_ssize_t
# include <dcmtk/config/cfwin32.h>
# undef ssize_t
#endif // _WIN32

#endif // DICOMSEL_DCMTK_H

/* End of File */
