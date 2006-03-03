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

# Windows resource handling
if HAVE_RC
include $(top_srcdir)/rc.am
RC_OBJECTS = resources.$(OBJEXT)
else
RC_OBJECTS =
endif

# Source files
noinst_PROGRAMS = rf
rf_SOURCES = \
    main.cpp \
    MainFrame.cpp \
    MainFrame.h
rf_DEPENDENCIES = \
    $(RC_OBJECTS)
rf_LDADD = $(WX_LIBS)

# Extra files to include into the distribution archive: icons
EXTRA_DIST = \
    resources.rc \
    README

# End of File
