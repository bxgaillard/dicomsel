# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ----------------------------------------------------------------------------


# Additional flags
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/libdicom/include \
	      -I$(top_srcdir)/3rdparty/dcmtk-$(DCMTK_VERSION)/include \
	      -I$(top_srcdir)/libdicom/include/dicom $(WX_CPPFLAGS)
AM_CXXFLAGS = $(WX_CXXFLAGS_ONLY)

# DCMTK libs: put here which ones are used (-ldcmimage -ldcmnet...)
DCMTK_LIBS = -ldcmimage -ldcmnet -ldcmdata -ldcmtls -lofstd

# Windows resource handling
if HAVE_RC
include $(top_srcdir)/rc.am
RC_OBJECTS = resources.$(OBJEXT)
else
RC_OBJECTS =
endif

# Source files
bin_PROGRAMS = dicomsel
dicomsel_SOURCES = \
    BitmapPanel.cpp \
    DicomCollection.cpp \
    DicomFile.cpp \
    DicomTree.cpp \
    StoreDialog.cpp \
    MainFrame.cpp \
    TagDialog.cpp \
    TagSet.cpp \
    Store.cpp \
    main.cpp
dicomsel_DEPENDENCIES = \
    $(RC_OBJECTS)
dicomsel_LDADD = $(RC_OBJECTS) -L../3rdparty/dcmtk-$(DCMTK_VERSION) \
		 $(DCMTK_LIBS) -L../libdicom/src -ldicom $(WX_LIBS)

# Extra files to include into the distribution archive: icons
EXTRA_DIST = \
    Makefile.msc \
    README \
    resources.rc \
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
