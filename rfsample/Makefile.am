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
AM_CPPFLAGS = -I$(top_srcdir)/include $(WX_CPPFLAGS)
AM_CXXFLAGS = $(WX_CXXFLAGS_ONLY)

# Source files
noinst_PROGRAMS = rf
rf_SOURCES = \
    main.cpp \
    MainFrame.cpp \
    MainFrame.h \
    resources.rc
rf_LDADD = $(WX_LIBS)

# Extra files to include into the distribution archive: icons
EXTRA_DIST = \
    README

# End of File
