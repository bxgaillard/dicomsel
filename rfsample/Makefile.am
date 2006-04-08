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
    MainFrame.cpp \
    MainFrame.h \
    main.cpp
rf_DEPENDENCIES = \
    $(RC_OBJECTS)
rf_LDADD = $(WX_LIBS)

# Extra files to include into the distribution archive: icons
EXTRA_DIST = \
    Makefile.msc \
    README \
    resources.rc

# End of File
