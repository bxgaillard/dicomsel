# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# << LICENSE >>
#
# ----------------------------------------------------------------------------


# Additional flags
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/libdicom/include \
	      -I$(top_srcdir)/libdicom/include/dicom $(WX_CPPFLAGS)
AM_CXXFLAGS = $(WX_CXXFLAGS_ONLY)

# Source files
bin_PROGRAMS = dicomsel
dicomsel_SOURCES = \
    main.cpp \
    BitmapPanel.cpp \
    DicomCollection.cpp \
    DicomFile.cpp \
    DicomTree.cpp \
    MainFrame.cpp \
    TagDialog.cpp \
    TagSet.cpp \
    resources.rc
dicomsel_LDADD = -L../libdicom/src -ldicom $(WX_LIBS)

# Extra files to include into the distribution archive: icons
EXTRA_DIST = \
    README \
    images/README \
    images/dir.xpm \
    images/icon.icns \
    images/icon.ico \
    images/icon.xpm \
    images/image.xpm \
    images/patient.xpm \
    images/series.xpm \
    images/study.xpm

# End of File
